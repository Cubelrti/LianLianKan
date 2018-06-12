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
#include <QtWebSockets/QtWebSockets>

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
	QGraphicsPixmapItem *ban;
	QGraphicsRectItem *blind;
	Block *prev = nullptr;
	Map map;
	void drawLightning(std::vector<std::vector<int>> seq, std::vector<int> lightningSeq);
	void drawBoom(Block * prev, Block * next);
	void linking(Block * next);
	void drawBlocks(vector<vector<int>> mapVec);
	QTimer lifeTimer;
	int remainBlocks;
	int remainResorts;
	int remainNavigators;
	QMediaPlayer player;
	Difficulty difficulty = normal;
	int score;
	void loadResources();
	void resetDiffStyle();
	QWebSocket websocket;
	bool isGameNow = false;
	bool isNetwork = false;
	int netId = -1;
private slots:
	void selectNormalMode();
	void selectEasyMode();
	void selectHardMode();
	void updateVersus(QString message);
	void HandGame();
	void BlindGame();
	void MirrorGame();
	void ClearScene();
	void ObstacleGame();
	void versusGame();
	void updateTimer();
	void navigateGame();
	void boomGame();
	void pauseGame();
	void resortGame();
	void updateItems();
	void startGame();
	void setBackgroundMusic();
	void endGame();
};
