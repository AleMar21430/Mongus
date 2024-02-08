#include "main window.h"

App::App(int argc, char* argv[]) :
	QApplication(argc, argv),
	log(new Text_Stream())
{
	loadStyle();
	loadSettings();

	window = new Main_Window(this);
	init();
}

void App::init() {
	error_image = QPixmap("./res/Error.png");

	remote_requests = {
		{ Remote_Request_Type::UNKNOWN, 0 }
	};
	local_requests = {
		{ Local_Request_Type::UNKNOWN, 0 }
	};
	headers = cpr::Header {
	};

	mongo::instance inst = mongo::instance();
	try {
		mongo::client conn = mongo::client(mongo::uri(settings["db_url"].toStdString()));
		mongo::database db = conn["Proyecto_1"];
		mongo::collection collection = db["users"];
		mongo::cursor cursor = collection.find({});
		json resultArray;
		for (auto&& doc : cursor) {
			json docJson = json::parse(bsoncxx::to_json(doc));
			resultArray.push_back(docJson);
		}

		log->append(QString::fromStdString(resultArray.dump()));
	}
	catch (const std::exception& error) {
		log->append(QString::fromStdString("MongoDB Error: ") + error.what());
	}
}

void App::loadStyle() {
	QFile file = QFile("./res/Style.txt");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream stream(&file);
		QString style = stream.readAll();
		file.close();
		setStyleSheet(style);
	}
}

void App::loadSettings() {
	QString db_url = QSettings("Peko", "Database").value("db_url", "ERROR").toString();
	if (db_url == "ERROR") {
		QString key = QInputDialog::getText(nullptr, "Input Dialog", "Enter the database url:");
		QSettings("Peko", "Database").setValue("db_url", key);
		db_url = key;
	}
	QString username = QSettings("Peko", "Database").value("username", "ERROR").toString();
	if (username == "ERROR") {
		QString key = QInputDialog::getText(nullptr, "Input Dialog", "Enter your username:");
		QSettings("Peko", "Database").setValue("username", key);
		username = key;
	}
	QString password = QSettings("Peko", "Database").value("password", "ERROR").toString();
	if (password == "ERROR") {
		QString key = QInputDialog::getText(nullptr, "Input Dialog", "Enter your password:");
		QSettings("Peko", "Database").setValue("password", key);
		password = key;
	}

	settings["db_url"] = db_url;
	settings["username"] = username;
	settings["password"] = password;

	log->append("Initialized");
	//log->append(QString::fromStdString("Initialized with database settings: [ ") + settings["db_url"] + " ]");
	//log->append(QString::fromStdString("Using credentials: [ ") + settings["username"] + " | " + settings["password"] + " ]");
}

int App::cleanup() {
	QSettings("Peko", "Database").setValue("db_url", settings["db_url"]);
	QSettings("Peko", "Database").setValue("username", settings["username"]);
	QSettings("Peko", "Database").setValue("password", settings["password"]);
	cerr << "Exit";
	return 0;
}

Main_Window::Main_Window(App* i_app) :
	app(i_app)
{
	settings = new Settings(app);

	tabs = new Tabs();
	tabs->addTab(settings, "Settings");

	Splitter* splitter = new Splitter();
	splitter->addWidget(tabs);
	splitter->addWidget(app->log);
	splitter->setSizes({ 2000, 250 });

	setCentralWidget(splitter);
	showMaximized();
}

void Main_Window::logMessage(const QString& i_message) {
	if (QThread::currentThread() != QApplication::instance()->thread()) {
		QMetaObject::invokeMethod(this, "logMessage", Qt::QueuedConnection, Q_ARG(QString, i_message));
		return;
	}
	app->log->append(i_message);
}