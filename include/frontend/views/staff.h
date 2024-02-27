#pragma once

#include "../../include.h"
#include "../../qt core.h"
#include "../../threading.h"

class App;

class Staff_Tab : public QMainWindow {
	Q_OBJECT
public:
	App* app;

	Staff_Tab(App* i_app, const string& nombre);

public slots:
	void process(const json& data);
};