#pragma once

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>


class Block : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT

public:
	Block(QObject *parent, int block_type, int x, int y);
	void select();
	void deselect();
	int x = 0;
	int y = 0;
	~Block();
protected:
	int block_type = 0;
};
