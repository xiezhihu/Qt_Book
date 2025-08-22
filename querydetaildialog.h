#ifndef QUERYDETAILDIALOG_H
#define QUERYDETAILDIALOG_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class QueryDetailDialog;
}

class QueryDetailDialog : public QDialog
{
    Q_OBJECT
private:
    int bookID;
    int userID;
    QSqlDatabase DB;
    QSqlQuery *query;
    void iniDialog();

public:
    explicit QueryDetailDialog(int bookID,int userID,QWidget *parent = nullptr);
    ~QueryDetailDialog();

private slots:


    void on_btnBorrow_clicked();

private:
    Ui::QueryDetailDialog *ui;
};

#endif // QUERYDETAILDIALOG_H
