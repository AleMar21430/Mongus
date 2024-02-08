#pragma once

#include "include.h"
#include "qt core.h"

struct App;

class Settings : public Form {
	Q_OBJECT
public:
	App* app;
	Settings(App* i_app);
};