#include "frontend/movie.h"

#include "main window.h"

#include "frontend/producer.h"
#include "frontend/actor.h"

Movie_Tab::Movie_Tab(App* i_app, QListWidgetItem* item) :
	QMainWindow(),
	app(i_app)
{
	Linear_Contents* contents = new Linear_Contents();

	json data = json::parse(item->data(500).toString().toStdString());

	Label* title_widget =  new Label(item->text());
	title_widget->setFontSize(25);
	Label* premiere =      new Label("Premiere Date: " + QString::fromStdString(data["anio_lanzamiento"].dump()));

	Label* duration =      new Label ("Duration: " + QString::fromStdString(data["duration"].dump()));
	Button* producer =     new Button("Producer: " + QString::fromStdString(data["producer"].dump()));
	Button* director =     new Button("Director: " + QString::fromStdString(data["director"].dump()));
	Label* rating =        new Label ("Rating: " + QString::fromStdString(data["rating"].dump()));
	Label* source =        new Label ("Source: " + QString::fromStdString(data["source"].dump()));
	Label* genre =         new Label ("Genre: " + QString::fromStdString(data["genre"].dump()));
	Label* budget =        new Label ("Budget: " + QString::fromStdString(data["budget"].dump()));
	Label* ww_box_office = new Label ("Box Office: " + QString::fromStdString(data["ww_box_office"].dump()));

	Widget_List* cast_list = new Widget_List("Cast Members");
	Label* cast =          new Label ("Cast: " + QString::fromStdString(data["cast"].dump()));
	cast_list->addWidget(cast);

	contents->addWidget(title_widget);
	contents->addWidget(premiere);
	contents->addWidget(duration);
	contents->addWidget(producer);
	contents->addWidget(director);
	contents->addWidget(rating);
	contents->addWidget(source);
	contents->addWidget(genre);
	contents->addWidget(budget);
	contents->addWidget(ww_box_office);
	contents->addWidget(cast_list);

	setCentralWidget(contents);
	setWindowTitle(item->text());
	showMaximized();
}