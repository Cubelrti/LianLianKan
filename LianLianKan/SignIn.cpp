#include "SignIn.h"
#include "LianLianKan.h"

SignIn::SignIn(QWidget *parent)
	: QWidget(parent)
{
	QMovie *movie = new QMovie(":/LianLianKan/Images/glow.gif");
	QLabel *background = new QLabel(this);
	background->setMovie(movie);
	movie->start();
	ui.setupUi(this);
	connect(ui.quitButton, SIGNAL(clicked()), SLOT(quitGame()));
	connect(ui.startButton, SIGNAL(clicked()), SLOT(startGame()));
}

void SignIn::quitGame() {
	this->close();
}

void SignIn::startGame() {
	LianLianKan *w = new LianLianKan();
	w->username = ui.username->text();
	w->show();
	w->updateUserInfo();
	this->hide();
}

void SignIn::mousePressEvent(QMouseEvent *event) {
	mpos = event->pos();
}

void SignIn::mouseMoveEvent(QMouseEvent *event) {
	if (event->buttons() & Qt::LeftButton) {
		QPoint diff = event->pos() - mpos;
		QPoint newpos = this->pos() + diff;

		this->move(newpos);
	}
}


SignIn::~SignIn()
{
}

