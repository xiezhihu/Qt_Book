#include "twidgetstudent.h"
#include "ui_twidgetstudent.h"
#include "tmyicondelegate.h"
#include <QTabBar>
#include <QtSql>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>


TWidgetStudent::TWidgetStudent(int ID,QWidget *parent)
    : QWidget(parent),id(ID)
    , ui(new Ui::TWidgetStudent)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->tabWidget->tabBar()->hide(); //隐藏标签
    ui->btnPerson->click();
}

TWidgetStudent::~TWidgetStudent()
{
    delete ui;
}

// 个人信息界面初始化
void TWidgetStudent::iniTab0()
{
    // 数据库连接,前面登陆界面已经连接过了，不用再连接，直接拿就可以
    QSqlDatabase DB=QSqlDatabase::database();
    QSqlQuery *query=new QSqlQuery(DB);
    query->prepare("SELECT login.username, login.icon, login.debt, login.number FROM login WHERE login.id = :ID");
    query->bindValue(":ID",id);
    bool ok=query->exec();
    if(!ok){
        QMessageBox::critical(this,"错误","信息初始化失败:"+query->lastError().text());
        delete query;
        return ;
    }else{
        query->next();
        QString Tname=query->value("username").toString();
        qint64 Tnumber=query->value("number").toLongLong();
        QByteArray TiconData=query->value("icon").toByteArray();
        int Tdebt=query->value("debt").toInt();
        ui->labName->setText("姓名:"+Tname);
        ui->labid->setText("学号:"+QString::number(Tnumber));
        ui->labDebt->setText("欠款:"+QString::number(Tdebt));
        QPixmap map;
        map.loadFromData(TiconData);
        ui->labIcon->setPixmap(map.scaled(300,300,Qt::KeepAspectRatio,Qt::SmoothTransformation));

        this->name=Tname;
        this->number=Tnumber;
        this->debt=Tdebt;
        this->icon=TiconData;
    }


    //query
    QSqlQueryModel *queryModel=new QSqlQueryModel(ui->tableView);
    QString sql = R"(
    SELECT login.id, borrow.book_id, books.image_data,  books.title, books.author, books.press, borrow.borrow_date, borrow.return_date
    FROM login
    JOIN borrow ON login.id = borrow.user_id
    JOIN books ON borrow.book_id = books.id
    WHERE login.id = %1
    )";
    queryModel->setQuery(sql.arg(id),DB);
    if(queryModel->lastError().isValid()){
        QMessageBox::critical(this,"错误","查询失败:"+queryModel->lastError().text());
        delete query;
        return ;
    }

    //设置表头
    QSqlRecord rec=queryModel->record();
    queryModel->setHeaderData(rec.indexOf("image_data"),Qt::Horizontal,"封面");
    queryModel->setHeaderData(rec.indexOf("title"),Qt::Horizontal,"书名");
    queryModel->setHeaderData(rec.indexOf("author"),Qt::Horizontal,"作者");
    queryModel->setHeaderData(rec.indexOf("press"),Qt::Horizontal,"出版社");
    queryModel->setHeaderData(rec.indexOf("borrow_date"),Qt::Horizontal,"借阅时间");
    queryModel->setHeaderData(rec.indexOf("return_date"),Qt::Horizontal,"预计还书日期");


    //model/view
    TMyIconDelegate *delegate=new TMyIconDelegate(ui->tableView);
    ui->tableView->setItemDelegateForColumn(rec.indexOf("image_data"),delegate);


    ui->tableView->setModel(queryModel);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
    ui->tableView->setColumnHidden(rec.indexOf("id"),true);//设置隐藏
    ui->tableView->setColumnHidden(rec.indexOf("book_id"),true);//设置隐藏

    delete query;
    ui->tabWidget->setCurrentIndex(0);
}

// 信息修改界面初始化
void TWidgetStudent::iniTab1()
{
    ui->tabWidget->setCurrentIndex(1);
    ui->btnStackedPag0->click();
}

//信息修改界面——信息修改
void TWidgetStudent::iniStackedPag0()
{
    //初始化数据

    ui->lineMessageName->setText(this->name);
    ui->lineMessageNumber->setText(QString::number(this->number));
    QPixmap map;
    map.loadFromData(this->icon);
    ui->labelMessageIcon->setPixmap(map.scaled(300,300,Qt::KeepAspectRatio,Qt::SmoothTransformation));

    ui->stackedWidget->setCurrentIndex(0);
}

void TWidgetStudent::on_btnPerson_clicked()
{
    iniTab0();
}


void TWidgetStudent::on_btnMessage_clicked()
{
    iniTab1();
}


void TWidgetStudent::on_btnStackedPag0_clicked()
{
    iniStackedPag0();
}

// 设置头像
void TWidgetStudent::on_btnMessageSetIcon_clicked()
{
    QString fileName=QFileDialog::getOpenFileName(this,"选择图片",QCoreApplication::applicationFilePath(),"图片(*.jpg *.png)");
    if(fileName.isEmpty()){
        return ;
    }

    QFile file(fileName);
    if(file.exists() && file.open(QIODevice::ReadOnly)){
        QByteArray icon=file.readAll();
        file.close();

        ui->labelMessageIcon->clear();
        QPixmap map;
        map.loadFromData(icon);
        ui->labelMessageIcon->setPixmap(map.scaled(300,300,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    }else{
        QMessageBox::critical(this,"打开失败","请核对正确路径");
        return ;
    }
}


// 保存修改
void TWidgetStudent::on_btnMessageSave_clicked()
{
    // 准备数据
    QString Tname=ui->lineMessageName->text();
    qint64 Tnumber=ui->lineMessageNumber->text().toLongLong();
    QPixmap Ticon=ui->labelMessageIcon->pixmap(Qt::ReturnByValue);
    QByteArray byte;
    QBuffer buffer(&byte);
    buffer.open(QIODevice::WriteOnly);
    Ticon.save(&buffer,"PNG");


    // 保存
    QSqlDatabase DB=QSqlDatabase::database();
    DB.transaction(); //开启事务


    QSqlQuery *query = new QSqlQuery(DB);
    if(!Tname.isEmpty() && Tname!=this->name){
        query->prepare("UPDATE login"
                       " SET username = :Tname"
                       " WHERE id = :id");
        query->bindValue(":Tname",Tname);
        query->bindValue(":id",this->id);
        bool ok=query->exec();

        if(!ok){
            auto err=query->lastError();
            if(err.nativeErrorCode()=="1062"){
                QMessageBox::information(this,"提示","用户名已被使用！！！");
            }

            qDebug()<<"姓名保存失败:"+err.text();
            DB.rollback();
            return ;
        }
    }


    if(Tnumber!=this->number){
        query->prepare("UPDATE login"
                       " SET number = :Tnumber"
                       " WHERE id = :id");
        query->bindValue(":Tnumber",Tnumber);
        query->bindValue(":id",this->id);
        bool ok=query->exec();

        if(!ok){
            auto err=query->lastError();
            if(err.nativeErrorCode()=="1062"){
                QMessageBox::information(this,"提示","学号已被注册！！！");
            }

            qDebug()<<"学号保存失败:"+err.text();
            DB.rollback();
            return ;
        }
    }

    if(byte!=this->icon){
        query->prepare("UPDATE login"
                       " SET icon = :byte"
                       " WHERE id = :id");
        query->bindValue(":byte",byte);
        query->bindValue(":id",this->id);
        bool ok=query->exec();

        if(!ok){
            auto err=query->lastError();
            qDebug()<<"头像保存失败:"+err.text();
            DB.rollback();
            return ;
        }
    }

    DB.commit();
    QMessageBox::information(this,"提示","保存成功");
    this->name=Tname;
    this->number=Tnumber;
    this->icon=byte;

    delete query;
}

