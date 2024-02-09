#include "frontend/movie listings.h"

#include "main window.h"

Movie_Listings_Tab::Movie_Listings_Tab(App* i_app) :
	Linear_Contents(),
	app(i_app)
{
	list = new List();
	layout->addWidget(list);

	connect(app->mongo_thread, &Mongo_Thread::result, [this](const Mongo_Query& query, const json& data) {
		if (query.type == Mongo_Type::LISTINGS && query.request_id == app->mongo_request)
			process(data);
	});
}

void Movie_Listings_Tab::activate() {
	app->mongo_request++;
	app->mongo_thread->cancelWork();
	list->clear();
	Mongo_Query work = Mongo_Query({ {"collection", "movies"} }, Mongo_Type::LISTINGS, app->mongo_request);
	app->mongo_thread->queueWork(work);
}

void Movie_Listings_Tab::process(const json& data) {
	if (data.is_array()) {
		for (const auto& entry : data) {
			if (entry.contains("title")) {
				list->addItem(QString::fromStdString(entry["title"].dump()));
			}
		}
	}
}