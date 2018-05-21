#pragma once


#include <QGraphicsView>
#include <QPoint>
#include <QMouseEvent>
class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QObject *parent = 0);

signals:

protected:
	void GraphicsView::mousePressEvent(QMouseEvent *event);

};