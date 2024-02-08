#include "frontend/movie listings.h"

#include "main window.h"

Movie_Listings_Tab::Movie_Listings_Tab(App* i_app) :
	Linear_Contents(),
	app(i_app)
{
	connect(app->mongo_thread, &Mongo_Thread::result, [this](const Mongo_Query& query, const json& data) {
		if (query.type == Mongo_Type::LISTINGS && query.request_id == app->mongo_request) {
			json processed_data = data[0];
			app->log->append(QString::fromStdString(processed_data.dump()));
		}
	});
}

void Movie_Listings_Tab::activate() {
	app->mongo_request++;
	app->mongo_thread->cancelWork();

	Mongo_Query work = Mongo_Query({ {"collection", "movies"} }, Mongo_Type::LISTINGS, app->mongo_request);
	app->mongo_thread->queueWork(work);
}