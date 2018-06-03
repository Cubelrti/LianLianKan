#include "Lightning.h"

const QString images[6] = {
	":/LianLianKan/Images/Elec1.png",
	":/LianLianKan/Images/Elec2.png",
	":/LianLianKan/Images/Elec3.png",
	":/LianLianKan/Images/Elec4.png",
	":/LianLianKan/Images/Elec5.png",
	":/LianLianKan/Images/Elec6.png",
};

const QRect rects[2] = {
	QRect(0, 0, 35, 39),
	QRect(35, 0, 70, 39)
};

Lightning::Lightning(QObject *parent, DIRECTION direction, int x, int y)
	: QObject(parent), direction(direction)
{
	const int X_OFFSET = 31;
	const int Y_OFFSET = 175;
	const int X_PER_BLOCK = 30;
	const int Y_PER_BLOCK = 35;

	setPixmap(QPixmap(images[direction]).copy(rects[0]));
	setPos(X_OFFSET + X_PER_BLOCK * x, Y_OFFSET + Y_PER_BLOCK * y);

	timer.start(100);
	connect(&timer, SIGNAL(timeout()), this, SLOT(animate()));
	startAnimate();
}


Lightning::~Lightning()
{
	delete fade;
}

void Lightning::startAnimate()
{
	fade = new QPropertyAnimation(this, "opacity");
	fade->setDuration(1000);
	fade->setStartValue(1);
	fade->setEndValue(0);
	animation.addAnimation(fade);
	animation.start();
}

void Lightning::animate()
{
	if (state > 10) {
		timer.stop();
	}
	setPixmap(QPixmap(images[direction]).copy(rects[state % 2]));
	state++;
}
