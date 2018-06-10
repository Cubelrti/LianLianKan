#include "LianLianKan.h"

void LianLianKan::updateVersus(QString message) {
	ui.statusBar->showMessage(message);
	auto tokens = message.split('_');
	if (message.contains("GAME_START")) {
		ui.versusButton->setDisabled(false);
		isNetwork = true;
		auto mapVec = map.makeMap(tokens[2].toInt());
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
		ui.opponentState->setText(QString::fromUtf16(u"对手剩余：") + QString::number(remainBlocks));
	}
	if (message.contains("USERTYPE")) {
		netId = tokens[1].toInt();
	}
	if (message.contains("ACTION")) {
		int id = tokens[1].toInt();
		int remain = tokens[2].toInt();
		if (id != netId) {
			ui.opponentState->setText(QString::fromUtf16(u"对手剩余：") + tokens[2]);
		}
		if (remain == 0) {
			endGame();
		}
	}
	if (message.contains("FAIL")) {
		int id = tokens[1].toInt();
		if (id != netId) {
			QGraphicsPixmapItem *win = new QGraphicsPixmapItem(QPixmap(":/LianLianKan/Images/win.png"));
			win->setPos(175, 200);
			scene->addItem(win);
			lifeTimer.stop();
			isGameNow = false;
		}
	}
	if (message.contains("ITEM")) {
		int id = tokens[2].toInt();
		if (tokens[1] == "MIRROR" && id != netId) {
			MirrorGame();
		}
		if (tokens[1] == "TIME" && id != netId) {
			lifeTimer.setInterval(75);
		}
		if (tokens[1] == "BLIND" && id != netId) {
			BlindGame();
		}
		if (tokens[1] == "OBSTACLE" && id != netId) {
			ObstacleGame();
		}
		if (tokens[1] == "HAND" && id != netId) {
			HandGame();
		}
	}
}


void LianLianKan::versusGame() {
	ui.versusButton->setDisabled(true);
	if (websocket.state() == QAbstractSocket::SocketState::ConnectedState) {
		ui.opponentState->show();
		ui.opponentState->setText(QString::fromUtf16(u"对手搜索中..."));
		websocket.sendTextMessage("MATCH");
		ui.statusBar->showMessage("Now matching opponent...");
	}
	else {
		ui.statusBar->showMessage("Connection failed.");
	}
}
