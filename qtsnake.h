/************************************************************
*  开发人员 ： 刘凯                                           *
*  日   期 ： 2020年02月14日20时40分                           *
*  名   称 ： 山寨版贪吃蛇                                     *
*************************************************************/
#ifndef QTSNAKE_H
#define QTSNAKE_H
#include<QPainter>
#include<QList>
#include<QTimer>
#include<QKeyEvent>
#include<QRectF>//矩形类
#include<QPen>
#include<QBrush>
#include<QDebug>
#include<QTime>
#include <QWidget>

namespace Ui {
class QtSnake;
}

class QtSnake : public QWidget
{
    Q_OBJECT

public:
    explicit QtSnake(QWidget *parent = 0);
    ~QtSnake();
protected:
    void paintEvent(QPaintEvent *);//绘图事件
    void keyPressEvent(QKeyEvent *);//鼠标点击事件
private:
    void addTopRectF();//向上按
    void addLeftRectF();//向左按
    void addDownRectF();//向下按
    void addRightRectF();//向右按
    bool snakeStrike();//蛇撞到自己
    void deleteLastRectF();
    void gameEnd();//游戏结束
    enum Move{Up,Down,Left,Right};//移动的方向
  private slots:
    void timeOut();
    void rewardTimeOut();

private:
    Ui::QtSnake *ui;
    QList<QRectF>snake;//蛇本体
    int snakeNodeWidth = 10;// 蛇的每一节都是正方形的格子边长为10
    int snakeNodeHeight = 10;
    QTimer*timer;
    QTimer*rewardTimer;
    int time =100;
    int moveFlag =Up;
    bool gameOver =false;
    bool gameStart = false;
    QList<QRectF>rewardNode;//奖励节点

};

#endif // QTSNAKE_H
