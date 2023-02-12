#ifndef SIGMATODO_TODOAPP_HPP
#define SIGMATODO_TODOAPP_HPP

#define CROW_STATIC_DIRECTORY "./res/static/"

#include "../svc/ProjectService.hpp"
#include "../svc/TaskService.hpp"

#include <memory>
#include <crow.h>

#include "../view/HTMLView.hpp"
#include "../fs/FileCache.hpp"
#include "../fs/DirWatcher.hpp"

struct CtorInit {
	inline explicit CtorInit(const std::function<void(void)>& fn)	{fn();}
};

struct TodoAppConfig {
	std::string serverAddr = "0.0.0.0";
	int serverPort = 9022;
	std::string dbPath = "./test.db";
	std::string adminKey = "dev";
	bool devMode = true;
	
	static inline TodoAppConfig Parse(const crow::json::rvalue& json)
	{
		return TodoAppConfig {
			.serverAddr = json["serverAddr"].s(),
			.serverPort = json["serverPort"].operator int(),
			.dbPath = json["dbPath"].s(),
			.adminKey = json["adminKey"].s(),
			.devMode = json["devMode"].b()
		};
	}
};

class TodoApp {
public:
	explicit TodoApp(const TodoAppConfig &config = {});

	void run();
	void initExampleData();
private:

	crow::response renderView(
		const HTMLViewInstance &view,
		const std::string_view title = "...",
		int status = 200,
		const std::string& contentType = "text/html"
	);
	
	void importViews(const std::string &dirname = "res/views");
	
	void initSiteEndpoints();
	void initFormEndpoints();
	void initMgmtEndpoints();
	void initDevMode();

	TodoAppConfig config;

	std::shared_ptr<sql::Sqlite> db;
	FileCache fileCache;
	std::unique_ptr<DirWatcher> dirWatcher;
	
	[[maybe_unused]]
	CtorInit dbInit;

	ProjectService projSvc;
	TaskService taskSvc;
	
	std::unordered_map<std::string, std::reference_wrapper<crow::websocket::connection>> devModeRefreshSockets;
	std::unordered_map<std::string, HTMLView> views; //TODO string_view lookup

	crow::SimpleApp crowApp;
};


#endif //SIGMATODO_TODOAPP_HPP
