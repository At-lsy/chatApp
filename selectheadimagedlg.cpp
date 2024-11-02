#include "selectheadimagedlg.h"
#include "ui_selectheadimagedlg.h"

#include<QDebug>
#include<QPixmap>
#include<QPainter>

SelectHeadImageDlg::SelectHeadImageDlg(QString path,QWidget *parent) :
    QDialog(parent),pic_path(path),
    ui(new Ui::SelectHeadImageDlg)
{
    ui->setupUi(this);
    set_pixmap(path);
}

SelectHeadImageDlg::~SelectHeadImageDlg()
{
    delete ui;
}

void SelectHeadImageDlg::set_pixmap(QString path)
{
    QPixmap pixmap(path);
    pixmap=pixmap.scaled(ui->label->size(),Qt::KeepAspectRatio);
    ui->label->setPixmap(pixmap);
    ui->label->setAlignment(Qt::AlignCenter);
    int dx=(ui->label->width()-pixmap.rect().width())/2;
    pic_rect=pixmap.rect();
    pic_rect.moveTopLeft(QPoint(dx,0));
    qDebug()<<"pic_rect:"<<pic_rect<<"pixmap.rect():"<<pixmap.rect()<<"dx:"<<dx;

    if(zoom==nullptr){
        zoom=new CutImageDlg(ui->label);
        zoom->show();
    }

    zoom->setGeometry(pic_rect);
    zoom->resize(pic_rect.height()/2,pic_rect.height()/2);
    zoom->set_Random(pic_rect);
}

QPixmap SelectHeadImageDlg::cut_picture(QRect rect)
{
    QPixmap init_pic(pic_path);
    QRect last_rect=rect;
    float factor_x=init_pic.width()*1.0/pic_rect.width();
    float factor_y=init_pic.height()*1.0/pic_rect.height();
    qDebug()<<"pic_rect:"<<pic_rect;
    qDebug()<<"init_pic_rect:"<<init_pic.rect();
    qDebug()<<"last_rect:"<<rect;
    rect.setX(rect.x()*factor_x);
    rect.setY(rect.y()*factor_y);
    rect.setWidth(last_rect.width()*factor_x);
    rect.setHeight(last_rect.height()*factor_y);
    qDebug()<<"cur_rect:"<<rect;

    QPixmap cut_pic=init_pic.copy(rect);
    return cut_pic;
}

void SelectHeadImageDlg::on_but_cut_clicked()
{
    QRect rect=zoom->geometry();
    rect.moveTopLeft(zoom->geometry().topLeft() - pic_rect.topLeft());
    cut_pixmap=cut_picture(rect);

    QPixmap background_pix(ui->label_2->size());
    background_pix.fill(Qt::transparent);
    QPainter painter(&background_pix);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    QRegion region(background_pix.rect(),QRegion::Ellipse);
    painter.setClipRegion(region);
    painter.drawPixmap(background_pix.rect(),cut_pixmap.scaled(ui->label_2->size(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation));
    ui->label_2->setPixmap(background_pix);

    //ui->label_2->setPixmap(cut_pixmap.scaled(ui->label_2->size()));
}

void SelectHeadImageDlg::on_but_confirm_clicked()
{
    emit ensure_cut(cut_pixmap);
}
