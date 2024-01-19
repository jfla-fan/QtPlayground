#include "mainWindow.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BallsCollision w(BallsCollision::eCST_PerfectlyElastic);

	w.show();

	return a.exec();
}
