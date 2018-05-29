#pragma once

#include <QtWidgets/QMainWindow>
#include <vector>
#include "ui_LianLianKan.h"
#include "Block.h"
#include "Lightning.h"
#include "BoomEffect.h"
#include "GraphicsScene.h"
#include "Map.h"
#include <QPixmap>
#include <QMediaPlayer>
#include <QSound>
#include <QGraphicsTextItem>
#include <QMediaPlaylist>

class LianLianKan : public QMainWindow
{
	Q_OBJECT

public:
	LianLianKan(QWidget *parent = Q_NULLPTR);
	void removeBoom(BoomEffect * effect);
	void updateUserInfo();
	QString username;
protected:
	bool eventFilter(QObject *obj, QEvent *event);
private:
	Ui::LianLianKanClass ui;
	QGraphicsScene *scene;
	std::vector<Lightning *> lightningSequence;
	std::vector<QPixmap> boomPixmaps;
	std::vector<QPixmap> fireworkPixmaps;
	std::vector<QPixmap> blockPixmaps;
	Block *prev = nullptr;
	Map map;
	void drawLightning(std::vector<std::vector<int>> seq);
	void drawBoom(Block * prev, Block * next);
	void linking(Block * next);
	void drawBlocks();
	QTimer lifeTimer;
	int remainBlocks;
	int remainResorts;
	int remainNavigators;
	QMediaPlayer player;
	int score;
	void loadResources();
private slots:
	void updateTimer();
	void navigateGame();
	void pauseGame();
	void resortGame();
	void updateItems();
	void startGame();
	void setBackgroundMusic();
	void endGame();
};
