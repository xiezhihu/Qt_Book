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

    enum class QueryStackWidgetType{
        ShowData=0,Category
    };
private:
    QSqlDatabase DB;
    QSqlQuery *query;
    int id;
    QString pwd;
    QStandardItemModel *itemModel;
    QHash<int,std::function<void()>> fun;



    QString currAuthor;
    QString currBookName;
    QString currCategory;


    void setLoginTableView(int pag,QString name = QString(),QString number = QString());
    void do_change();
    void do_resetPwd();
    void do_clearDebt();
    void do_delete();
    void do_AllPushBUtton();
    void do_addBookSum();
    void do_SetBook();
    void do_deleleBook();
    void setSpinPagMax(int rowCount);
    void setQueryTabModel(int pag=1);
    void setSpinBox(int rowCount);

public:
    explicit TWidgetTeacher(int id,QWidget *parent = nullptr);
    ~TWidgetTeacher();

private slots:

    void on_btnPerson_clicked();

    void on_btnMessageChange_clicked();

    void on_btnPwdChange_clicked();

    void on_btnPersonIconChange_clicked();

    void on_btnCommitMessage_clicked();

    void on_btnManagement_clicked();

    void on_btnFirst_clicked();

    void on_btnLast_clicked();

    void on_btnNext_clicked();

    void on_btnEnd_clicked();

    void on_spinPag_valueChanged(int arg1);

    void on_radName_clicked();

    void on_radNumber_clicked();

    void on_btnSearch_clicked();

    void on_lineSearch_textChanged(const QString &arg1);

    void on_btnAddStudent_clicked();

    void on_btnCommitPwd_clicked();

    void on_btnBook_clicked();


    void on_radioAuthor_clicked(bool checked);

    void on_radioName_clicked(bool checked);

    void on_radioCategory_clicked(bool checked);

    void on_spinPagBook_valueChanged(int arg1);

    void on_btnFirstBook_clicked();

    void on_btnLastBook_clicked();

    void on_btnNextBook_clicked();

    void on_btnEndBook_clicked();

    void on_btnSearchBook_clicked();

    void on_lineSearchBook_textChanged(const QString &arg1);

    void on_btnQuit_clicked();

private:
    Ui::TWidgetTeacher *ui;
};

#endif // TWIDGETTEACHER_H
