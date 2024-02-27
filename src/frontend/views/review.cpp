#include "frontend/views/review.h"

#include "main window.h"

Review_Tab::Review_Tab(App* i_app, const string& id) :
	QMainWindow(),
	app(i_app)
{
	Linear_Contents* contents = new Linear_Contents();
	Label* name = new Label("Add Review");
	Text_Edit* review = new Text_Edit();
	QComboBox* score = new QComboBox();
	Button* add_review = new Button("Submit Review");
	add_review->setStyleProp("QPushButton { background: rgb(50,150,50); } QPushButton:hover { background: rgb(70,180,70); }");
	connect(add_review, &Button::clicked, [this, review, score, id]() {
		process(review->toPlainText().toStdString(), score->currentText().toStdString(), "NOW", "Pekoyo", id);
	});
	score->addItems({ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" });

	contents->addWidget(name);
	contents->addWidget(review);
	contents->addWidget(score);
	contents->addWidget(add_review);

	setCentralWidget(contents);
	setWindowTitle("REVIEW");

	connect(app->mongo_thread, &Mongo_Thread::result, [this](const Mongo_Query& query, const json& data) {
		if (query.type == Mongo_Type::REVIEW_ADD && query.request_id == app->mongo_request)
			close();
	});
	showMaximized();
}

void Review_Tab::process(const string& review, const string& score, const string& date, const string& user, const string& id) {

	size_t start_pos = id.find("\"$oid\":\"");

	if (start_pos != std::string::npos) {
		start_pos += 8;
		size_t end_pos = id.find("\"", start_pos);

		if (end_pos != std::string::npos) {
			std::string extracted_value = id.substr(start_pos, end_pos - start_pos);

			cerr << endl << endl << review << endl << endl << score << endl << "NOW" << endl << "Pekoyo" << endl << endl << extracted_value << endl << endl;

			app->mongo_request++;
			app->mongo_thread->cancelWork();
			Mongo_Query work = Mongo_Query({
				{"resena", review},
				{"score", score},
				{"fecha", date},
				{"usuario", user},
				{"movie_id", extracted_value}
				}, Mongo_Type::REVIEW_ADD, app->mongo_request);
			app->mongo_thread->queueWork(work);
		}
	}
}