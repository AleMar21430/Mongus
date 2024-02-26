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
			process(data);
	});

	app->mongo_request++;
	app->mongo_thread->cancelWork();
	Mongo_Query work = Mongo_Query(item->data(500).toString().toStdString(), Mongo_Type::MOVIE_VIEW, app->mongo_request);
	app->mongo_thread->queueWork(work);

	showMaximized();
}

void Movie_Tab::process(const json& data) {
	Linear_Contents* contents = new Linear_Contents();

	Label* title_widget = new Label("Title: " + QString::fromStdString(data["title"].dump()));
	title_widget->setFontSize(25);
	Label* premiere = new Label("Premiere Date: " + QString::fromStdString(data["premiere"].dump()));
	Label* duration = new Label("Duration: " + QString::fromStdString(data["duration"].dump()));
	Button* producer = new Button("Producer: " + QString::fromStdString(data["producer"].dump()));
	Button* director = new Button("Director: " + QString::fromStdString(data["director"].dump()));
	Label* rating = new Label("Rating: " + QString::fromStdString(data["rating"].dump()));
	Label* genre = new Label("Genre: " + QString::fromStdString(data["genre"].dump()));
	Label* budget = new Label("Budget: " + QString::fromStdString(data["budget"].dump()));
	Label* ww_box_office = new Label("Box Office: " + QString::fromStdString(data["ww_box_office"].dump()));

	Widget_List* cast_list = new Widget_List("Cast Members");

	if (data["cast"].is_array()) {
		for (const auto& entry : data["cast"]) {
			Button* cast = new Button(QString::fromStdString(entry["name"].dump()));
			connect(cast, &Button::clicked, [this]() {

			});
			cast_list->addWidget(cast);
		}
	}

	contents->addWidget(title_widget);
	contents->addWidget(premiere);
	contents->addWidget(duration);
	contents->addWidget(producer);
	contents->addWidget(director);
	contents->addWidget(rating);
	contents->addWidget(genre);
	contents->addWidget(budget);
	contents->addWidget(ww_box_office);
	contents->addWidget(cast_list);

	setCentralWidget(contents);
	setWindowTitle(QString::fromStdString(data["title"].dump()));
}
