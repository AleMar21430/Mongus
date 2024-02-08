#pragma once

#include "include.h"

#include "settings.h"
#include "threading.h"

class Main_Window;

struct App : QApplication {
	Main_Window* window;
	Text_Stream* log;

	cpr::Header headers;
	unordered_map<QString, QString> settings;

	unordered_map<Remote_Request_Type, uint32_t> remote_requests;
	unordered_map<Local_Request_Type, uint32_t> local_requests;

	QPixmap error_image;

	App(int argc, char* argv[]);

	void init();
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