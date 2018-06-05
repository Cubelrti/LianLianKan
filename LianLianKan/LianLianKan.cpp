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

void LianLianKan::setBackgroundMusic()
{
	QMediaPlaylist *playlist = new QMediaPlaylist();
	playlist->addMedia(QUrl::fromLocalFile("./Sounds/BGM.mp3"));
	playlist->setPlaybackMode(QMediaPlaylist::Loop);
	player.setPlaylist(playlist);
	player.setVolume(50);
	player.play();
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
