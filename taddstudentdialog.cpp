#include "taddstudentdialog.h"
#include "ui_taddstudentdialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QBuffer>
#include <QtSql>
#include <QDebug>

TAddStudentDialog::TAddStudentDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TAddStudentDialog)
{
    ui->setupUi(this);
    ui->btnIcon->setFixedSize(200,200);
    ui->btnIcon->setIconSize(ui->btnIcon->size());
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setModal(true);
    this->setFixedSize(561,382);

}

TAddStudentDialog::~TAddStudentDialog()
{
    qDebug()<<this->width()<<" "<<this->height();
    delete ui;
}

void TAddStudentDialog::on_btnIcon_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,"选择一张图片",QCoreApplication::applicationFilePath(),"图片 (*.jpg *.png)");
    if(fileName.isEmpty())return ;
    QFile file(fileName);
    if(!file.exists()){
        QMessageBox::information(this,"提示","请检查文件是否存在！！！");
        return ;
    }

    QIcon icon(fileName);
    ui->btnIcon->setIcon(icon);
}


void TAddStudentDialog::on_btnCommite_clicked()
{
    QString userName = ui->lineName->text();
    QString numberStr = ui->lineNumber->text();
    if(userName.isEmpty() || numberStr.isEmpty()){
        QMessageBox::information(this,"提示","姓名、学号不能为空！！！");
        return ;
    }
    if(numberStr.length()!=10){
        QMessageBox::information(this,"提示","学号格式有误！！！");
        return ;
    }

    QPixmap Ticon=ui->btnIcon->icon().pixmap(ui->btnIcon->size());
    QByteArray byte;
    QBuffer buffer(&byte);
    buffer.open(QIODevice::WriteOnly);
    Ticon.save(&buffer,"PNG");

    QSqlDatabase DB = QSqlDatabase::database();
    QSqlQuery *query = new QSqlQuery(DB);

    query->prepare("INSERT INTO login (username, password, role, debt, icon, number)"
                   " VALUES (:username, '123456', 'STUDENT', 0, :icon, :number)");
    query->bindValue(":username",userName);
    query->bindValue(":icon",byte);
    query->bindValue(":number",numberStr.toLongLong());

    query->exec();
    auto err = query->lastError();
    if(err.isValid()){
        if(err.nativeErrorCode() == "1062"){
            QMessageBox::information(this,"提示","学号已被使用！！！");
        }else{
            QMessageBox::information(this,"提示","添加失败:"+err.text());
        }
        return ;
    }
    QMessageBox::information(this,"提示","添加成功！！！");
    ui->lineName->clear();
    ui->lineNumber->clear();
    ui->btnIcon->setIcon(QIcon(":/icon/icon.png"));
}

