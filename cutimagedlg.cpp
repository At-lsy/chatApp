#include "cutimagedlg.h"
#include "ui_cutimagedlg.h"

#include<QCursor>
#include<QDebug>
#include<QPainter>
#include<QRegion>
#include<QPen>


CutImageDlg::CutImageDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CutImageDlg)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);

}

CutImageDlg::~CutImageDlg()
{
    delete ui;
}


void CutImageDlg::setMouseCursor(QPoint point)
{
    QPoint leftTop=this->rect().topLeft();
    QPoint rightBottom=this->rect().bottomRight();

    QPoint leftBottom=this->rect().bottomLeft();
    QPoint rightTop=this->rect().topRight();
    //设置鼠标在上下设置大小的鼠标样式
    if((point.y()==leftTop.y()||point.y()==rightBottom.y())&&(point.x()>leftTop.x()&&point.x()<rightBottom.x())){
        this->setCursor(Qt::SizeVerCursor);
    }
    //设置鼠标在左右设置大小的样式
    else if((point.x()==leftTop.x()||point.x()==rightBottom.x())&&(point.y()>leftTop.y()&&point.y()<rightBottom.y())){
        this->setCursor(Qt::SizeHorCursor);
    }
    //设置鼠标在中间移动的鼠标样式
    else if(point.x()>leftTop.x()&&point.x()<rightBottom.x()&&point.y()>leftTop.y()&&point.y()<rightBottom.y()){
        this->setCursor(Qt::SizeAllCursor);
    }
    //设置鼠标在左上角和右下角设置大小的样式
    else if(point==leftTop||point==rightBottom){
        this->setCursor(Qt::SizeFDiagCursor);
    }
    //设置鼠标在左下角和右上角设置大小的样式
    else if(point==leftBottom||point==rightTop){
        this->setCursor(Qt::SizeBDiagCursor);
    }
}

void CutImageDlg::resizeDlg(QPoint point)
{
    switch(this->cursor().shape()){
        case Qt::SizeVerCursor:
            //qDebug()<<"上下调整大小";
            top_to_bottom(point);
            break;
        case Qt::SizeHorCursor:
            //qDebug()<<"左右调整大小";
            left_to_right(point);
            break;
        case Qt::SizeBDiagCursor:
            //qDebug()<<"左下至右上调整大小";
            lBottom_to_rTop(point);
            break;
        case Qt::SizeFDiagCursor:
            //qDebug()<<"左上至右下调整大小";
            lTop_to_rBottom(point);
            break;
    }
}

void CutImageDlg::top_to_bottom(QPoint point)
{
    //为向上拉
//    qDebug()<<"dy:"<<dy<<"last_point:"<<last_point;

    QRect newGeo=last_geo;
    if(point.y()<10){
        int dy=point.y()-last_point.y();
        newGeo.setTop(last_geo.top()+dy);
    }
    //为向下拉
    else if(point.y()>last_geo.height()-10){
        newGeo.setHeight(point.y());
    }

    if(check_border(newGeo)){
        this->setGeometry(newGeo);
        last_geo=this->geometry();
    }
}

void CutImageDlg::left_to_right(QPoint point)
{
    //为向左拉
    QRect newGeo=last_geo;
    qDebug()<<"left:"<<last_geo.left()<<"bottom:"<<last_geo.bottom();
    if(point.x()<10){
        int dx=point.x()-last_point.x();
        newGeo.setLeft(last_geo.x()+dx);
    }
    else if(point.x()>last_geo.width()-10){
        int dx=point.x()-last_geo.width();
        newGeo.setRight(last_geo.right()+dx);
    }
    if(check_border(newGeo)){
        this->setGeometry(newGeo);
        last_geo=this->geometry();
    }
}

void CutImageDlg::lBottom_to_rTop(QPoint point)
{
    QRect newGeo=last_geo;
    if(point.x()<20){
        int dx=point.x()-last_point.x();
        int dy=point.y() - last_geo.height();
        newGeo.setLeft(last_geo.left()+dx);
        newGeo.setHeight(last_geo.height()+dy);
    }
    else if(point.x()>last_geo.width()-10){
        int dy=point.y()-last_point.y();
        int dx=point.x()-last_geo.width();
        newGeo.setTop(last_geo.top()+dy);
        newGeo.setRight(last_geo.right()+dx);
    }

    if(check_border(newGeo)){
        this->setGeometry(newGeo);
        last_geo=this->geometry();
    }
}

void CutImageDlg::lTop_to_rBottom(QPoint point)
{
    QRect newGeo=last_geo;
    if(point.x()<10){
        int dx=point.x()-last_point.x();
        int dy=point.y()-last_point.y();
        newGeo.setLeft(last_geo.left()+dx);
        newGeo.setTop(last_geo.top()+dy);
    }
    else if(point.x()>last_geo.width()-10){
        int dx=point.x()-last_geo.width();
        int dy=point.y()-last_geo.height();
        newGeo.setRight(last_geo.right()+dx);
        newGeo.setBottom(last_geo.bottom()+dy);
    }

    if(check_border(newGeo)){
        this->setGeometry(newGeo);
        last_geo=this->geometry();
    }
}

void CutImageDlg::set_size(int width, int height)
{
    this->resize(width,height);
}

void CutImageDlg::set_Random(QRect random_rect)
{
    this->random_rect=random_rect;
}

bool CutImageDlg::check_border(QRect new_Rect)
{

    if(new_Rect.left()<random_rect.left()||new_Rect.top()<random_rect.top()||new_Rect.right()>random_rect.right()||new_Rect.bottom()>random_rect.bottom()){
        qDebug()<<"超出边界";
        return false;
    }
    return true;
}

void CutImageDlg::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        Qt::CursorShape curShape=this->cursor().shape();
        if(curShape==Qt::SizeVerCursor||curShape==Qt::SizeHorCursor||curShape==Qt::SizeBDiagCursor||curShape==Qt::SizeFDiagCursor){
            qDebug()<<"重设大小";
            isResize=true;
            last_point=event->pos();
            last_gloPoint=event->globalPos();
            this->last_geo=this->geometry();
        }
        else if(curShape==Qt::SizeAllCursor){
            qDebug()<<"移动窗口";
            isMove=true;
            last_point=event->pos();
            last_gloPoint=event->globalPos();
            this->last_geo=this->geometry();
        }
    }
}

void CutImageDlg::mouseMoveEvent(QMouseEvent *event)
{
    if(isMove){
        int dx=event->globalPos().x()-last_gloPoint.x();
        int dy=event->globalPos().y()-last_gloPoint.y();
        QRect new_Rect=last_geo;
        new_Rect.moveTopLeft(QPoint(last_geo.x()+dx,last_geo.y()+dy));
        //this->move(new_Rect.topLeft());
        if(check_border(new_Rect)){
            this->move(new_Rect.topLeft());
        }
        else{
            last_gloPoint=event->globalPos();
            last_geo=this->geometry();
            qDebug()<<"last_gloPoint:"<<last_gloPoint<<"event->globalPos():"<<event->globalPos();
        }
    }
    else if(isResize){
        resizeDlg(event->pos());
    }
    else{
        setMouseCursor(event->pos());
    }
}

void CutImageDlg::mouseReleaseEvent(QMouseEvent *event)
{
    isResize=false;
    isMove=false;
    this->setCursor(Qt::ArrowCursor);
}

void CutImageDlg::paintEvent(QPaintEvent *event)
{
    //qDebug()<<this->geometry();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen=painter.pen();
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(228,228,228,10));
    painter.drawRect(this->rect());

    //绘制边框
    painter.setBrush(Qt::NoBrush);
    pen.setColor(Qt::white);
    pen.setWidth(4);
    painter.setPen(pen);
    QRect rect=this->rect().adjusted(2,2,-2,-2);
    painter.drawRect(rect);


    //绘制空心圆
    QRegion e_region(rect,QRegion::Ellipse);
    QRegion e_rect(rect);
    QRegion result = e_rect.subtracted(e_region);
    painter.setClipRegion(result);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(228,228,228,127));
    painter.drawRect(rect);
}
