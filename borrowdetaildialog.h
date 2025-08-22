#ifndef BORROWDETAILDIALOG_H
#define BORROWDETAILDIALOG_H

#include <QDialog>
#include <QByteArray>
#include <QtSql>

namespace Ui {
class BorrowDetailDialog;
}

class BorrowDetailDialog : public QDialog
{
    Q_OBJECT
private:
    int borrow_id;
    QSqlQuery *query;
    QSqlDatabase DB;

    void seleDialog();

public:
    explicit BorrowDetailDialog(int Tborrow_id,QWidget *parent = nullptr);
    ~BorrowDetailDialog();

private slots:
    void on_btnBorrow_clicked();

    void on_btnReturn_clicked();

private:
    Ui::BorrowDetailDialog *ui;
};

#endif // BORROWDETAILDIALOG_H
