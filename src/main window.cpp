#include "main window.h"

App::App(int argc, char* argv[]) :
	QApplication(argc, argv),
	log(new Text_Stream())
{
	loadStyle();
	loadSettings();
	mongo_thread = new Mongo_Thread(&settings);
	window = new Main_Window(this);
	load();
}

void App::load() {
	error_image = QPixmap("./res/Error.png");
	async_requests = {
		{ Async_Type::UNKNOWN, 0 }
	};
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
	Movie_Listings_Tab* movie_listings_tab = new Movie_Listings_Tab(app);
	Statistics_Tab* statistics_tab = new Statistics_Tab(app);
	Settings_Tab* settings_tab = new Settings_Tab(app);
	Showings_Tab* showings_tab = new Showings_Tab(app);
	Studio_Tab* studio_tab = new Studio_Tab(app);
	Movie_Tab* movie_tab = new Movie_Tab(app);
	User_Tab* user_tab = new User_Tab(app);

	tabs = new Tabs();
	tabs->addTab(movie_listings_tab, "Movie Listings");
	tabs->addTab(showings_tab, "Movie Showings");
	tabs->addTab(settings_tab, "Settings");

	Splitter* splitter = new Splitter(true);
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