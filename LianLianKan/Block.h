#pragma once

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>

const QString images[10] = {
	":/LianLianKan/Elem1.png",
	":/LianLianKan/Elem2.png",
	":/LianLianKan/Elem3.png",
	":/LianLianKan/Elem4.png",
	":/LianLianKan/Elem5.png",
	":/LianLianKan/Elem6.png",
	":/LianLianKan/Elem7.png",
	":/LianLianKan/Elem8.png",
	":/LianLianKan/Elem9.png",
	":/LianLianKan/Elem10.png"
};
const QRect rects[4] = {
	QRect(0, 0, 35, 39),
	QRect(35, 39, 70, 78),
	QRect(35, 0, 70, 39),
	QRect(0, 39, 35, 78)
};

class Block : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT

public:
	Block(QObject *parent, int block_type, int x, int y, QPixmap &pixmap);
	void select();
	void deselect();
	int x = 0;
	int y = 0;
	~Block();
protected:
	int block_type = 0;
};
