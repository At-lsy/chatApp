#ifndef SEARCHFRIENDSUCCEEDDLG_H
#define SEARCHFRIENDSUCCEEDDLG_H

#include <QWidget>
#include<QString>

namespace Ui {
class SearchFriendSucceedDlg;
}

class SearchFriendSucceedDlg : public QWidget
{
    Q_OBJECT

public:
    explicit SearchFriendSucceedDlg(QString id,QString name,QWidget *parent = nullptr);
    ~SearchFriendSucceedDlg();
    bool inFocus(QWidget* obj);
    void changeFocus(QWidget *old, QWidget *now);

private slots:
    void on_but_add_clicked();

protected:
    void focusOutEvent(QFocusEvent* event);

signals:
    void request_addFriend(QString id,QString name);

private:
    Ui::SearchFriendSucceedDlg *ui;
    QString my_id;
    QString my_name;
};

#endif // SEARCHFRIENDSUCCEEDDLG_H
