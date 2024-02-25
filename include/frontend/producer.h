#pragma once

#include "../include.h"
#include "../qt core.h"
#include "../threading.h"

class App;

class Producer_Tab : public Linear_Contents {
	Q_OBJECT
public:
	App* app;

	Producer_Tab(App* i_app);
};