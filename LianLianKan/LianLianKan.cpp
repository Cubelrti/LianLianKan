#include "LianLianKan.h"

LianLianKan::LianLianKan(QWidget *parent)
	: QMainWindow(parent)
{
	// ui'cing
	setFixedSize(790, 598);
	ui.setupUi(this);
	scene = new QGraphicsScene(this);
	ui.graphicsView->setScene(scene);
	ui.graphicsView->installEventFilter(this);
	scene->setSceneRect(0, 0, 791, 571);
	ui.opponentState->hide();
	// buttons
	connect(ui.startButton, SIGNAL(clicked()), this, SLOT(startGame()));
	connect(ui.resortButton, SIGNAL(clicked()), this, SLOT(resortGame()));
	connect(ui.navigateButton, SIGNAL(clicked()), this, SLOT(navigateGame()));
	connect(ui.pauseButton, SIGNAL(clicked()), this, SLOT(pauseGame()));
	connect(ui.easyButton, SIGNAL(clicked()), this, SLOT(selectEasyMode()));
	connect(ui.normalButton, SIGNAL(clicked()), this, SLOT(selectNormalMode()));
	connect(ui.hardButton, SIGNAL(clicked()), this, SLOT(selectHardMode()));
	connect(ui.versusButton, SIGNAL(clicked()), this, SLOT(versusGame()));
	connect(&websocket, &QWebSocket::textMessageReceived, this, &LianLianKan::updateVersus);
	connect(&websocket, &QWebSocket::connected, this, [&] {
		ui.statusBar->showMessage("Connected to server.");
	});

	loadResources();
	// timers
	connect(&lifeTimer, SIGNAL(timeout()), this, SLOT(updateTimer()));
	score = 0;
	websocket.open(QUrl("ws://47.95.3.113:8888"));
}



void LianLianKan::resetDiffStyle() {
	ui.easyButton->setStyleSheet(R"(
		QPushButton { background: transparent; border: none;} 
		QPushButton:pressed{
			background: transparent;
		})");
	ui.normalButton->setStyleSheet(R"(
		QPushButton { background: transparent; border: none;} 
		QPushButton:pressed{
			background: transparent;
		})");
	ui.hardButton->setStyleSheet(R"(
		QPushButton { background: transparent; border: none;} 
		QPushButton:pressed{
			background: transparent;
		})");
}

void LianLianKan::selectEasyMode() {
	resetDiffStyle();
	difficulty = easy;
	ui.easyButton->setStyleSheet(R"(
		QPushButton { background-image: url(:/LianLianKan/Images/blue.png); border: none;} 
		QPushButton:pressed{
			background: transparent;
		})");

}

void LianLianKan::selectNormalMode() {
	resetDiffStyle();
	difficulty = normal;
	ui.normalButton->setStyleSheet(R"(
		QPushButton { background-image: url(:/LianLianKan/Images/yellow.png); border: none;} 
		QPushButton:pressed{
			background: transparent;
		})");

}

void LianLianKan::selectHardMode() {
	resetDiffStyle();
	difficulty = hard;
	ui.hardButton->setStyleSheet(R"(
		QPushButton { background-image: url(:/LianLianKan/Images/red.png); border: none;} 
		QPushButton:pressed{
			background: transparent;
		})");

}

void LianLianKan::updateVersus(QString message) {
	ui.statusBar->showMessage(message);
	auto tokens = message.split('_');
	if (message.contains("GAME_START")) {
		ui.opponentState->setText(QString::fromUtf16(u"对手剩余：")+QString::number(remainBlocks));
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
			ui.opponentState->setText(QString::fromUtf16(u"对手剩余：" )+ tokens[2]);
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

void LianLianKan::loadResources() {
	// resource initialization
	ui.statusBar->showMessage("Loading explosions...");
	for (int i = 1; i <= 50; i++)
	{
		boomPixmaps.push_back(QPixmap(":/Boom/BoomEffect/explosion_" + QString::number(i) + ".png").scaled(60, 60));
	}
	for (int i = 1; i <= 50; i++)
	{
		fireworkPixmaps.push_back(QPixmap(":/Boom/BoomEffect/explosion_" + QString::number(i) + ".png"));
	}
	ui.statusBar->showMessage("Loading blocks...");
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			blockPixmaps.push_back(QPixmap(images[i]).copy(rects[j]));
		}
	}
	ui.statusBar->showMessage("Resources initialized.");
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
	if (!isGameNow) return;
	if (lifeTimer.isActive()) {
		lifeTimer.stop();
	}
	else lifeTimer.start();
}

void LianLianKan::updateUserInfo() {
	QString user_info = QString::fromUtf16(u"昵称：") + username
		+ "\n" + QString::fromUtf16(u"分数：") + QString::number(score);
	ui.userInfo->setText(user_info);
}

void LianLianKan::drawBlocks(vector<vector<int>> mapVec) {
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
	isGameNow = false;
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
		websocket.sendTextMessage("FAIL_" + QString::number(netId));
		ui.remainBlock->setText(QString::fromUtf16(u"游戏结束！"));
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

void LianLianKan::setBackgroundMusic()
{
	QMediaPlaylist *playlist = new QMediaPlaylist();
	playlist->addMedia(QUrl::fromLocalFile("./Sounds/BGM.mp3"));
	playlist->setPlaybackMode(QMediaPlaylist::Loop);
	player.setPlaylist(playlist);
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
	remainResorts--;
	updateItems();
}

void LianLianKan::updateItems()
{
	if (remainResorts == 0) {
		ui.resortButton->setEnabled(false);
	}
	else ui.resortButton->setEnabled(true);
	if (remainNavigators == 0) {
		ui.navigateButton->setEnabled(false);
	}
	else ui.navigateButton->setEnabled(true);
}

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

void LianLianKan::linking(Block * next) {
	if (remainBlocks == 0 || !lifeTimer.isActive()) {
		return;
	}
	if (prev) {
		auto directionVec = map.connection(prev->y, prev->x, next->y, next->x);
		auto lightnings = map.getDirections(directionVec);
		if (directionVec.size() == 0) {
			prev->deselect();
			next->select();
			prev = next;
			return;
		}
		if (next->block_type == 27) {
			remainNavigators++;
		}
		if (next->block_type == 8) {
			remainResorts++;
		}
		if (next->block_type == 25 && isNetwork) {
			websocket.sendTextMessage("ITEM_MIRROR_" + QString::number(netId));
		}
		if (next->block_type == 29 && isNetwork) {
			websocket.sendTextMessage("ITEM_TIME_" + QString::number(netId));
		}
		if (next->block_type == 28 && isNetwork) {
			// websocket.sendTextMessage("ITEM_BOOM_" + QString::number(netId));
		}
		//if(next->block_type == )
		updateItems();
		remainBlocks -= 2;
		if (isNetwork) {
			websocket.sendTextMessage("ACTION_" + QString::number(netId) + "_" + QString::number(remainBlocks));
		}
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
		drawLightning(directionVec, lightnings);

		if (prev == next)
		{
			delete prev;
			prev = nullptr;
			return;
		}
		delete prev;
		delete next;
		prev = nullptr;
		// check if there is no more avalible block -- 
		if (remainBlocks == 0) {
			endGame();
			return;
		}
		if (map.prompt().size() == 0) {
			QGraphicsTextItem *prompt = new QGraphicsTextItem(QString::fromUtf16(u"检测到死局，请使用重排道具！"));
			prompt->setPos(200, 250);
			prompt->setDefaultTextColor(Qt::black);
			scene->addItem(prompt);
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
