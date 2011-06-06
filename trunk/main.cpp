#include <QtGui/QApplication>
#include "ruler.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Ruler w;
	w.show();
	return a.exec();
}
