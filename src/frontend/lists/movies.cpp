#include "frontend/lists/movies.h"

#include "main window.h"
#include "frontend/views/movie.h"

Movies_Tab::Movies_Tab(App* i_app) :
	Linear_Contents(),
	app(i_app)
{
	list = new List(true);
	layout->addWidget(list);
	connect(list, &List::itemDoubleClicked, [this](QListWidgetItem* item) {
		Movie_Tab* movie = new Movie_Tab(app, item->text().toStdString());
	});

	connect(app->mongo_thread, &Mongo_Thread::result, [this](const Mongo_Query& query, const json& data) {
		if (query.type == Mongo_Type::MOVIES && query.request_id == app->mongo_request)
			process(data);
	});
}

void Movies_Tab::activate() {
	app->mongo_request++;
	app->mongo_thread->cancelWork();
	list->clear();
	Mongo_Query work = Mongo_Query({}, Mongo_Type::MOVIES, app->mongo_request);
	app->mongo_thread->queueWork(work);
}

void Movies_Tab::process(const json& data) {
	const QIcon loading = QIcon("./res/loading.png");
	for (const auto& entry : data) {
		QListWidgetItem* item = new QListWidgetItem();
		item->setSizeHint(QSize(256, 300));
		item->setTextAlignment(Qt::AlignmentFlag::AlignHCenter | Qt::AlignmentFlag::AlignBottom);

		item->setText(QString::fromStdString(entry["titulo"]));

		item->setIcon(loading);
		list->addItem(item);
	}
}