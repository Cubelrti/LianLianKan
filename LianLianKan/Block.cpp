#include "Block.h"

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

Block::Block(QObject *parent, int block_type, int x, int y)
	: QObject(parent), block_type(block_type), x(x), y(y)
{
	

	const int X_OFFSET = 6;
	const int Y_OFFSET = 155;
	const int X_PER_BLOCK = 30;
	const int Y_PER_BLOCK = 35;

	setPixmap(QPixmap(images[block_type / 4]).copy(rects[block_type % 4]));
	setPos(X_OFFSET + X_PER_BLOCK * x, Y_OFFSET + Y_PER_BLOCK * y);

}

void Block::select() {

		const QString mask = ":/LianLianKan/Sel.png";
		QPixmap pixmap = QPixmap(this->pixmap());
		QPainter painter(&pixmap);
		painter.drawPixmap(4, 0, QPixmap(mask));
		setPixmap(pixmap);
}

void Block::deselect() {
	setPixmap(QPixmap(images[block_type / 4]).copy(rects[block_type % 4]));
}


Block::~Block()
{
}
