#include "LianLianKan.h"

void LianLianKan::updateVersus(QString message) {
	ui.statusBar->showMessage(message);
	auto tokens = message.split('_');
	if (message.contains("GAME_START")) {
		ui.versusButton->setDisabled(false);
		ui.opponentState->setText(QString::fromUtf16(u"对手剩余：") + QString::number(remainBlocks));
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
		if (tokens[1] == "TIME" && id != netId) {
			lifeTimer.setInterval(75);
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
