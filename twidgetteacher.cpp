#include "twidgetteacher.h"
#include "ui_twidgetteacher.h"
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>

TWidgetTeacher::TWidgetTeacher(int id,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TWidgetTeacher)
{
    ui->setupUi(this);
    this->id = id;
    DB = QSqlDatabase::database();
    query = new QSqlQuery(DB);
    ui->btnPerson->click();
}

TWidgetTeacher::~TWidgetTeacher()
{
    delete ui;
}

// 个人中心
void TWidgetTeacher::on_btnPerson_clicked()
{
    ui->stackedWidget->setCurrentIndex(int(WidgetType::Person));
    ui->btnMessageChange->click();

}

// 个人中心-信息修改
void TWidgetTeacher::on_btnMessageChange_clicked()
{
    query->prepare("SELECT id, icon, username"
                   " FROM login"
                   " WHERE id = :id");
    query->bindValue(":id",this->id);
    bool ok=query->exec();
    if(!ok){
        QMessageBox::critical(this,"错误","初始化失败:"+query->lastError().text());
        return ;
    }
    query->next();
    QByteArray Ticon = query->value("icon").toByteArray();
    QString TuserName =  query->value("username").toString();
    ui->lineName->setText(TuserName);
    QPixmap map;
    map.loadFromData(Ticon);
    ui->labPersonIcon->setPixmap(map.scaled(300,300,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->personStackedWidget->setCurrentIndex(0);
}

// 个人中心-密码修改
void TWidgetTeacher::on_btnPwdChange_clicked()
{
    // 初始化pwd
    query->prepare("SELECT id, password, username"
                   " FROM login"
                   " WHERE id = :id");
    query->bindValue(":id",this->id);
    bool ok=query->exec();
    if(!ok){
        QMessageBox::critical(this,"错误","初始化失败:"+query->lastError().text());
        return ;
    }
    query->next();
    this->pwd=query->value("password").toString();

    ui->personStackedWidget->setCurrentIndex(1);
}


// 更换头像
void TWidgetTeacher::on_btnPersonIconChange_clicked()
{
    QString fileName=QFileDialog::getOpenFileName(this,"选择图片",QCoreApplication::applicationFilePath(),"图片(*.jpg *.png)");
    if(fileName.isEmpty()){
        return ;
    }

    QFile file(fileName);
    if(file.exists() && file.open(QIODevice::ReadOnly)){
        QByteArray icon=file.readAll();
        file.close();

        ui->labPersonIcon->clear();
        QPixmap map;
        map.loadFromData(icon);
        ui->labPersonIcon->setPixmap(map.scaled(300,300,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    }else{
        QMessageBox::critical(this,"打开失败","请核对正确路径");
        return ;
    }

}

// 信息修改
void TWidgetTeacher::on_btnCommitMessage_clicked()
{
    QPixmap Ticon=ui->labPersonIcon->pixmap(Qt::ReturnByValue);
    QByteArray byte;
    QBuffer buffer(&byte);
    buffer.open(QIODevice::WriteOnly);
    Ticon.save(&buffer,"PNG");

    QString TuserName=ui->lineName->text();


    query->prepare("UPDATE login"
                   " SET username = :TuserName, icon = :Ticon"
                   " WHERE id = :id");
    query->bindValue(":TuserName",TuserName);
    query->bindValue(":Ticon",byte);
    query->bindValue(":id",this->id);
    bool ok=query->exec();

    if(!ok){
        auto err=query->lastError();
        if(err.nativeErrorCode()=="1062"){
            QMessageBox::information(this,"提示","用户名已被使用！！！");
        }else{
            QMessageBox::critical(this,"错误","提交修改失败:"+err.text());
        }

    }else{
        QMessageBox::information(this,"提示","修改成功");
    }

}


// 密码修改
void TWidgetTeacher::on_btnOk_clicked()
{
    // 获取数据
    QString Opwd=ui->linePwd->text();
    QString Npwd=ui->lineNewPwd->text();
    QString Apwd=ui->lineAgainPwd->text();

    if(Opwd.isEmpty()){
        QMessageBox::information(this,"提示","原密码不为空！！！");
        ui->lineNewPwd->clear();
        ui->lineAgainPwd->clear();
        return ;
    }
    if(Npwd.isEmpty()){
        QMessageBox::information(this,"提示","新密码不能为空！！！");
        ui->lineAgainPwd->clear();
        return ;
    }
    if(Apwd.isEmpty()){
        QMessageBox::information(this,"提示","请再次确认密码！！！");
        return ;
    }

    if(Npwd!=Apwd){
        QMessageBox::information(this,"提示","两次密码不一致！！！");
        return ;
    }

    if(Opwd!=this->pwd){
        QMessageBox::information(this,"提示","密码错误！！！");
        ui->lineNewPwd->clear();
        ui->lineAgainPwd->clear();
        return ;
    }

    if(Opwd==Npwd){
        QMessageBox::information(this,"提示","新密码和原密码一致!!!");
        return ;
    }

    // 修改密码

    query->prepare("UPDATE login"
                   " SET login.password = :Npwd"
                   " WHERE login.id = :id");
    query->bindValue(":Npwd",Npwd);
    query->bindValue(":id",this->id);
    bool ok=query->exec();
    if(!ok){
        QMessageBox::information(this,"提示","修改失败:"+query->lastError().text());

    }else{
        QMessageBox::information(this,"提示","修改成功");
        this->pwd=Npwd;
    }

    ui->linePwd->clear();
    ui->lineNewPwd->clear();
    ui->lineAgainPwd->clear();

    return ;
}



