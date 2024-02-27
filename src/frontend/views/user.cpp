#include "frontend/views/user.h"

#include "main window.h"

#include "frontend/views/movie.h"

User_Tab::User_Tab(App* i_app, const string& nombre) :
	QMainWindow(),
	app(i_app)
{
	connect(app->mongo_thread, &Mongo_Thread::result, [this](const Mongo_Query& query, const json& data) {
		if (query.type == Mongo_Type::USER && query.request_id == app->mongo_request)
			QMetaObject::invokeMethod(this, "process", Qt::QueuedConnection, Q_ARG(json, data));
		});

	app->mongo_request++;
	app->mongo_thread->cancelWork();
	Mongo_Query work = Mongo_Query({ {"nombre", nombre} }, Mongo_Type::USER, app->mongo_request);
	app->mongo_thread->queueWork(work);

	showMaximized();
}

void User_Tab::process(const json& data) {
	if (data.is_array() && !data.empty()) {
		json json_data = data[0];

		Linear_Contents* contents = new Linear_Contents();

		Button* reload = new Button("Reload");
		connect(reload, &Button::clicked, [this, data]() {
			QMetaObject::invokeMethod(this, "process", Qt::QueuedConnection, Q_ARG(json, data));
		});

		Label* name = new Label(json_data.contains("nombre_usuario") ? "Name: " + QString::fromStdString(json_data["nombre_usuario"]) : "Name: UNAVAILABLE");
		name->setFontSize(25);
		Label* email = new Label(json_data.contains("correo") ? "EMail: " + QString::fromStdString(json_data["correo"]) : "EMail: UNAVAILABLE");
		Label* type = new Label(json_data.contains("tipo") ? "User Type: " + QString::fromStdString(json_data["tipo"]) : "User Type: UNAVAILABLE");

		contents->addWidget(reload);
		contents->addWidget(name);
		contents->addWidget(email);
		contents->addWidget(type);

		setCentralWidget(contents);
		setWindowTitle(json_data.contains("nombre_usuario") ? QString::fromStdString(json_data["nombre_usuario"]) : "UNAVAILABLE");
	}
}