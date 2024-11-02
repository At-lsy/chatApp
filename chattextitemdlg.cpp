#include "chattextitemdlg.h"
#include "ui_chattextitemdlg.h"
#include<QPainter>
#include<QPixmap>
#include<QRectF>
#include<QFont>
#include<QPainterPath>
#include<QPolygonF>
#include<QPoint>

#include"globals.h"

chatTextItemDlg::chatTextItemDlg(QString clientid,bool isLeft,QString message,QWidget *parent) :
    QWidget(parent),isLeft(isLeft),My_Id(clientid),message(message),
    ui(new Ui::chatTextItemDlg)
{
    ui->setupUi(this);
    head_path=MAIN_PATH+"/head_image/"+clientid+".jpg";
}

chatTextItemDlg::~chatTextItemDlg()
{
    delete ui;
}

void chatTextItemDlg::paintEvent(QPaintEvent *event)
{
    int win_width=this->rect().width();
    QFont font=this->font();
    font.setPixelSize(25);
    font.setFamily("Microsoft YaHei");
    QPixmap head(head_path);
    QPainter painter(this);
    painter.setFont(font);
    if(isLeft){
        //绘制头像
        painter.drawPixmap(BORDER_GAP,TOP_GAP,HEAD_WIDTH,HEAD_WIDTH,head);
        //绘制聊天气泡
        QRectF default_rect(BORDER_GAP+HEAD_WIDTH+MID_GAP,TOP_GAP+4,win_width*0.6-(BORDER_GAP+HEAD_WIDTH+MID_GAP),HEAD_WIDTH);
        QRectF text_rect=painter.boundingRect(default_rect,Qt::AlignLeft|Qt::TextWordWrap,this->message);
        QRectF background_rect=text_rect.adjusted(-TEXT2BACK_GAP,-TEXT2BACK_GAP,2*TEXT2BACK_GAP,2*TEXT2BACK_GAP);

        QPainterPath background_path;
        background_path.addRoundedRect(background_rect,4,4);
        QPolygonF polygon;
        QPointF point1=background_rect.topLeft()+QPointF(0,10);
        QPointF point2=point1+QPointF(0,20);
        QPointF point3=QPointF(BORDER_GAP+HEAD_WIDTH+4,TOP_GAP+HEAD_WIDTH/2);
        polygon.append(point1);
        polygon.append(point2);
        polygon.append(point3);
        background_path.addPolygon(polygon);

        painter.fillPath(background_path,QBrush(QColor(255,255,255)));
        painter.drawText(text_rect,Qt::AlignLeft|Qt::TextWordWrap,this->message);

        int h=background_rect.height()+TOP_GAP+10;
        emit sendHight(h);
    }
    else{
        painter.drawPixmap(win_width-BORDER_GAP-HEAD_WIDTH,TOP_GAP,HEAD_WIDTH,HEAD_WIDTH,head);

        QRectF default_rect(win_width*0.4,TOP_GAP+4,win_width*0.6-(BORDER_GAP+HEAD_WIDTH+MID_GAP),HEAD_WIDTH);
        QRectF text_rect=painter.boundingRect(default_rect,Qt::AlignRight|Qt::TextWordWrap,this->message);
        QRectF background_rect=text_rect.adjusted(-TEXT2BACK_GAP,-TEXT2BACK_GAP,2*TEXT2BACK_GAP,2*TEXT2BACK_GAP);

        QPainterPath background_path;
        background_path.addRoundedRect(background_rect,4,4);
        QPolygonF polygon;
        QPointF point1=background_rect.topRight()+QPointF(0,10);
        QPointF point2=point1+QPointF(0,20);
        QPointF point3=QPointF(win_width-BORDER_GAP-HEAD_WIDTH-4,TOP_GAP+HEAD_WIDTH/2);
        polygon.append(point1);
        polygon.append(point2);
        polygon.append(point3);
        background_path.addPolygon(polygon);

        painter.fillPath(background_path,QBrush(QColor(150,235,105)));
        painter.drawText(text_rect,Qt::AlignLeft|Qt::TextWordWrap,this->message);

        int h=background_rect.height()+TOP_GAP+10;
        emit sendHight(h);
    }
}
