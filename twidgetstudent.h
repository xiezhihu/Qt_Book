#ifndef TWIDGETSTUDENT_H
#define TWIDGETSTUDENT_H

#include <QWidget>
#include <QtSql>


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
    QSqlDatabase DB;
    QString pwd;
    QString name;
    qint64 number;
    int debt;
    QByteArray icon;
private:
    int id;
    void iniTabPerson();
    void iniTabMessage();
    void iniTabQuery();
    void iniStackedPagMessage();
    void iniStackedPagPassWord();
    void iniStackedPagDebt();

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

private:
    Ui::TWidgetStudent *ui;
};

#endif // TWIDGETSTUDENT_H
