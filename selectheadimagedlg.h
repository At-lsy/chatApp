#ifndef SELECTHEADIMAGEDLG_H
#define SELECTHEADIMAGEDLG_H

#include <QWidget>
#include<QRect>
#include<QDialog>
#include"cutimagedlg.h"

namespace Ui {
class SelectHeadImageDlg;
}

class SelectHeadImageDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SelectHeadImageDlg(QString path,QWidget *parent = nullptr);
    ~SelectHeadImageDlg();        
    void set_pixmap(QString path);
    QPixmap cut_picture(QRect rect);

private slots:
    void on_but_cut_clicked();
    void on_but_confirm_clicked();

signals:
    void ensure_cut(QPixmap cut_pixmap);

private:
    Ui::SelectHeadImageDlg *ui;
    QRect pic_rect;
    CutImageDlg* zoom=nullptr;
    QString pic_path;
    QPixmap cut_pixmap;
};

#endif // SELECTHEADIMAGEDLG_H
