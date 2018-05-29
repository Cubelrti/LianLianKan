#pragma once

#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QMouseEvent>
#include "ui_SignIn.h"

class SignIn : public QWidget
{
	Q_OBJECT

public:
	SignIn(QWidget *parent = Q_NULLPTR);
	~SignIn();

private:
	Ui::SignIn ui;
	QPoint mpos;
	void mousePressEvent(QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent * event);
private slots:
	void startGame();
	void quitGame();
};
