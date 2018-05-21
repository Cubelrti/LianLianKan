#pragma once

#include <QObject>
#include <QGraphicsItem>
#include <QTimer>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

enum DIRECTION
{
	UD,
	LR,
	LU,
	RU,
	LD,
	RD
};

class Lightning : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
	Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)

public:
	Lightning(QObject *parent, DIRECTION direction, int x, int y);
	~Lightning();
private:
	DIRECTION direction;
	void startAnimate();
	QTimer timer;
	QSequentialAnimationGroup animation;
	int state = 0;
	QPropertyAnimation *fade;
private slots:
	void animate();

};
