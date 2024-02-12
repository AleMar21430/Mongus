#pragma once

#include "../include.h"
#include "../qt core.h"
#include "../threading.h"

class App;

class Add_Showing_Tab : public Linear_Contents {
	Q_OBJECT
public:
	App* app;

	List* list;

	Add_Showing_Tab(App* i_app);
};