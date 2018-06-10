#include "LianLianKan.h"


void LianLianKan::navigateGame() {
	if (remainBlocks == 0) {
		return;
	}
	QSound::play("./Sounds/item.wav");
	auto navigator = map.prompt();
	auto lightnings = map.getDirections(navigator);
	if (navigator.size() == 2) {
		for (auto item : scene->items()) {
			auto *block = dynamic_cast<Block *>(item);
			if (block == nullptr) continue;
			if (block->x == navigator[0][1] && block->y == navigator[0][0]) {
				block->select();
			}
			if (block->x == navigator[1][1] && block->y == navigator[1][0]) {
				block->select();
			}
		}
	}
	drawLightning(navigator, lightnings);
	remainNavigators--;
	updateItems();
}


void LianLianKan::MirrorGame()
{
	QSound::play("./Sounds/item.wav");
	prev = nullptr;
	scene->clear();
	lightningSequence.clear();
	auto mapVec = map.mirror();
	for (int i = 17; i >= 0; i--)
	{
		for (int j = 0; j < 10; j++)
		{
			if (mapVec[j][i] == 0) continue;
			int block_type = mapVec[j][i] - 1;
			Block *_block = new Block(this, block_type, i, j, blockPixmaps[block_type]);
			scene->addItem(_block);
		}
	}
}

void LianLianKan::ObstacleGame()
{
	QSound::play("./Sounds/item.wav");
	prev = nullptr;
	scene->clear();
	lightningSequence.clear();
	auto mapVec = map.obstacle();
	for (int i = 17; i >= 0; i--)
	{
		for (int j = 0; j < 10; j++)
		{
			if (mapVec[j][i] == 0) continue;
			int block_type = mapVec[j][i] - 1;
			Block *_block = new Block(this, block_type, i, j, blockPixmaps[block_type]);
			scene->addItem(_block);
		}
	}
}

void LianLianKan::HandGame()
{
	QSound::play("./Sounds/item.wav");
	QGraphicsPixmapItem *ban = new QGraphicsPixmapItem(QPixmap(":/LianLianKan/Images/ban.png"));
	ban->setPos(50, 0);
	scene->addItem(ban);
	isGameNow = false;
	QTimer::singleShot(3000, this, [=] {
		isGameNow = true;
		if (ban == nullptr) return;
		scene->removeItem(ban);
	});
}

void LianLianKan::BlindGame()
{
	QSound::play("./Sounds/item.wav");
	QGraphicsRectItem *blind = new QGraphicsRectItem(0, 150, 600, 385);
	blind->setBrush(QBrush(Qt::black));
	blind->setOpacity(0.85);
	scene->addItem(blind);
	QTimer::singleShot(5000, this, [=] {
		if (blind == nullptr) return;
		scene->removeItem(blind);
	});
}

void LianLianKan::resortGame() {
	if (remainBlocks == 0) {
		return;
	}
	QSound::play("./Sounds/item.wav");
	prev = nullptr;
	scene->clear();
	lightningSequence.clear();
	auto mapVec = map.rearrange();
	for (int i = 17; i >= 0; i--)
	{
		for (int j = 0; j < 10; j++)
		{
			if (mapVec[j][i] == 0) continue;
			int block_type = mapVec[j][i] - 1;
			Block *_block = new Block(this, block_type, i, j, blockPixmaps[block_type]);
			scene->addItem(_block);
		}
	}
	remainResorts--;
	updateItems();
}