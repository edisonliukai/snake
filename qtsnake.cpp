#include "qtsnake.h"
#include "ui_qtsnake.h"

QtSnake::QtSnake(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QtSnake)
{
    ui->setupUi(this);
    this->setFixedSize(300,540);
    this->setWindowTitle("山寨版贪吃蛇");
    //设置窗口为黑色
    setStyleSheet("QWidget{background:black}");
    snake.append(QRectF(200,500,snakeNodeWidth,snakeNodeHeight));
    addTopRectF();
    addTopRectF();
    //生成第一个奖励节点(食物）
    rewardNode.append(QRectF(100,100,snakeNodeWidth,snakeNodeHeight));

    timer = new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(timeOut()));
    rewardTimer = new QTimer;
    connect(rewardTimer,SIGNAL(timeout()),SLOT(rewardTimeOut()));

}
//界面刷新
void QtSnake::timeOut()
{
    int  flag =1;
    for(int i =0;i<rewardNode.length();i++)
    {
        if(rewardNode.at(i).contains(snake.at(0).topLeft()
                                     +QPointF(snakeNodeWidth/2,snakeNodeHeight/2))) {
            if(rewardNode.at(i).width()>snakeNodeWidth) {//大食物
                flag+=2;

            }
            flag++;//正常食物
            rewardNode.removeAt(i);
            break;
        }

    }
    while(flag--){
        switch(moveFlag) {
        case Up:
            addTopRectF();
            break;
        case Down:
            addDownRectF();
            break;
        case Left:
            addLeftRectF();
            break;
        case Right:
            addRightRectF();
            break;
        default:
            break;
        }
    }
    deleteLastRectF();
    update();
}
// 随机食物生成
void QtSnake::rewardTimeOut()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    if(rewardNode.length() > 5) {
        rewardNode.removeAt(qrand()%5);
    }
    rewardNode.append(QRectF(qrand()%(this->width()/20)*20,qrand()%(this->height()/20)*20,snakeNodeWidth,snakeNodeHeight));
    if(qrand()%5 == 3) {
        rewardNode.append(QRectF(qrand()%(this->width()/20)*20-5,qrand()%(this->height()/20)*20-5,snakeNodeWidth*2,snakeNodeHeight*2));
    }
}
//向上移动吃食物增加蛇身体
void QtSnake::addTopRectF()
{
    snake.insert(0,QRectF(snake.at(0).topLeft()+QPointF(0,-snakeNodeHeight),snake.at(0).topRight()));

}
//向下
void QtSnake::addDownRectF()
{
    snake.insert(0,QRectF(snake.at(0).bottomLeft(),snake.at(0).bottomRight()+QPointF(0,snakeNodeHeight)));
}
//向左
void QtSnake::addLeftRectF()
{
    snake.insert(0,QRectF(snake.at(0).topLeft()+QPointF(-snakeNodeWidth,0),snake.at(0).bottomLeft()));
}
//向右
void QtSnake::addRightRectF()
{
    snake.insert(0,QRectF(snake.at(0).topRight(),snake.at(0).bottomRight()+QPointF(snakeNodeWidth,0)));
}
//绘图事件
void QtSnake::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);//创建画家对象
    QPen pen;//创建画笔对象(一般用来绘制轮廓）
    QBrush brush;//创建笔刷对象(一般用来填充)
    QFont font("方正舒体",12,QFont::ExtraLight,false);//字体样式
    //反锯齿（这种就要记住如何使用）
    painter.setRenderHint(QPainter::Antialiasing);
    pen.setColor(Qt::black);//设置画笔颜色
    brush.setColor(Qt::green);//设置笔刷颜色
    brush.setStyle(Qt::SolidPattern);//填充模式
    painter.setPen(pen);
    painter.setBrush(brush);
    //画蛇
    for(int i =0;i<snake.length();i++)
    {
        if(i == 0) {
            painter.drawEllipse(snake.at(i));
        } else {
            painter.drawRect(snake.at(i));
        }
    }
    brush.setColor(Qt::red);
    painter.setBrush(brush);
    //画食物
    for(int i=0;i<rewardNode.length();i++)
    {
        painter.drawEllipse(rewardNode.at(i)); //画椭圆
    }
    pen.setColor(Qt::white);
    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(20,20,QString("当前分数: ")+QString("%1").arg(snake.length()));
    if(snakeStrike()||(snake.at(0).x()-snakeNodeWidth < -10)//向左边撞墙
            ||(snake.at(0).x()+snakeNodeWidth*2 > this->width()+10)//向右边撞墙
            ||(snake.at(0).y()+snakeNodeHeight*2 > this->height()+10)//向下撞墙
            ||(snake.at(0).y()-snakeNodeHeight < -10)){//向上撞墙
        gameEnd();
        painter.setFont(font);
        painter.drawText((this->width()-40)/2,(this->height()-30)/2,QString("GAME OVER!"));
    }

    QWidget::paintEvent(event);
}
// 按键事件
void QtSnake::keyPressEvent(QKeyEvent*event)
{
    switch(event->key()) {
    case Qt::Key_Up:
        if(moveFlag !=Down) {
            moveFlag =Up;
        }
        break;
    case Qt::Key_Down:
        if(moveFlag !=Up) {
            moveFlag = Down;
        }
        break;
    case Qt::Key_Right:
        if(moveFlag !=Left) {
            moveFlag = Right;
        }
        break;
    case Qt::Key_Left:
        if(moveFlag != Right) {
            moveFlag = Left;
        }
        break;
    //case Qt::Key_Enter:
    case Qt::Key_Return:
        if(gameOver) {
            snake.clear();
            rewardNode.clear();
            moveFlag =Up;
            snake.append(QRectF(200,500,snakeNodeWidth,snakeNodeHeight));
            addTopRectF();
            addTopRectF();
            rewardNode.append(QRectF(100,100,snakeNodeWidth,snakeNodeHeight));

            timer->start(time);
            rewardTimer->start(time*30);
            gameOver = false;
        }
        break;
    case Qt::Key_Space:
        if(gameStart && !gameOver) {
            timer->start(time);
            rewardTimer->start(time*30);
            gameStart = false;
        } else if (!gameStart && !gameOver) {
            timer->stop();
            rewardTimer->stop();
            gameStart = true;
        }
        break;
    default:
        break;
    }
}
// 删除蛇本体最后一个数据
void QtSnake::deleteLastRectF()
{
    snake.removeLast();
}
//判断蛇是否与身体相撞
bool QtSnake::snakeStrike()
{
    for(int i=0;i<snake.length();i++)
    {
        for(int j =i+1;j<snake.length();j++)
        {
            if(snake.at(i) == snake.at(j)) {
                return true;
            }
        }
    }
    return false;
}
// 游戏结束
void QtSnake::gameEnd()
{
    timer->stop();
    rewardTimer->stop();
    gameOver = true;
}
QtSnake::~QtSnake()
{
    delete ui;
}
