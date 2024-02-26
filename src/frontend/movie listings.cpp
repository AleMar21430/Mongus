#include "frontend/movie listings.h"

#include "main window.h"
#include "frontend/movie.h"

Movie_Listings_Tab::Movie_Listings_Tab(App* i_app) :
	Linear_Contents(),
	app(i_app)
{
	list = new List(true);
	layout->addWidget(list);
	connect(list->verticalScrollBar(), &QScrollBar::valueChanged, [this](int value) { loadThumbnails(); });
	connect(list, &List::itemDoubleClicked, [this](QListWidgetItem* item) {
		Movie_Tab* movie = new Movie_Tab(app, item->text().toStdString());
	});

	connect(app->mongo_thread, &Mongo_Thread::result, [this](const Mongo_Query& query, const json& data) {
		if (query.type == Mongo_Type::MOVIE_LISTINGS && query.request_id == app->mongo_request)
			process(data);
	});
}

void Movie_Listings_Tab::activate() {
	//app->async_request++;
	app->mongo_request++;
	app->mongo_thread->cancelWork();
	list->clear();
	Mongo_Query work = Mongo_Query({}, Mongo_Type::MOVIE_LISTINGS, app->mongo_request);
	app->mongo_thread->queueWork(work);
	QTimer::singleShot(1500, this, &Movie_Listings_Tab::loadThumbnails);
}

void Movie_Listings_Tab::loadThumbnails() {
	/*const int columns = static_cast<int>(floor(list->width() / 256));
	const QListWidgetItem* item_a = list->itemAt(QPoint(5, 5));
	const QListWidgetItem* item_b = list->itemAt(QPoint(5, list->height() - 10));

	int index_a = 0;
	int index_b = 0;

	if (item_a != nullptr)
		index_a = list->row(item_a);
	if (item_b != nullptr)
		index_b = list->row(item_b);

	if ((index_a - columns) < 0)
		index_a = 0;
	else
		index_a -= columns;

	if ((index_b + 2 * columns) >= list->count())
		index_b = list->count();
	else index_b += (2 * columns);

	//for (int i = 0; i < index_a; i++) {
	//	list->item(i)->setIcon(QIcon());
	//}
	const cpr::Header headers{{"user-agent", "User [Pekoyo]"}, {"Client-ID", " {{clientId}}"}};
	for (int i = index_a; i < index_b; i++) {
		Async_Thread* thread = new Async_Thread(Async_Query({ { "url", list->item(i)->data(100).toString().toStdString() }}, Async_Type::UNKNOWN, app->async_request, headers, i));
		app->async_threads.push_back(thread);

		connect(thread, &Async_Thread::iconResult, [this](const Async_Query& query, const QIcon& icon) {
			if (query.request_id == app->async_request && query.thread_id >= 0 && query.thread_id < list->count()) {
				list->item(query.thread_id)->setIcon(icon);
			}
			else {
				stringstream msg;
				msg << "Ignored thread [" << query.thread_id << "] with request id: [" << query.request_id << "] mismatch with current id: [" << app->async_request << "]";
				app->window->logMessage(QString::fromStdString(msg.str()));
			}
		});
		connect(thread, &Async_Thread::logMsg, app->window, &Main_Window::logMessage);
		connect(thread, &Async_Thread::finished, [this, thread]() {
			app->async_threads.removeOne(thread);
			thread->deleteLater();
		});
		thread->start();
	}
	
	//for (int i = index_b + 1; i < gallery_view->count(); i++) {
	//	gallery_view->item(i)->setIcon(QIcon());
	//}*/
}

void Movie_Listings_Tab::process(const json& data) {
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