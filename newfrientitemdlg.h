#ifndef NEWFRIENTITEMDLG_H
#define NEWFRIENTITEMDLG_H

#include <QWidget>

namespace Ui {
class NewFrientItemDlg;
}

class NewFrientItemDlg : public QWidget
{
    Q_OBJECT

public:
    explicit NewFrientItemDlg(quint16 state,QString id,QString name,QString msg,QWidget *parent = nullptr);
    ~NewFrientItemDlg();
    void init();
    void setButtonStyle();

public slots:
    void change_butCondition(quint16 condition);

private slots:
    void on_rbut_select_clicked();

signals:
    void agree_addFriend(QString id);

private:
    Ui::NewFrientItemDlg *ui;
    quint16 item_status;
    QString client_id;
    QString client_name;
    QString client_msg;
    char* msg_status[];
};

#endif // NEWFRIENTITEMDLG_H
