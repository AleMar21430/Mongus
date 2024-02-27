#include "frontend/views/showing.h"

#include "main window.h"

#include "frontend/views/movie.h"

Showing_Tab::Showing_Tab(App* i_app, const string& nombre) :
	QMainWindow(),
	app(i_app)
{
	connect(app->mongo_thread, &Mongo_Thread::result, [this](const Mongo_Query& query, const json& data) {
		if (query.type == Mongo_Type::SHOWING && query.request_id == app->mongo_request)
			QMetaObject::invokeMethod(this, "process", Qt::QueuedConnection, Q_ARG(json, data));
	});

	app->mongo_request++;
	app->mongo_thread->cancelWork();
	Mongo_Query work = Mongo_Query({ {"sala", nombre} }, Mongo_Type::SHOWING, app->mongo_request);
	app->mongo_thread->queueWork(work);

	showMaximized();
}

void Showing_Tab::process(const json& data) {
	if (data.is_array() && !data.empty()) {
		json json_data = data[0];
		cerr << json_data.dump();
		Linear_Contents* contents = new Linear_Contents();
		Label* name = new Label(json_data.contains("sala") ? "Showroom: " + QString::fromStdString(json_data["sala"]) : "Showroom: UNAVAILABLE");
		name->setFontSize(25);

		string date = "UNAVAILABLE";
		if (json_data.contains("fecha_proyeccion")) {
			chrono::milliseconds timestamp(json_data["fecha_proyeccion"]["$date"]);
			chrono::system_clock::time_point timePoint(timestamp);
			time_t time = chrono::system_clock::to_time_t(timePoint);
			tm* timeStruct = localtime(&time);
			char buffer[80];
			strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeStruct);
			date = buffer;
		}

		Label* show_time = new Label(QString::fromStdString("Showtime: " + date));
		Label* seats_sold = new Label(json_data.contains("asientos_vendidos") ? "Seats Sold: " + QString::fromStdString(json_data["asientos_vendidos"].dump()) : "Seats Sold: UNAVAILABLE");
		Label* amount_sold = new Label(json_data.contains("dolares_recaudados") ? "$ Sold: " + QString::fromStdString(json_data["dolares_recaudados"].dump()) : "$ Sold: UNAVAILABLE");

		contents->addWidget(name);
		contents->addWidget(show_time);
		contents->addWidget(seats_sold);
		contents->addWidget(amount_sold);

		if (json_data["pelicula_detalle"].is_array() && !json_data["pelicula_detalle"].empty()) {
			Button* movie = new Button("Movie: " + QString::fromStdString(json_data["pelicula_detalle"][0]["titulo"]));
			connect(movie, &Button::clicked, [this, json_data]() {
		
				Movie_Tab* movie = new Movie_Tab(app, json_data["pelicula_detalle"][0]["titulo"]);
			});
			contents->addWidget(movie);
		}
		setCentralWidget(contents);
	}
}