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
    enum class StackWidgetType{
        Message=0,Password,Debt
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
    void iniStackedPagMessage();
    void iniStackedPagPassWord();

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

private:
    Ui::TWidgetStudent *ui;
};

#endif // TWIDGETSTUDENT_H
