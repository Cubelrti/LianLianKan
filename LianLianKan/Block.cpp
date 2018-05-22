#include "Block.h"

Block::Block(QObject *parent, int block_type, int x, int y, QPixmap &pixmap)
	: QObject(parent), block_type(block_type), x(x), y(y)
{
	

	const int X_OFFSET = 27; //6
	const int Y_OFFSET = 175; //155
	const int X_PER_BLOCK = 30;
	const int Y_PER_BLOCK = 35;

	setPixmap(pixmap);
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
