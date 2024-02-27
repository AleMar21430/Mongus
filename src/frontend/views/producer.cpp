#include "frontend/views/producer.h"

#include "main window.h"

#include "frontend/views/movie.h"
#include "frontend/views/actor.h"

Producer_Tab::Producer_Tab(App* i_app, const string& nombre) :
	QMainWindow(),
	app(i_app)
{
	connect(app->mongo_thread, &Mongo_Thread::result, [this](const Mongo_Query& query, const json& data) {
		if (query.type == Mongo_Type::PRODUCER && query.request_id == app->mongo_request)
			QMetaObject::invokeMethod(this, "process", Qt::QueuedConnection, Q_ARG(json, data));
		});

	app->mongo_request++;
	app->mongo_thread->cancelWork();
	Mongo_Query work = Mongo_Query({ {"nombre", nombre} }, Mongo_Type::PRODUCER, app->mongo_request);
	app->mongo_thread->queueWork(work);

	showMaximized();
}

void Producer_Tab::process(const json& data) {
	if (data.is_array() && !data.empty()) {
		json json_data = data[0];

		Linear_Contents* contents = new Linear_Contents();
		Label* name = new Label(json_data.contains("nombre") ? "Name: " + QString::fromStdString(json_data["nombre"]) : "Name: UNAVAILABLE");
		name->setFontSize(25);
		Label* country = new Label(json_data.contains("pais") ? "Country: " + QString::fromStdString(json_data["pais"]) : "Country: UNAVAILABLE");

		Widget_List* movie_list = new Widget_List("Producers");

		if (json_data["peliculas_detalle"].is_array()) {
			for (const auto& entry : json_data["peliculas_detalle"]) {
				Button* json_data_item = new Button(QString::fromStdString(entry["titulo"]));
				connect(json_data_item, &Button::clicked, [this, entry]() {
					Movie_Tab* movie = new Movie_Tab(app, entry["titulo"]);
				});
				movie_list->addWidget(json_data_item);
			}
		}

		contents->addWidget(name);
		contents->addWidget(country);
		contents->addWidget(movie_list);

		setCentralWidget(contents);
		setWindowTitle(json_data.contains("nombre") ? QString::fromStdString(json_data["nombre"]) : "UNAVAILABLE");
	}
}