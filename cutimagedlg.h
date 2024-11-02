#ifndef CUTIMAGEDLG_H
#define CUTIMAGEDLG_H

#include <QWidget>
#include<QMouseEvent>
#include<QPoint>
#include<QRect>

namespace Ui {
class CutImageDlg;
}

class CutImageDlg : public QWidget
{
    Q_OBJECT

public:
    explicit CutImageDlg(QWidget *parent = nullptr);
    ~CutImageDlg();
    void setMouseCursor(QPoint point);
    void resizeDlg(QPoint point);
    void top_to_bottom(QPoint point);
    void left_to_right(QPoint point);
    void lBottom_to_rTop(QPoint point);
    void lTop_to_rBottom(QPoint point);
    void set_size(int width,int height);
    void set_Random(QRect random_rect);
    bool check_border(QRect new_Rect);

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);


private:
    Ui::CutImageDlg *ui;
    bool isMove=false;
    bool isResize=false;
    QPoint last_point;
    QPoint last_gloPoint;
    QRect last_geo;
    QRect random_rect;
};

#endif // CUTIMAGEDLG_H
