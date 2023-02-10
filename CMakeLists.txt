cmake_minimum_required(VERSION 3.20.0)
project(sigmatodo VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(Crow REQUIRED)
find_package(fmt REQUIRED)
find_package(SQLite3 REQUIRED)


add_executable(sigmatodo
		src/main.cpp
		src/time/Time.cpp
		src/svc/TaskService.cpp
		src/svc/ProjectService.cpp
		src/components/templated/ProjectComponent.cpp
		src/components/templated/TaskDetailsComponent.cpp
		src/view/HTMLView.cpp
		src/components/templated/TaskComponent.cpp
		src/app/TodoApp.cpp
		src/components/PlaintextComponent.cpp
		src/entities/Note.hpp
		src/components/SubviewComponent.cpp
		src/fs/FileCache.cpp
		src/fs/StringSource.cpp
		src/components/templated/ProjectDetailsComponent.cpp
		src/components/templated/ProjectFormComponent.cpp
		src/components/TextFileComponent.cpp src/fs/DirWatcher.cpp src/fs/DirWatcher.hpp src/util/FormDataParser.cpp src/util/FormDataParser.hpp src/components/DictComponent.cpp src/components/DictComponent.hpp src/components/templated/TaskFormComponent.cpp src/components/templated/TaskFormComponent.hpp)

target_link_libraries(sigmatodo PRIVATE Crow::Crow fmt::fmt SQLite::SQLite3)
target_include_directories(sigmatodo PRIVATE "cpp-sqlitelib/include")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")  #dev mode
	add_custom_command(TARGET sigmatodo POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E create_symlink
			${CMAKE_SOURCE_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/res)
else ()
	add_custom_command(TARGET sigmatodo POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_SOURCE_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/res)
endif ()


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
