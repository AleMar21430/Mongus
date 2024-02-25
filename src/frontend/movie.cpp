#include "frontend/movie.h"

#include "main window.h"

Movie_Tab::Movie_Tab(App* i_app, QListWidgetItem* item) :
	QMainWindow(),
	app(i_app)
{
	Linear_Contents* contents = new Linear_Contents();

	json data = json::parse(item->data(500).toString().toStdString());

	Label* title_widget =  new Label(item->text());
	Label* premiere =      new Label(QString::fromStdString(data["anio_lanzamiento"].dump()));
	//Label* duration =      new Label(QString::fromStdString(data["duration"].dump()));
	//Label* producer =      new Label(QString::fromStdString(data["producer"].dump()));
	//Label* director =      new Label(QString::fromStdString(data["director"].dump()));
	//Label* rating =        new Label(QString::fromStdString(data["rating"].dump()));
	//Label* source =        new Label(QString::fromStdString(data["source"].dump()));
	//Label* cast =          new Label(QString::fromStdString(data["cast"].dump()));
	//Label* genre =         new Label(QString::fromStdString(data["genre"].dump()));
	//Label* budget =        new Label(QString::fromStdString(data["budget"].dump()));
	//Label* ww_box_office = new Label(QString::fromStdString(data["ww_box_office"].dump()));

	contents->addWidget(title_widget);
	contents->addWidget(premiere);
	//contents->addWidget(duration);
	//contents->addWidget(producer);
	//contents->addWidget(director);
	//contents->addWidget(rating);
	//contents->addWidget(source);
	//contents->addWidget(cast);
	//contents->addWidget(genre);
	//contents->addWidget(budget);
	//contents->addWidget(ww_box_office);

	setCentralWidget(contents);
	setWindowTitle(item->text());
	showMaximized();
}