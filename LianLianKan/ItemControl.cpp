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