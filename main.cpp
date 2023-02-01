#include <displayer.hpp>
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	Displayer dp;
	dp.show();

	return a.exec();
}