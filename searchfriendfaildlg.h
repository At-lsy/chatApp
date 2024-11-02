#ifndef SEARCHFRIENDFAILDLG_H
#define SEARCHFRIENDFAILDLG_H

#include <QWidget>
#include<QDialog>

namespace Ui {
class SearchFriendFailDlg;
}

class SearchFriendFailDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SearchFriendFailDlg(QWidget *parent = nullptr);
    ~SearchFriendFailDlg();

private slots:
    void on_but_close_clicked();

private:
    Ui::SearchFriendFailDlg *ui;
};

#endif // SEARCHFRIENDFAILDLG_H
