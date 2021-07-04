
#include <QApplication>
#include "Qt5DigitalImageProcessing.h"
#include "ImageProcess.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	Qt5DigitalImageProcessing w;
	w.show();
	return a.exec();
}
