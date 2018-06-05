#include "LianLianKan.h"

void LianLianKan::drawLightning(std::vector<std::vector<int>> seq, std::vector<int> lightningSeq) {
	lightningSequence.clear();
	QSound::play("./Sounds/Elec.wav");
	if (seq.size() == 0 || seq.size() == 2 || lightningSeq.size() == 0) return;
	int count = 1;
	for (int i = 0; i < lightningSeq.size(); i++) {
		lightningSequence.push_back(new Lightning(this, (DIRECTION)lightningSeq[i], seq[count][1], seq[count][0]));
		count++;
	}
	for (auto it : lightningSequence) scene->addItem(it);
}

void LianLianKan::drawBoom(Block * prev, Block * next) {
	auto boom_1 = new BoomEffect(this, prev->x, prev->y, boomPixmaps);
	auto boom_2 = new BoomEffect(this, next->x, next->y, boomPixmaps);
	scene->addItem(boom_1);
	scene->addItem(boom_2);
}
void LianLianKan::removeBoom(BoomEffect * effect) {
	scene->removeItem(effect);
	delete effect;
}