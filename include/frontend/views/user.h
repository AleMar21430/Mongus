#pragma once

#include "../../include.h"
#include "../../qt core.h"
#include "../../threading.h"

class App;

class User_Tab : public QMainWindow {
	Q_OBJECT
public:
	App* app;

	User_Tab(App* i_app, const string& nombre);

public slots:
	void process(const json& data);
};