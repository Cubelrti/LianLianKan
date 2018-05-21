#pragma once

#include <QObject>
#include <QGraphicsItem>
#include <QTimer>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <vector>


class BoomEffect : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT

public:
	BoomEffect(QObject *parent, int x, int y, std::vector<QPixmap> &pixmapSource);
	QTimer timer;
	int currentImage;
	~BoomEffect();
private:
	std::vector<QPixmap> & pixmap;
	private slots:
	void animate();
};
