#include "TodoApp.hpp"

#include "../components/components.hpp"
#include "../util/FormDataParser.hpp"

#include <chrono>
#include <fmt/chrono.h>
#include <fmt/printf.h>

#include <filesystem>
namespace fs = std::filesystem;

TodoApp::TodoApp(const TodoAppConfig &config)
	: config(config),
	  db(std::make_shared<sql::Sqlite>(config.dbPath.c_str())),
	  fileCache(),
	  projSvc(db),
	  taskSvc(db, projSvc)
{
	db->execute(Project::DDL);
	db->execute(Task::DDL);
	
	importViews();
	initSiteEndpoints();
	initFormEndpoints();
	initMgmtEndpoints();
	if(config.devMode) {
		initDevMode();
	}
	
	ProjectComponent::setTemplateSource(fileCache.getSource("res/components/project.html"));
	TaskComponent::setTemplateSource(fileCache.getSource("res/components/task.html"));
	TaskDetailsComponent::setTemplateSource(fileCache.getSource("res/components/taskdetails.html"));
	ProjectDetailsComponent::setTemplateSource(fileCache.getSource("res/components/projectdetails.html"));
	ProjectFormComponent::setTemplateSource(fileCache.getSource("res/components/projectform.html"));
}

void TodoApp::run()
{
	crowApp
		.bindaddr(config.serverAddr)
		.port(config.serverPort)
		.run();
}


void TodoApp::importViews(const std::string &dirname)
{
	views.clear();
	for(const auto& entry: fs::recursive_directory_iterator(dirname)) {
		const auto& path = entry.path();
		if(path.extension() == ".html") {
			views.emplace(path.stem().string(), HTMLView(fileCache, path.string()));
		}
	}
	
	CROW_LOG_INFO << fmt::format("Imported {} views from {}", views.size(), dirname);
}


void TodoApp::initSiteEndpoints()
{
	CROW_ROUTE(crowApp, "/")([&]() {
		auto projects = projSvc.getProjects();
		
		auto view = views.at("homepage").createInstance();
		view.addComponent<ForEachComponent<ProjectComponent>>("projlist", {
			.elements = projects
		});
		
		return renderView(view, "homepage");
	});
	
	
	CROW_ROUTE(crowApp, "/projects")([&]() {
		auto projects = projSvc.getProjects();
		
		auto view = views.at("projects").createInstance();
		view.addComponent<ForEachComponent<ProjectComponent>>("projlist", {
			.elements = projects
		});
		
		return renderView(view, "projects");
	});
	
	
	CROW_ROUTE(crowApp, "/projects/<int>")([&](int projectId) {
		auto optProject = projSvc.getProject(projectId);
		if(!optProject) {
			return renderView(views.at("404").createInstance(), "not found", 404);
		}
		
		auto view = views.at("projectdetails").createInstance();
		view.addComponent<ProjectDetailsComponent>("prjdetails", *optProject);
		
		return renderView(view, "project details");
	});
	
	
	CROW_ROUTE(crowApp, "/projects/<int>/edit")([&](int projectId) {
		auto optProject = projSvc.getProject(projectId);
		if(!optProject) {
			return renderView(views.at("404").createInstance(), "not found", 404);
		}
		
		auto view = views.at("projectedit").createInstance();
		view.addComponent<PlaintextComponent>("prjname", optProject->name);
		view.addComponent<ProjectFormComponent>("prjform", {
			.project = *optProject,
			.action = fmt::format("/forms/projects/{}/edit", optProject->id)
		});
		
		return renderView(view, "edit project");
	});
	
	
	CROW_ROUTE(crowApp, "/projects/<int>/tasks")([&](int projectId) {
		auto tasks = taskSvc.getTaskHeaderList(projectId);
		return "project task list stub"; //TODO
	});
	
	
	CROW_ROUTE(crowApp, "/task/<string>")([&](const std::string& taskName) {
		auto optTask = taskSvc.getTaskByName(taskName);
		return "task details stub"; //TODO
	});
	
	
	CROW_ROUTE(crowApp, "/task/<string>/edit")([&](const std::string& taskName) {
		auto optTask = taskSvc.getTaskByName(taskName);
		return "task edit stub"; //TODO
	});
}

void TodoApp::initFormEndpoints()
{
	CROW_ROUTE(crowApp, "/forms/projects/<int>/edit")
	.methods(crow::HTTPMethod::POST)([&](const crow::request& req, int projectId) {
		auto view = views.at("formecho").createInstance();
		view.addComponent<DictComponent>("kv", ParseQueryString(req.body));
		return renderView(view);
	});
	
	
}

void TodoApp::initMgmtEndpoints()
{
	
	CROW_ROUTE(crowApp, "/cstatic/<path>")([&](const std::string &path) {
		//TODO check for spooky shit in path (like ../../../)
		fs::path fullPath = fs::path("res/static").append(path);
		crow::response ret;
		ret.set_static_file_info(fullPath.string());
		ret.set_header("Cache-Control", "public, max-age=3600, immutable");
		return ret;
	});
}

void TodoApp::initDevMode()
{
	using namespace std::chrono_literals;
	
	dirWatcher = std::make_unique<DirWatcher>("res");
	
	crowApp.tick(500ms, [&](){
		if(!dirWatcher->detectChanges()) {
			return;
		}
		CROW_LOG_INFO << fmt::format("file change detected, reloading & triggering refresh");
		for(auto& [_, view]: views) {
			view.reload();
		}
		for(auto& [_, connection]: devModeRefreshSockets) {
			connection.get().send_text("refresh");
		}
		fileCache.invalidate();
	});
	
	CROW_WEBSOCKET_ROUTE(crowApp, "/devmode/refresh")
		.onopen([&](crow::websocket::connection& conn){
			devModeRefreshSockets.insert({conn.get_remote_ip(), conn});
		})
		.onclose([&](crow::websocket::connection& conn, const std::string& reason) {
			devModeRefreshSockets.erase(conn.get_remote_ip());
		})
		.onmessage([&](crow::websocket::connection& conn, const std::string& msg, bool isBinary) {
			//nop
		});
	
	//fileCache.cacheRecursive("res");
}


crow::response TodoApp::renderView(const HTMLViewInstance &view, const std::string_view title, int status, const std::string& contentType)
{
	auto commonView = views.at("common").createInstance();
	commonView.addComponent<PlaintextComponent>("title", std::string(title));
	commonView.addComponent<SubviewComponent>("content", view);
	
	if(config.devMode) {
		commonView.addComponent<PlaintextComponent>("devmodehdr", R"html(
			<script src="/static/js/devrefresh.js"></script>
		)html");
	}
	return crow::response(status, contentType, commonView.render());
}

void TodoApp::initExampleData()
{

	auto p1id = projSvc.createProject({
		.name = "Test project",
		.shortName = "TST",
		.description = "bruh"
	});

	taskSvc.createTask({
		.projectId = p1id,
		.title = "do thing",
		.description = "thing needs to be done",
		.dueDate = 0
	});

	taskSvc.createTask({
		.projectId = p1id,
		.title = "do another thing",
		.description = "another thing needs to be done",
		.dueDate = 0
	});
}
