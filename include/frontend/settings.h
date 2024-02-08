#pragma once

#include "../include.h"
#include "../qt core.h"
#include "../threading.h"

struct App;

class Settings_Tab : public Form {
	Q_OBJECT
public:
	App* app;
	Settings_Tab(App* i_app);
};