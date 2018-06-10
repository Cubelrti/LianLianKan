#pragma once

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>

const QString images[11] = {
	":/LianLianKan/Images/Elem1.png",
	":/LianLianKan/Images/Elem2.png",
	":/LianLianKan/Images/Elem3.png",
	":/LianLianKan/Images/Elem4.png",
	":/LianLianKan/Images/Elem5.png",
	":/LianLianKan/Images/Elem6.png",
	":/LianLianKan/Images/Elem7.png",
	":/LianLianKan/Images/Elem8.png",
	":/LianLianKan/Images/Elem9.png",
	":/LianLianKan/Images/Elem10.png",
	":/LianLianKan/Images/Elem11.png"
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
	int block_type = 0;
	~Block();
protected:
};
