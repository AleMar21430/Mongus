#pragma once

#include "../include.h"
#include "../qt core.h"
#include "../threading.h"

class App;

class Studio_Tab : public Linear_Contents {
	Q_OBJECT
public:
	App* app;

	Studio_Tab(App* i_app);
};