#include "LianLianKan.h"

void LianLianKan::updateTimer() {
	int val = ui.timeBar->value();
	if (val == 100) {
		lifeTimer.stop();
		endGame();
	}

	ui.timeBar->setValue(val + 1);
	ui.timeBar->repaint();
}

void LianLianKan::endGame() {
	isGameNow = false;
	QSound::play("./Sounds/boom.wav");
	QSound::play("./Sounds/end.wav");
	player.stop();
	lifeTimer.stop();
	auto ex_boom = new BoomEffect(this, 1, -4, fireworkPixmaps);
	ex_boom->setScale(2);
	if (remainBlocks == 0) {
		ui.remainBlock->setText(QString::fromUtf16(u"ÄãÓ®ÁË£¡"));
		score += 10;
		updateUserInfo();
	}
	else {
		if( isNetwork ) websocket.sendTextMessage("FAIL_" + QString::number(netId));
		ui.remainBlock->setText(QString::fromUtf16(u"ÓÎÏ·½áÊø£¡"));
		score -= 10;
		QGraphicsPixmapItem *lose = new QGraphicsPixmapItem(QPixmap(":/LianLianKan/Images/lose.png"));
		lose->setPos(175, 200);
		scene->addItem(lose);
		updateUserInfo();
	}
	scene->addItem(ex_boom);
	remainBlocks = 0;
}

void LianLianKan::startGame() {
	ui.opponentState->hide();
	isNetwork = false;
	isGameNow = true;
	auto mapVec = map.makeMap(difficulty);
	QSound::play("./Sounds/Start.wav");
	prev = nullptr;
	remainNavigators = 3;
	remainResorts = 2;
	updateItems();
	scene->clear();
	lightningSequence.clear();
	drawBlocks(mapVec);
	ui.timeBar->setValue(0);
	ui.timeBar->setMaximum(100);
	lifeTimer.start(150);
	ui.remainBlock->setText(QString::number(remainBlocks));
	setBackgroundMusic();
}

void LianLianKan::pauseGame() {
	if (!isGameNow || isNetwork) return;
	if (lifeTimer.isActive()) {
		lifeTimer.stop();
	}
	else lifeTimer.start();
}