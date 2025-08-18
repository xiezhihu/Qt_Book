#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtSql>

QT_BEGIN_NAMESPACE

namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    enum class Role{
        STUDENT=1,TEACHER
    };

    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:

    void on_btnLogin_clicked();
public:
    Role roleType()const;
    int  userId();


private:
    Role role;
    QSqlDatabase DB;
    QSqlQuery *query;
    int id;

    void Login(const QString &user,const QString &pwd);

private:
    Ui::Dialog *ui;
    int MAXINPUT=3;
};
#endif // DIALOG_H
