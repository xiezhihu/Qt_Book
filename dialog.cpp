#include "dialog.h"
#include "./ui_dialog.h"
#include <QMessageBox>
#include <QDebug>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setWindowTitle("登录");
    this->setFixedSize(1091,826);


    ui->frameLogin->setStyleSheet(
        "QFrame {"
        "border-image: url(:/icon/2.png) 0 0 0 0 stretch stretch;"
        "}"
        );
    ui->frame->setStyleSheet(
        "QFrame {"
        "border-image: url(:/icon/1.png) 0 0 0 0 stretch stretch;"
        "}"
        );
}

Dialog::~Dialog()
{
    qDebug()<<int(role);
    delete ui;
}


void Dialog::on_btnLogin_clicked()
{
    QString user=ui->lineUser->text();
    QString pwd=ui->linePwd->text();
    if(user.isEmpty() || pwd.isEmpty()){
        QMessageBox::information(this,"登陆失败","用户名或密码不能为空！！！");
        MAXINPUT--;
        if(MAXINPUT==0){
            this->reject();
        }
        return ;
    }

    DB= QSqlDatabase::addDatabase("QMYSQL");
    DB.setHostName("localhost");
    DB.setUserName("root");
    DB.setPassword("123456");
    DB.setDatabaseName("book");
    if(DB.open()){
        Login(user,pwd);
    }else{
        QMessageBox::critical(this,"错误","数据库连接失败:"+DB.lastError().text());
        return ;
    }

}

void Dialog::Login(const QString &user, const QString &pwd)
{
    query=new QSqlQuery(DB);
    QString str="SELECT password, role, id"
                  " FROM login"
                  " WHERE username = :user;";
    query->prepare(str);
    query->bindValue(":user",user);
    bool ok=query->exec();


    if(!ok){
        QMessageBox::critical(this,"错误","验证失败:"+query->lastError().text());
        return ;
    }

    query->last();
    QString pass=query->value("password").toString();
    if(pass==pwd){
        QMessageBox::information(this,"提示","登陆成功");
        QString type=query->value("role").toString();
        role=(type=="STUDENT")?Role::STUDENT : Role::TEACHER;
        id=query->value("id").toInt();
        qDebug()<<int(role)<<" "<<id;
        this->accept();
    }else{
        QMessageBox::information(this,"登陆失败","用户名或密码错误！！！");
        MAXINPUT--;
        if(MAXINPUT==0){
            this->reject();
        }
    }
}

int Dialog::userId()
{
    return id;
}


Dialog::Role Dialog::roleType() const
{
    return this->role;
}

