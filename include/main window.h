#pragma once

#include "include.h"
#include "qt core.h"
#include "threading.h"

#include "frontend/lists/producers.h"
#include "frontend/lists/showings.h"
#include "frontend/lists/movies.h"
#include "frontend/lists/actors.h"
#include "frontend/lists/staffs.h"
#include "frontend/lists/users.h"

#include "frontend/views/producer.h"
#include "frontend/views/showing.h"
#include "frontend/views/movie.h"
#include "frontend/views/actor.h"
#include "frontend/views/staff.h"
#include "frontend/views/user.h"

#include "frontend/settings.h"

class Main_Window;

struct App : QApplication {
	Main_Window* window;
	Text_Stream* log;

	Mongo_Thread* mongo_thread;
	uint32_t mongo_request;

	QList<Async_Thread*> async_threads;
	uint32_t async_request;

	unordered_map<QString, QString> settings;

	App(int argc, char* argv[]);

	void load();
	void loadStyle();
	void loadSettings();

	int cleanup();
};

class Main_Window : public QMainWindow {
	Q_OBJECT
public:
	App* app;
	Tabs* tabs;

	Main_Window(App* app = nullptr);

	void changeView(const int& row);
public slots:
	void logMessage(const QString& message);
};