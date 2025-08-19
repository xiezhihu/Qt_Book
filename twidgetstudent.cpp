#include "twidgetstudent.h"
#include "ui_twidgetstudent.h"
#include "tmyicondelegate.h"
#include <QTabBar>
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

    // 数据库连接,前面登陆界面已经连接过了，不用再连接，直接拿就可以
    DB=QSqlDatabase::database();
}

TWidgetStudent::~TWidgetStudent()
{
    delete ui;
}

// 个人信息界面初始化
void TWidgetStudent::iniTabPerson()
{
    QSqlQuery *query=new QSqlQuery(DB);
    query->prepare("SELECT login.username, login.password, login.icon, login.debt, login.number FROM login WHERE login.id = :ID");
    query->bindValue(":ID",id);
    bool ok=query->exec();
    if(!ok){
        QMessageBox::critical(this,"错误","信息初始化失败:"+query->lastError().text());
        delete query;
        return ;
    }else{
        query->next();
        QString Tpwd=query->value("password").toString();
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

        this->pwd=Tpwd;
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
}

// 信息修改界面初始化
void TWidgetStudent::iniTabMessage()
{
    ui->btnStackedPagMessage->click();
}

//图书查询界面初始化
void TWidgetStudent::iniTabQuery()
{

}

//信息修改界面——信息修改
void TWidgetStudent::iniStackedPagMessage()
{
    //初始化数据

    ui->lineMessageName->setText(this->name);
    ui->lineMessageNumber->setText(QString::number(this->number));
    QPixmap map;
    map.loadFromData(this->icon);
    ui->labelMessageIcon->setPixmap(map.scaled(300,300,Qt::KeepAspectRatio,Qt::SmoothTransformation));

}

//信息修改界面——密码修改
void TWidgetStudent::iniStackedPagPassWord()
{
    ui->lineOriginPwd->clear();
    ui->lineNewPwd->clear();
    ui->lineAgainPwd->clear();
}

//信息修改界面——还款
void TWidgetStudent::iniStackedPagDebt()
{
    ui->lineDebt->setReadOnly(true);
    ui->lineDebt->setText(QString::number(this->debt));
    ui->btnRepay->setEnabled(this->debt);
}

void TWidgetStudent::on_btnPerson_clicked()
{
    iniTabPerson();
    ui->tabWidget->setCurrentIndex(int(TabWidgetType::Person));
}


void TWidgetStudent::on_btnMessage_clicked()
{
    iniTabMessage();
    ui->tabWidget->setCurrentIndex(int(TabWidgetType::Message));
}

void TWidgetStudent::on_btnquery_clicked()
{
    iniTabQuery();
    ui->tabWidget->setCurrentIndex(int(TabWidgetType::Query));
}
void TWidgetStudent::on_btnStackedPagMessage_clicked()
{
    /**
     * @brief iniStackedPagMessage
     * 初始化信息修改界面,因为信息可能会修改，所以不能写死，需要动态初始化
     */
    iniStackedPagMessage();
    ui->stackedWidget->setCurrentIndex(int(MessageStackWidgetType::Message));
}

void TWidgetStudent::on_btnStackedPagPassword_clicked()
{
    iniStackedPagPassWord();
    ui->stackedWidget->setCurrentIndex(int(MessageStackWidgetType::Password));
}

void TWidgetStudent::on_btnStackedPagDebt_clicked()
{
    iniStackedPagDebt();
    ui->stackedWidget->setCurrentIndex(int(MessageStackWidgetType::Debt));
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
    // QSqlDatabase DB=QSqlDatabase::database();
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


// 提交修改密码
void TWidgetStudent::on_btnCommitPwd_clicked()
{
    // 获取数据
    QString Opwd=ui->lineOriginPwd->text();
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
    QSqlQuery *query=new QSqlQuery(DB);
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

    ui->lineOriginPwd->clear();
    ui->lineNewPwd->clear();
    ui->lineAgainPwd->clear();

    delete query;
    return ;
}

//一键还款
void TWidgetStudent::on_btnRepay_clicked()
{
    QSqlQuery *query=new QSqlQuery(DB);
    query->prepare("UPDATE login"
                   " SET login.debt = :debt"
                   " WHERE login.id = :id");
    query->bindValue(":debt",0);
    query->bindValue(":id",this->id);
    bool ok=query->exec();

    if(ok){
        ui->lineDebt->setText("0");
        QMessageBox::information(this,"提示","还款成功！！！");
        this->debt=0;
    }else{
        QMessageBox::critical(this,"错误","还款失败:"+query->lastError().text());
    }
    delete query;
}




void TWidgetStudent::on_radioCategory_clicked(bool checked)
{
    Q_UNUSED(checked);
    ui->queryStackedWidget->setCurrentIndex(int(QueryStackWidgetType::Category));
}

