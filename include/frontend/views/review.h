#pragma once

#include "../../include.h"
#include "../../qt core.h"
#include "../../threading.h"

class App;

class Review_Tab : public QMainWindow {
	Q_OBJECT
public:
	App* app;

	Review_Tab(App* i_app, const string& id);

public slots:
	void process(const string& review, const string& score, const string& date, const string& user, const string& id);
};