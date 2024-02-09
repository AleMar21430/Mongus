#include "main window.h"

int main(int argc, char* argv[]) {
	QApplication::setAttribute(Qt::ApplicationAttribute::AA_NativeWindows);
	QApplication::setAttribute(Qt::ApplicationAttribute::AA_UseDesktopOpenGL);

	mongo::instance mongo_driver = mongo::instance();
	App* application = new App(argc, argv);
	application->exec();
	application->cleanup();
}