#pragma once

#include "../include.h"
#include "../qt core.h"
#include "../threading.h"

class App;

class Movie_Listings_Tab : public Linear_Contents {
	Q_OBJECT
public:
	App* app;

	Movie_Listings_Tab(App* i_app);

	void activate();
};