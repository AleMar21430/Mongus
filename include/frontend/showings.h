#pragma once

#include "../include.h"
#include "../qt core.h"
#include "../threading.h"

class App;

class Showings_Tab : public Linear_Contents {
	Q_OBJECT
public:
	App* app;

	Showings_Tab(App* i_app);
};