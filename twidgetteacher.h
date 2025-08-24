#ifndef TWIDGETTEACHER_H
#define TWIDGETTEACHER_H

#include <QWidget>
#include <QtSql>
#include <QStandardItemModel>

namespace Ui {
class TWidgetTeacher;
}

class TWidgetTeacher : public QWidget
{
    Q_OBJECT
    enum class WidgetType{
        Person,Management,Book,Quit
    };

private:
    QSqlDatabase DB;
    QSqlQuery *query;
    int id;
    QString pwd;

public:
    explicit TWidgetTeacher(int id,QWidget *parent = nullptr);
    ~TWidgetTeacher();

private slots:
    void on_btnOk_clicked();

    void on_btnPerson_clicked();

    void on_btnMessageChange_clicked();

    void on_btnPwdChange_clicked();

    void on_btnPersonIconChange_clicked();

    void on_btnCommitMessage_clicked();

    void on_btnManagement_clicked();

private:
    Ui::TWidgetTeacher *ui;
};

#endif // TWIDGETTEACHER_H
