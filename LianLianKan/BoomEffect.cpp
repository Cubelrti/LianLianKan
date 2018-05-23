#include "BoomEffect.h"
#include "LianLianKan.h"


BoomEffect::BoomEffect(QObject *parent, int x, int y, std::vector<QPixmap> &pixmapSource)
	: QObject(parent), pixmap(pixmapSource)
{

	const int X_OFFSET = 21;
	const int Y_OFFSET = 162;
	const int X_PER_BLOCK = 30;
	const int Y_PER_BLOCK = 35;
	setPos(X_OFFSET + X_PER_BLOCK * x, Y_OFFSET + Y_PER_BLOCK * y);
	currentImage = 1;
	setPixmap(pixmap[currentImage]);
	timer.start(20);
	connect(&timer, SIGNAL(timeout()), this, SLOT(animate()));
}

BoomEffect::~BoomEffect()
{
}

void BoomEffect::animate()
{
	currentImage++;
	if (currentImage >= 50) {
		timer.stop();
		((LianLianKan*)parent())->removeBoom(this);
		return;
	}
	setPixmap(pixmap[currentImage]);
}
