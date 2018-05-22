#include "LianLianKan.h"

LianLianKan::LianLianKan(QWidget *parent)
	: QMainWindow(parent)
{
	setFixedSize(791, 571);
	// ui'cing
	ui.setupUi(this);
	scene = new QGraphicsScene(this);
	ui.graphicsView->setScene(scene);
	ui.graphicsView->installEventFilter(this);
	scene->setSceneRect(0, 0, 791, 571);
	// buttons
	connect(ui.startButton, SIGNAL(clicked()), this, SLOT(startGame()));
	connect(ui.resortButton, SIGNAL(clicked()), this, SLOT(resortGame()));
	connect(ui.navigateButton, SIGNAL(clicked()), this, SLOT(navigate()));
	connect(ui.pauseButton, SIGNAL(clicked()), this, SLOT(pauseGame()));
	// resource initialization
	for (int i = 1; i <= 50; i++)
	{
		boomPixmaps.push_back(QPixmap(":/Boom/BoomEffect/explosion_" + QString::number(i) + ".png").scaled(60, 65));
	}
	for (int i = 1; i <= 50; i++)
	{
		fireworkPixmaps.push_back(QPixmap(":/Boom/BoomEffect/explosion_" + QString::number(i) + ".png"));
	}
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			blockPixmaps.push_back(QPixmap(images[i]).copy(rects[j]));
		}
	}
	// timers
	connect(&lifeTimer, SIGNAL(timeout()), this, SLOT(updateTimer()));
	// user init
	username = QString::fromUtf16(u"游客");
	score = 0;
	updateUserInfo();
}

void LianLianKan::updateTimer() {
	int val = ui.timeBar->value();
	if (val == 100) {
		lifeTimer.stop();
		endGame();
	}

	ui.timeBar->setValue(val + 1);
	ui.timeBar->repaint();
}

void LianLianKan::pauseGame() {
	if (lifeTimer.isActive()) {
		lifeTimer.stop();
	}
	else lifeTimer.start();
}

void LianLianKan::updateUserInfo() {
	QString user_info = QString::fromUtf16(u"昵称：") + username 
		+ "\n" + QString::fromUtf16(u"分数：") + QString::number(score);
	ui.label->setText(user_info);
}

void LianLianKan::drawBlocks() {
	auto mapVec = map.makeMap(difficult);
	int block_count = 0;
	for (int i = 17; i >= 0; i--)
	{
		for (int j = 0; j < 10; j++)
		{
			if (mapVec[j][i] == 0) continue;
			int block_type = mapVec[j][i] - 1;
			Block *_block = new Block(this, block_type, i, j, blockPixmaps[block_type]);
			scene->addItem(_block);
			block_count++;
		}
	}
	remainBlocks = block_count;
}

void LianLianKan::endGame() {
	QSound::play("./Sounds/boom.wav");
	QSound::play("./Sounds/end.wav");
	player.stop();
	lifeTimer.stop();
	auto ex_boom = new BoomEffect(this, 1, -4, fireworkPixmaps);
	ex_boom->setScale(2);
	if (remainBlocks == 0) {
		ui.remainBlock->setText(QString::fromUtf16(u"你赢了！"));
		score += 10;
		updateUserInfo();
	}
	else {
		ui.remainBlock->setText(QString::fromUtf16(u"游戏结束！"));		
		score -= 10;
		updateUserInfo();
	}
	scene->addItem(ex_boom);
	remainBlocks = 0;
}

void LianLianKan::startGame() {
	QSound::play("./Sounds/Start.wav");
	prev = nullptr;
	scene->clear();
	lightningSequence.clear();
	drawBlocks();
	ui.timeBar->setValue(0);
	ui.timeBar->setMaximum(100);
	lifeTimer.start(100);
	ui.remainBlock->setText(QString::number(remainBlocks));
	player.setMedia(QUrl::fromLocalFile("./Sounds/BGM.mp3"));
	player.setVolume(50);
	player.play();
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
}

void LianLianKan::navigate() {
	if (remainBlocks == 0) {
		return;
	}
	QSound::play("./Sounds/item.wav");
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
	QSound::play("./Sounds/Elec.wav");
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
			// 左下右上型
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
			else if (prev == LR) {
				if (prev_delta == 1) {
					lightningSequence.push_back(new Lightning(this, LU, currx, curry));
					prev = LU;
				}
				else {
					lightningSequence.push_back(new Lightning(this, RD, currx, curry));
					prev = RD;
				}
			}
			else if (prev == RD) {

				lightningSequence.push_back(new Lightning(this, LU, currx, curry));
			}
			else if (prev == RU) {
				if (curry < nexty) {
					lightningSequence.push_back(new Lightning(this, LU, currx, curry));
				}
				else
					lightningSequence.push_back(new Lightning(this, RD, currx, curry));
			}
			else if (prev == LD) {
				if (nexty < curry)
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
			// 左上右下型
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
				if (nextx > currx)
					lightningSequence.push_back(new Lightning(this, LD, currx, curry));
				else if (nexty < curry) {
					lightningSequence.push_back(new Lightning(this, LD, currx, curry));
				}
				else
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
				}
				else
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
	if (remainBlocks == 0) {
		return;
	}
	if (prev) {
		auto directionVec = map.connection(prev->y, prev->x, next->y, next->x);
		if (directionVec.size() == 0) {
			prev->deselect();
			next->select();
			prev = next;
			return;
		}
		remainBlocks -= 2;
		ui.timeBar->setValue(0);
		ui.remainBlock->setText(QString::number(remainBlocks));
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
		if (remainBlocks == 0) {
			endGame();
		}
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
			QSound::play("./Sounds/Sel.wav");
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
