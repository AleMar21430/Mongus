#include "frontend/movie.h"

#include "main window.h"

#include "frontend/producer.h"
#include "frontend/actor.h"

Movie_Tab::Movie_Tab(App* i_app, QListWidgetItem* item) :
	QMainWindow(),
	app(i_app)
{

	connect(app->mongo_thread, &Mongo_Thread::result, [this](const Mongo_Query& query, const json& data) {
		if (query.type == Mongo_Type::MOVIE_VIEW && query.request_id == app->mongo_request)
			QMetaObject::invokeMethod(this, "process", Qt::QueuedConnection, Q_ARG(json, data));
	});

	app->mongo_request++;
	app->mongo_thread->cancelWork();
	Mongo_Query work = Mongo_Query({ {"titulo", item->text().toStdString()}}, Mongo_Type::MOVIE_VIEW, app->mongo_request);
	app->mongo_thread->queueWork(work);

	showMaximized();
}

void Movie_Tab::process(const json& data) {
	if (data.is_array() && !data.empty()) {
		json json_data = data[0];

		cerr << endl << endl << json_data.dump() << endl << endl;

		Linear_Contents* contents = new Linear_Contents();
		Label* title_widget = new Label(json_data.contains("titulo") ? "Title: " + QString::fromStdString(json_data["titulo"]) : "Title: Unknown");
		title_widget->setFontSize(25);
		Label* premiere = new Label(json_data.contains("anio_lanzamiento") ? "Premiere Date: " + QString::fromStdString(json_data["anio_lanzamiento"]) : "Premiere Date: Unknown");

		Label* duration = new Label(json_data.contains("duratcon") ? "Duration: " + QString::fromStdString(json_data["duracion"]) : "Duration: Unknown");
		Button* director = new Button(json_data.contains("director") ? "Director: " + QString::fromStdString(json_data["director"]) : "Director: Unknown");
		Label* rating = new Label(json_data.contains("score") ? "Score: " + QString::fromStdString(json_data["score"]) : "Score: Unknown");

		Widget_List* producer_list = new Widget_List("Producers");
		Widget_List* genre_list = new Widget_List("Genres");
		Widget_List* award_list = new Widget_List("Awards");
		Widget_List* staff_list = new Widget_List("Staff Members");
		Widget_List* cast_list = new Widget_List("Cast Members");
		Widget_List* review_list = new Widget_List("Reviews");

		if (json_data["casa_productora_detalle"].is_array()) {
			for (const auto& entry : json_data["casa_productora_detalle"]) {
				Button* json_data_item = new Button(QString::fromStdString(entry["nombre"]));
				connect(json_data_item, &Button::clicked, [this]() {

					});
				producer_list->addWidget(json_data_item);
			}
		}
		if (json_data["genero_detalle"].is_array()) {
			for (const auto& entry : json_data["genero_detalle"]) {
				Button* json_data_item = new Button(QString::fromStdString(entry["nombre_genero"]));
				connect(json_data_item, &Button::clicked, [this]() {

					});
				genre_list->addWidget(json_data_item);
			}
		}
		if (json_data["premios_detalle"].is_array()) {
			for (const auto& entry : json_data["premios_detalle"]) {
				Button* json_data_item = new Button(QString::fromStdString(entry["nombre_premio"]));
				connect(json_data_item, &Button::clicked, [this]() {

					});
				award_list->addWidget(json_data_item);
			}
		}
		if (json_data["staff_produccion_detalle"].is_array()) {
			for (const auto& entry : json_data["staff_produccion_detalle"]) {
				Button* json_data_item = new Button(QString::fromStdString(entry["nombre"]));
				connect(json_data_item, &Button::clicked, [this]() {

					});
				staff_list->addWidget(json_data_item);
			}
		}
		if (json_data["actores_detalle"].is_array()) {
			for (const auto& entry : json_data["actores_detalle"]) {
				Button* json_data_item = new Button(QString::fromStdString(entry["nombre"]));
				connect(json_data_item, &Button::clicked, [this]() {

					});
				cast_list->addWidget(json_data_item);
			}
		}
		if (json_data["resenas_detalle"].is_array()) {
			for (const auto& entry : json_data["resenas_detalle"]) {
				Label* json_data_item = new Label(QString::fromStdString(entry["comentario"]));
				review_list->addWidget(json_data_item);
			}
		}

		contents->addWidget(title_widget);
		contents->addWidget(premiere);
		contents->addWidget(duration);
		contents->addWidget(director);
		contents->addWidget(rating);
		contents->addWidget(producer_list);
		contents->addWidget(genre_list);
		contents->addWidget(award_list);
		contents->addWidget(staff_list);
		contents->addWidget(cast_list);
		contents->addWidget(review_list);

		setCentralWidget(contents);
		setWindowTitle(json_data.contains("titulo") ? QString::fromStdString(json_data["titulo"]) : "Unknown");
	}
}