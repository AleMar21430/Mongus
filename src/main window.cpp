#include "main window.h"

App::App(int argc, char* argv[]) :
	QApplication(argc, argv),
	log(new Text_Stream())
{
	loadSettings();
	loadStyle();
	load();
	window = new Main_Window(this);
}

void App::load() {
	async_request = 0;
	mongo_request = 0;
	mongo_thread = new Mongo_Thread(&settings);
	mongo_thread->start();
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

Main_Window::Main_Window(App* app) :
	app(app)
{
	Movie_Listings_Tab* movie_listings_tab = new Movie_Listings_Tab(app);
	Statistics_Tab* statistics_tab = new Statistics_Tab(app);
	Settings_Tab* settings_tab = new Settings_Tab(app);
	Showings_Tab* showings_tab = new Showings_Tab(app);

	tabs = new Tabs();
	tabs->addTab(movie_listings_tab, "Movie Listings");
	tabs->addTab(showings_tab, "Movie Showings");
	tabs->addTab(statistics_tab, "Statistics");
	tabs->addTab(settings_tab, "Settings");

	Splitter* splitter = new Splitter(true);
	splitter->addWidget(tabs);
	splitter->addWidget(app->log);
	splitter->setSizes({ 2000, 250 });

	connect(tabs, &Tabs::currentChanged, [this](int index) { changeView(index); });
	connect(app->mongo_thread, &Mongo_Thread::logMsg, this, &Main_Window::logMessage);

	setCentralWidget(splitter);
	showMaximized();
	movie_listings_tab->activate();
}

void Main_Window::changeView(const int& row) {
	if (row == 0) {
		dynamic_cast<Movie_Listings_Tab*>(tabs->currentWidget())->activate();
	}
}

void Main_Window::logMessage(const QString& message) {
	if (QThread::currentThread() != QApplication::instance()->thread()) {
		QMetaObject::invokeMethod(this, "logMessage", Qt::QueuedConnection, Q_ARG(QString, message));
		return;
	}
	app->log->append(message);
}