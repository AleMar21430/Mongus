#include "frontend/lists/users.h"

#include "main window.h"
#include "frontend/views/user.h"

Users_Tab::Users_Tab(App* i_app) :
	Linear_Contents(),
	app(i_app)
{
	list = new List(true);
	layout->addWidget(list);
	connect(list, &List::itemDoubleClicked, [this](QListWidgetItem* item) {
		User_Tab* movie = new User_Tab(app, item->text().toStdString());
	});

	connect(app->mongo_thread, &Mongo_Thread::result, [this](const Mongo_Query& query, const json& data) {
		if (query.type == Mongo_Type::USERS && query.request_id == app->mongo_request)
			process(data);
	});
}

void Users_Tab::activate() {
	app->mongo_request++;
	app->mongo_thread->cancelWork();
	list->clear();
	Mongo_Query work = Mongo_Query({}, Mongo_Type::USERS, app->mongo_request);
	app->mongo_thread->queueWork(work);
}


void Users_Tab::process(const json& data) {
	const QIcon loading = QIcon("./res/loading.png");
	for (const auto& entry : data) {
		QListWidgetItem* item = new QListWidgetItem();
		item->setSizeHint(QSize(256, 300));
		item->setTextAlignment(Qt::AlignmentFlag::AlignHCenter | Qt::AlignmentFlag::AlignBottom);

		item->setText(QString::fromStdString(entry["nombre_usuario"]));

		item->setIcon(loading);
		list->addItem(item);
	}
}