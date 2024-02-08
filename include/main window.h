#pragma once

#include "include.h"

#include "settings.h"
#include "threading.h"

class Main_Window;

struct App : QApplication {
	Main_Window* window;
	Text_Stream* log;

	unordered_map<QString, QString> settings;
	unordered_map<Async_Type, uint32_t> async_requests;
	mongo::instance mongo_driver;

	QPixmap error_image;

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

	// Tabs
	Settings* settings;

	Main_Window(App* i_app = nullptr);

public slots:
	void logMessage(const QString& i_message);
};