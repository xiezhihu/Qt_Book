#ifndef TSETBOOKSDIALOG_H
#define TSETBOOKSDIALOG_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class TSetBooksDialog;
}

class TSetBooksDialog : public QDialog
{
    Q_OBJECT
private:
    int id;
    QSqlQuery *query;
    QSqlDatabase DB;

    void seleBook();

public:
    explicit TSetBooksDialog(int id,QWidget *parent = nullptr);
    ~TSetBooksDialog();

private slots:
    void on_btnIcon_clicked();

    void on_btnSave_clicked();

private:
    Ui::TSetBooksDialog *ui;
};

#endif // TSETBOOKSDIALOG_H
