#include "TodoApp.hpp"

#include "../components/components.hpp"
#include "../util/FormDataParser.hpp"
#include "../time/Time.hpp"

#include "../dto/dto.hpp"

#include <chrono>
#include <fmt/chrono.h>
#include <fmt/printf.h>

#include <filesystem>
namespace fs = std::filesystem;

TodoApp::TodoApp(const TodoAppConfig &config)
	: config(config),
	  db(std::make_shared<sql::Sqlite>(config.dbPath.c_str())),
	  dbInit([this](){
		  db->execute(Project::DDL);
		  db->execute(Task::DDL);
	  }),
	  fileCache(),
	  projSvc(db),
	  taskSvc(db, projSvc)
{
	
	importViews();
	initSiteEndpoints();
	initFormEndpoints();
	initMgmtEndpoints();
	
	fmt::print("dev mode = {}\n", config.devMode);
	
	if(config.devMode) {
		initDevMode();
	}
	
	ProjectComponent::setTemplateSource(fileCache.getSource("res/components/project.html"));
	TaskComponent::setTemplateSource(fileCache.getSource("res/components/task.html"));
	TaskDetailsComponent::setTemplateSource(fileCache.getSource("res/components/taskdetails.html"));
	ProjectDetailsComponent::setTemplateSource(fileCache.getSource("res/components/projectdetails.html"));
	ProjectFormComponent::setTemplateSource(fileCache.getSource("res/components/projectform.html"));
	TaskFormComponent::setTemplateSource(fileCache.getSource("res/components/taskform.html"));
}


/**
 * TODO MISSING ENDPOINTS
 *  - delete project confirmation
 *  - delete project action
 */

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
		
		auto activeTasks = taskSvc.getActiveTaskHeaderList();
		auto view = views.at("homepage").createInstance();
		view.addComponent<ForEachComponent<ProjectComponent>>("projlist", {
			.elements = projects
		});
		view.addComponent<ForEachComponent<TaskComponent>>("tasklist", {
			.elements = activeTasks,
			.emptyFallbackText = "No active tasks."
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
		
		auto tasks = taskSvc.getTaskHeaderList(projectId);
		
		auto view = views.at("projectdetails").createInstance();
		view.addComponent<ProjectDetailsComponent>("prjdetails", *optProject);
		view.addComponent<PlaintextComponent>("prjname", optProject->name);
		view.addComponent<ForEachComponent<TaskComponent>>("tasklist", {
			.elements = tasks
		});
		
		
		return renderView(view, "project details");
	});
	
	
	CROW_ROUTE(crowApp, "/projects/new")([&]() {
		
		Project blankProject = {
			.name = "",
			.shortName = "",
			.taskCounter = 0
		};
		
		auto view = views.at("projectcreate").createInstance();
		view.addComponent<ProjectFormComponent>("prjform", {
			.project = blankProject,
			.edit = false
		});
		
		return renderView(view, "edit project");
	});
	
	CROW_ROUTE(crowApp, "/projects/<int>/edit")([&](int projectId) {
		auto optProject = projSvc.getProject(projectId); //TODO getOr404
		if(!optProject) {
			return renderView(views.at("404").createInstance(), "not found", 404);
		}

		auto view = views.at("projectedit").createInstance();
		view.addComponent<ProjectFormComponent>("prjform", {
			.project = *optProject,
			.edit = true
		});
		
		return renderView(view, "edit project");
	});
	
	CROW_ROUTE(crowApp, "/projects/<int>/addtask")([&](int projectId) {
		auto optProject = projSvc.getProject(projectId);
		if(!optProject) {
			return renderView(views.at("404").createInstance(), "not found", 404);
		}
		
		Task blankTask = {
			.projectId = optProject->id,
			.name = optProject->nameOfNextTask(),
			.dueDate = Time::SecsSinceEpoch()
		};
		
		auto view = views.at("projtaskcreate").createInstance();
		view.addComponent<TaskFormComponent>("taskform", {
			.task = blankTask,
			.edit = false
		});
		
		return renderView(view, "add task");
	});
	
	CROW_ROUTE(crowApp, "/task/<string>")([&](const std::string& taskName) {
		auto optTask = taskSvc.getTaskByName(taskName);
		if(!optTask) {
			return renderView(views.at("404").createInstance(), "not found", 404);
		}
		
		auto view = views.at("taskdetails").createInstance();
		view.addComponent<TaskDetailsComponent>("taskdetails", *optTask);
		
		return renderView(view, "task details");
	});
	
	CROW_ROUTE(crowApp, "/task/<string>/updatestatus")([&](const crow::request& req, const std::string& taskName){
		auto optTask = taskSvc.getTaskByName(taskName);
		if(!optTask) {
			return renderView(views.at("404").createInstance(), "not found", 404);
		}

		int newStatus = std::stoi(req.url_params.get("value"));
		Task::StatusNames.at(newStatus);
		taskSvc.updateStatus(optTask->id, newStatus);

		crow::response response {303};
		response.set_header("Location", "/task/"+taskName);
		return response;
	});

	
	CROW_ROUTE(crowApp, "/task/<string>/edit")([&](const std::string& taskName) {
		auto optTask = taskSvc.getTaskByName(taskName);
		if(!optTask) {
			return renderView(views.at("404").createInstance(), "not found", 404);
		}
		
		auto view = views.at("taskedit").createInstance();
		view.addComponent<PlaintextComponent>("taskname", optTask->name);
		view.addComponent<TaskFormComponent>("taskform", {
			.task = *optTask,
			.edit = true
		});
		
		return renderView(view, "task edit");
	});
}

void TodoApp::initFormEndpoints()
{
	CROW_ROUTE(crowApp, "/projects/new/action")
	.methods(crow::HTTPMethod::POST)([&](const crow::request& req) {
		
		auto data = ParseQueryString(req.body);
		WriteProjectRequest writeReq = { //TODO form validation
			.name = data.at("prjname"),
			.shortName = data.at("prjshortname"),
			.description = data.at("prjdesc")
		};
		projSvc.createProject(writeReq);
		
		crow::response response {303};
		response.set_header("Location", "/");
		return response;
	});

	CROW_ROUTE(crowApp, "/projects/<int>/edit/action")
	.methods(crow::HTTPMethod::POST)([&](const crow::request& req, int projectId) {
		
		if(!projSvc.getProject(projectId)) {
			return crow::response(404);
		}
		
		auto data = ParseQueryString(req.body);
		WriteProjectRequest writeReq = { //TODO form validation
			.name = data.at("prjname"),
			.shortName = data.at("prjshortname"),
			.description = data.at("prjdesc")
		};
		projSvc.updateProject(projectId, writeReq);
		
		crow::response response {303};
		response.set_header("Location", fmt::format("/projects/{}", projectId));
		return response;
	});
	
	CROW_ROUTE(crowApp, "/projects/<int>/addtask/action")
	.methods(crow::HTTPMethod::POST)([&](const crow::request& req, int projectId) {
		if(!projSvc.getProject(projectId)) {
			return crow::response(404);
		}
		auto data = ParseQueryString(req.body);
		std::string dateStr = data.at("duedate") + " " + data.at("duetime");
		
		WriteTaskRequest writeReq = { //TODO form validation
			.projectId = projectId,
			.title = data.at("tasktitle"),
			.description = data.at("taskdesc"),
			.dueDate = Time::ParseDateTime(dateStr),
			.status = std::stoi(data.at("status"))
		};
		taskSvc.createTask(writeReq);
		
		crow::response response {303};
		response.set_header("Location", fmt::format("/projects/{}", projectId));
		return response;
	});
	
	CROW_ROUTE(crowApp, "/task/<string>/edit/action")
	.methods(crow::HTTPMethod::POST)([&](const crow::request& req, const std::string& taskName) {
		Task originalTask = taskSvc.getTaskByName(taskName).value();
		
		auto data = ParseQueryString(req.body);
		
		std::string dateStr = data.at("duedate") + " " + data.at("duetime");
		
		WriteTaskRequest writeReq = { //TODO form validation
			.projectId = originalTask.projectId,
			.title = data.at("tasktitle"),
			.description = data.at("taskdesc"),
			.dueDate = Time::ParseDateTime(dateStr),
			.status = std::stoi(data.at("status"))
		};
		taskSvc.editTask(originalTask.id, writeReq);
		
		crow::response response {303};
		response.set_header("Location", fmt::format("/task/{}", taskName));
		return response;
	});
	

	
}

void TodoApp::initMgmtEndpoints()
{
	
	CROW_ROUTE(crowApp, "/cstatic/<path>")([&](const std::string &path) {
		//TODO check for spooky shit in path (like ../../../)
		fs::path fullPath = fs::path("res/static").append(path);
		crow::response ret;
		ret.set_static_file_info(fullPath.string());
		
		if(config.devMode == false) {
			ret.set_header("Cache-Control", "public, max-age=3600, immutable");
		} else {
			ret.set_header("Cache-Control", "no-store");
		}
		return ret;
	});
	
	CROW_ROUTE(crowApp, "/init")([&](){
		initExampleData();
		return crow::response(200);
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
