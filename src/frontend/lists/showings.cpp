#include "frontend/lists/showings.h"

#include "main window.h"
#include "frontend/views/showing.h"

Showings_Tab::Showings_Tab(App* i_app) :
	Linear_Contents(),
	app(i_app)
{
	list = new List(true);
	layout->addWidget(list);
	connect(list, &List::itemDoubleClicked, [this](QListWidgetItem* item) {
		Movie_Tab* movie = new Movie_Tab(app, item->text().toStdString());
	});

	connect(app->mongo_thread, &Mongo_Thread::result, [this](const Mongo_Query& query, const json& data) {
		if (query.type == Mongo_Type::SHOWINGS && query.request_id == app->mongo_request)
			process(data);
	});
}

void Showings_Tab::activate() {
	app->mongo_request++;
	app->mongo_thread->cancelWork();
	list->clear();
	Mongo_Query work = Mongo_Query({}, Mongo_Type::SHOWINGS, app->mongo_request);
	app->mongo_thread->queueWork(work);
}


void Showings_Tab::process(const json& data) {
	const QIcon loading = QIcon("./res/loading.png");
	for (const auto& entry : data) {
		QListWidgetItem* item = new QListWidgetItem();
		item->setSizeHint(QSize(256, 300));
		item->setTextAlignment(Qt::AlignmentFlag::AlignHCenter | Qt::AlignmentFlag::AlignBottom);

		chrono::milliseconds timestamp(entry["fecha_proyeccion"]["$date"]);
		chrono::system_clock::time_point timePoint(timestamp);
		time_t time = chrono::system_clock::to_time_t(timePoint);
		tm* timeStruct = localtime(&time);
		char buffer[80];
		strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeStruct);

		item->setText(QString::fromStdString(entry["sala"])  + " | " + QString::fromStdString(buffer));

		item->setIcon(loading);
		list->addItem(item);
	}
}