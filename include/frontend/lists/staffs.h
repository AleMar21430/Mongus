#pragma once

#include "../../include.h"
#include "../../qt core.h"
#include "../../threading.h"

class App;

class Staffs_Tab : public Linear_Contents {
	Q_OBJECT
public:
	App* app;

	List* list;

	Staffs_Tab(App* i_app);

	void activate();
	void process(const json& data);
};