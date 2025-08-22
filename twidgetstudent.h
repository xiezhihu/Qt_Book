#ifndef TWIDGETSTUDENT_H
#define TWIDGETSTUDENT_H

#include "tmyicondelegate.h"
#include <QWidget>
#include <QtSql>
#include <QStandardItemModel>



namespace Ui {
class TWidgetStudent;
}

class TWidgetStudent : public QWidget
{
    Q_OBJECT
    enum class TabWidgetType{
        Person=0,Message,Query,Quit
    };
    enum class MessageStackWidgetType{
        Message=0,Password,Debt
    };
    enum class QueryStackWidgetType{
        ShowData=0,Category
    };


private:
    QTimer *timer;
    QSqlQuery *query;
    QSqlDatabase DB;
    TMyIconDelegate *iconDelegate;
    QString pwd;
    QString name;
    qint64 number;
    int debt;
    QByteArray icon;

    QStandardItemModel *queryItemModel;
    QString currAuthor;
    QString currBookName;
    QString currCategory;
    int rowCount;


private:
    int id;
    void iniTabPerson();
    void setQueryTabModel(int pag=1);
    void do_timeOut();

public:
    explicit TWidgetStudent(int ID,QWidget *parent = nullptr);
    ~TWidgetStudent();
private slots:
    void on_btnPerson_clicked();

    void on_btnMessage_clicked();

    void on_btnStackedPagMessage_clicked();

    void on_btnMessageSetIcon_clicked();

    void on_btnMessageSave_clicked();

    void on_btnStackedPagPassword_clicked();


    void on_btnCommitPwd_clicked();

    void on_btnStackedPagDebt_clicked();

    void on_btnRepay_clicked();

    void on_btnquery_clicked();

    void on_radioCategory_clicked(bool checked);

    void on_radioAuthor_clicked();


    void on_spinPag_valueChanged(int arg1);

    void on_btnLast_clicked();

    void on_btnFirst_clicked();

    void on_btnNext_clicked();

    void on_btnEnd_clicked();

private:
    Ui::TWidgetStudent *ui;
};

#endif // TWIDGETSTUDENT_H
