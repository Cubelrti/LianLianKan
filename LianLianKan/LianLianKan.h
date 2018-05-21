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


class LianLianKan : public QMainWindow
{
	Q_OBJECT

public:
	LianLianKan(QWidget *parent = Q_NULLPTR);
	void removeBoom(BoomEffect * effect);
protected:
	bool eventFilter(QObject *obj, QEvent *event);
private:
	Ui::LianLianKanClass ui;
	QGraphicsScene *scene;
	std::vector<Lightning *> lightningSequence;
	std::vector<QPixmap> boomPixmaps;
	Block *prev = nullptr;
	Map map;
	void drawLightning(std::vector<std::vector<int>> seq);
	void drawBoom(Block * prev, Block * next);
	void linking(Block * next);
	void drawBlocks();
private slots:
	void resortGame();
	void startGame();
};
