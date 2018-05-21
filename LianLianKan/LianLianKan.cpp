#include "LianLianKan.h"

LianLianKan::LianLianKan(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	scene = new QGraphicsScene(this);
	ui.graphicsView->setScene(scene);
	ui.graphicsView->installEventFilter(this);
	scene->setSceneRect(0, 0, 791, 571);
	connect(ui.startButton, SIGNAL(clicked()), this, SLOT(startGame()));
	connect(ui.resortButton, SIGNAL(clicked()), this, SLOT(resortGame()));
	connect(ui.navigateButton, SIGNAL(clicked()), this, SLOT(navigate()));
	// resource initialization
	for (int i = 1; i <= 50; i++)
	{
		boomPixmaps.push_back(QPixmap(":/Boom/BoomEffect/explosion_" + QString::number(i) + ".png").scaled(60, 65));
	}
}

void LianLianKan::drawBlocks() {
	auto mapVec = map.makeMap();
	for (int i = 17; i >= 0; i--)
	{
		for (int j = 0; j < 10; j++)
		{
			if (mapVec[j][i] == 0) continue;
			Block *_block = new Block(this, mapVec[j][i] - 1, i, j);
			scene->addItem(_block);
		}
	}

}

void LianLianKan::startGame() {

	prev = nullptr;
	scene->clear();
	lightningSequence.clear();
	drawBlocks();
}

void LianLianKan::resortGame() {
	prev = nullptr;
	scene->clear();
	lightningSequence.clear();
	auto mapVec = map.rearrange();
	for (int i = 17; i >= 0; i--)
	{
		for (int j = 0; j < 10; j++)
		{
			if (mapVec[j][i] == 0) continue;
			Block *_block = new Block(this, mapVec[j][i] - 1, i, j);
			scene->addItem(_block);
		}
	}
}

void LianLianKan::navigate() {
	auto navigator = map.prompt();
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
	drawLightning(navigator);
}

void LianLianKan::drawLightning(std::vector<std::vector<int>> seq) {
	if (seq.size() == 0) return;
	lightningSequence.clear();
	int prevx = seq[0][1], prevy = seq[0][0];
	int u, v;
	DIRECTION prev;
	int prev_delta = 0;
	// prev maintaince
	if (prevx == seq[1][1]) {
		prev = UD;
		prev_delta = prevy - seq[1][0];
	}
	if (prevy == seq[1][0]) {
		prev = LR;
		prev_delta = prevx - seq[1][1];
	}
	if (seq.size() == 2) return;
	for (int i = 1; i < seq.size() - 1; i++)
	{
		int currx = seq[i][1], curry = seq[i][0];
		int nextx = seq[i + 1][1], nexty = seq[i + 1][0];
		// DETERMINE BY PREV1 AND NEXT1
		if (prevx == nextx) {
			prev = UD;
			prev_delta = curry - nexty;
			lightningSequence.push_back(new Lightning(this, UD, currx, curry));
		}
		else if (prevy == nexty) {
			prev = LR;
			prev_delta = currx - nextx;
			lightningSequence.push_back(new Lightning(this, LR, currx, curry));
		}
		else if (prevx < nextx && prevy > nexty || prevx > nextx && prevy < nexty) {
			// ����������
			if (prev == UD) {
				if (prev_delta == 1) {
					lightningSequence.push_back(new Lightning(this, LU, currx, curry));
					prev = LU;
				}
				else {
					lightningSequence.push_back(new Lightning(this, RD, currx, curry));
					prev = RD;
				}
			}
			else if(prev == LR) {
				if (prev_delta == 1) {
					lightningSequence.push_back(new Lightning(this, LU, currx, curry));
					prev = LU;
				}
				else {
					lightningSequence.push_back(new Lightning(this, RD, currx, curry));
					prev = RD;
				}
			}
			else if (prev == RD){

				lightningSequence.push_back(new Lightning(this, LU, currx, curry));
			}
			else if (prev == RU) {
				if (curry < nexty) {
					lightningSequence.push_back(new Lightning(this, LU, currx, curry));
				} else
				lightningSequence.push_back(new Lightning(this, RD, currx, curry));
			}
			else if (prev == LD) {
				if(nexty < curry)
					lightningSequence.push_back(new Lightning(this, RD, currx, curry));
				else if (nextx > currx) {
					lightningSequence.push_back(new Lightning(this, LU, currx, curry));
				}
				else {
					lightningSequence.push_back(new Lightning(this, RU, currx, curry));
				}
			}
			else if (prev == LU) {
				lightningSequence.push_back(new Lightning(this, RD, currx, curry));
			}
		}
		else if (prevx > nextx && prevy > nexty || prevx < nextx && prevy < nexty) {
			// ����������
			if (prev == UD) {
				if (prev_delta == 1) {
					lightningSequence.push_back(new Lightning(this, RU, currx, curry));
					prev = RU;
				}
				else {
					lightningSequence.push_back(new Lightning(this, LD, currx, curry));
					prev = LD;
				}
			}
			else if (prev == LR) {
				if (prev_delta == 1) {
					lightningSequence.push_back(new Lightning(this, LD, currx, curry));
					prev = LD;
				}
				else {
					lightningSequence.push_back(new Lightning(this, RU, currx, curry));
					prev = RU;
				}
			}
			else if (prev == LD) {
				lightningSequence.push_back(new Lightning(this, RU, currx, curry));
			}
			else if (prev == RU) {
				if(nextx > currx)
					lightningSequence.push_back(new Lightning(this, LD, currx, curry));
				else if (nexty < curry) {
					lightningSequence.push_back(new Lightning(this, LD, currx, curry));
				} else
					lightningSequence.push_back(new Lightning(this, RD, currx, curry));
			}
			else if (prev == RD) {
				if (nexty < curry)
					lightningSequence.push_back(new Lightning(this, RD, currx, curry));
				else
					lightningSequence.push_back(new Lightning(this, RU, currx, curry));
			}
			else if (prev == LU) {
				if (nexty > curry) {
					lightningSequence.push_back(new Lightning(this, RU, currx, curry));
				} else
				lightningSequence.push_back(new Lightning(this, LD, currx, curry));
			}
		}
		prevx = currx;
		prevy = curry;
	}


	//lightningSequence.push_back(new Lightning(this, static_cast<DIRECTION>(rand() % 6), prev->x, prev->y));
	//lightningSequence.push_back(new Lightning(this, static_cast<DIRECTION>(rand() % 6), next->x, next->y));
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

void LianLianKan::linking(Block * next) {
	if (prev) {
		auto directionVec = map.connection(prev->y, prev->x, next->y, next->x);
		if (directionVec.size() == 0) {
			prev->deselect();
			next->select();
			prev = next;
			return;
		}

		prev->deselect();
		scene->removeItem(prev);
		scene->removeItem(next);
		for (auto it : lightningSequence) {
			scene->removeItem(it);
			delete it;
		}
		drawBoom(prev, next);
		drawLightning(directionVec);
		if (prev == next)
		{
			delete prev;
			prev = nullptr;
			return;
		}
		delete prev;
		delete next;
		prev = nullptr;
		return;
	}
	next->select();
	prev = next;
}


bool LianLianKan::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::MouseButtonPress) {
		QMouseEvent *e = static_cast<QMouseEvent *>(event);
		if (auto *item = dynamic_cast<Block *>(ui.graphicsView->itemAt(e->pos()))) {
			if (item == nullptr) {
				return false;
			}
			linking(item);
			return true;
		}
		else {
			return false;
		}

		return false;
	}
	return false;
}
