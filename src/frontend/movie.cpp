#include "frontend/movie.h"

#include "main window.h"

Movie_Tab::Movie_Tab(App* i_app, const QString& title) :
	QMainWindow(),
	app(i_app)
{
	Linear_Contents* contents = new Linear_Contents();
	setCentralWidget(contents);
	setWindowTitle(title);
	showMaximized();
}