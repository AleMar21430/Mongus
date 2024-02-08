#pragma once

#include "../include.h"
#include "../qt core.h"
#include "../threading.h"

class App;

class User_Tab : public Linear_Contents {
	Q_OBJECT
public:
	App* app;

	User_Tab(App* i_app);
};