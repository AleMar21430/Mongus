#pragma once

#include "../include.h"
#include "../qt core.h"
#include "../threading.h"

class App;

class Actor_Tab : public Linear_Contents {
	Q_OBJECT
public:
	App* app;

	Actor_Tab(App* i_app);
};