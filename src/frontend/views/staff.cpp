#include "frontend/views/staff.h"

#include "main window.h"

#include "frontend/views/producer.h"
#include "frontend/views/movie.h"

Staff_Tab::Staff_Tab(App* i_app, const string& nombre) :
	QMainWindow(),
	app(i_app)
{
	connect(app->mongo_thread, &Mongo_Thread::result, [this](const Mongo_Query& query, const json& data) {
		if (query.type == Mongo_Type::STAFF && query.request_id == app->mongo_request)
			QMetaObject::invokeMethod(this, "process", Qt::QueuedConnection, Q_ARG(json, data));
		});

	app->mongo_request++;
	app->mongo_thread->cancelWork();
	Mongo_Query work = Mongo_Query({ {"nombre", nombre} }, Mongo_Type::STAFF, app->mongo_request);
	app->mongo_thread->queueWork(work);

	showMaximized();
}

void Staff_Tab::process(const json& data) {
	if (data.is_array() && !data.empty()) {
		json json_data = data[0];

		Linear_Contents* contents = new Linear_Contents();

		Button* reload = new Button("Reload");
		connect(reload, &Button::clicked, [this, data]() {
			QMetaObject::invokeMethod(this, "process", Qt::QueuedConnection, Q_ARG(json, data));
		});

		Label* name = new Label(json_data.contains("nombre") ? "Name: " + QString::fromStdString(json_data["nombre"]) : "Name: UNAVAILABLE");
		name->setFontSize(25);
		Label* role = new Label(json_data.contains("cargo") ? "Role: " + QString::fromStdString(json_data["cargo"]) : "Role: UNAVAILABLE");

		contents->addWidget(reload);
		contents->addWidget(name);
		contents->addWidget(role);

		setCentralWidget(contents);
		setWindowTitle(json_data.contains("nombre") ? QString::fromStdString(json_data["nombre"]) : "UNAVAILABLE");
	}
}