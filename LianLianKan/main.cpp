#include "LianLianKan.h"
#include "SignIn.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SignIn signin;
	signin.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
	signin.show();
	//w.show();
	return a.exec();
}
