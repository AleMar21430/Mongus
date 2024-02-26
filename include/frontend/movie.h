#pragma once

#include "../include.h"
#include "../qt core.h"
#include "../threading.h"

class App;

class Movie_Tab : public QMainWindow {
	Q_OBJECT
public:
	App* app;

	Movie_Tab(App* i_app, const string& titulo);

public slots:
	void process(const json& data);
};