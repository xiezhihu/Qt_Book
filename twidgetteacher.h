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
    QStandardItemModel *itemModel;
    QHash<int,std::function<void()>> fun;

    void setLoginTableView(int pag);
    void do_change();
    void do_resetPwd();
    void do_clearDebt();
    void do_delete();

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
