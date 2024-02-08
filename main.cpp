#include "main window.h"

int main(int argc, char* argv[]) {
	QApplication::setAttribute(Qt::ApplicationAttribute::AA_NativeWindows); // Enables V-Sync
	QApplication::setAttribute(Qt::ApplicationAttribute::AA_UseDesktopOpenGL);

	App* application = new App(argc, argv);
	application->exec();

	application->cleanup();
	return 0;
}