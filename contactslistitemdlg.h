#ifndef CONTACTS_H
#define CONTACTS_H

#include <QWidget>
#include<QString>
#include<QMouseEvent>

namespace Ui {
class ContactsListItemDlg;
}

class ContactsListItemDlg : public QWidget
{
    Q_OBJECT

public:
    explicit ContactsListItemDlg(QString c_id,QString c_remark,QWidget *parent = nullptr);
    ~ContactsListItemDlg();
    void init();
    QString get_remarkName();

protected:
    void mousePressEvent(QMouseEvent* event);

signals:
    void hasClicked(QString id,QString name);

private:
    Ui::ContactsListItemDlg *ui;
    QString client_id;//id号
    QString client_remark;//备注
};

#endif // CONTACTS_H
