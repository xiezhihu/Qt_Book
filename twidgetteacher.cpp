#include "twidgetteacher.h"
#include "ui_twidgetteacher.h"
#include "tmaskdelegate.h"
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QStandardItemModel>
#define LOGINMAXCOLOMN 6
#define LOGINMAXROW 1


TWidgetTeacher::TWidgetTeacher(int id,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TWidgetTeacher)
{
    ui->setupUi(this);
    this->id = id;
    DB = QSqlDatabase::database();
    query = new QSqlQuery(DB);

    fun[2] = [this](){do_change();};
    fun[3] = [this](){do_resetPwd();};
    fun[4] = [this](){do_clearDebt();};
    fun[5] = [this](){do_delete();};

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
        QMessageBox::critical(this,"错误","提交修改失败:"+err.text());

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


// 账号管理
void TWidgetTeacher::on_btnManagement_clicked()
{
    query->prepare("SELECT COUNT(*)"
                   " FROM login"
                   " WHERE role = 'STUDENT'");
    bool ok = query->exec();
    if(!ok){
        QMessageBox::critical(this,"错误","初始化失败:"+query->lastError().text());
        return ;
    }
    query->next();
    int rowCount = query->value(0).toInt();
    setSpinPagMax(rowCount);

    ui->spinPag->setValue(1);
    emit ui->spinPag->valueChanged(1); // 触发一下信号，初始化表格

    ui->lineSearch->clear();

    ui->stackedWidget->setCurrentIndex(int(WidgetType::Management));
    ui->radName->setChecked(true);
}


void TWidgetTeacher::setLoginTableView(int pag,QString username, QString number)
{
    if(pag<1) pag = 1;
    int start=(pag-1)*LOGINMAXROW;

    if(!username.isEmpty()){


        QString sql = QString("SELECT username, number, role, id"
                " FROM login"
                " WHERE role = 'STUDENT'"
                " AND username LIKE :username"
                " LIMIT %1,%2").arg(start).arg(LOGINMAXROW);
        query->prepare(sql);
        query->bindValue(":username","%"+username+"%");

    }else if (!number.isEmpty()){
        QString sql = QString("SELECT username, number, role, id"
                      " FROM login"
                      " WHERE role = 'STUDENT'"
                      " AND number = :number"
                      " LIMIT %1,%2").arg(start).arg(LOGINMAXROW);
        query->prepare(sql);
        query->bindValue(":number",number.toLongLong());

    }else{
        QString sql=QString("SELECT username, number, role, id"
                              " FROM login"
                              " WHERE role = 'STUDENT'"
                              " LIMIT %1,%2").arg(start).arg(LOGINMAXROW);
        query->prepare(sql);

    }


    // model/view
    itemModel = new QStandardItemModel(0,LOGINMAXCOLOMN);
    TMaskDelegate *mask= new TMaskDelegate(this);
    ui->loginTableView->setModel(itemModel);
    ui->loginTableView->setItemDelegateForColumn(1,mask);
    QStringList strList;
    strList<<"姓名"<<"学号"<<"修改"<<"重置密码"<<"清除欠款"<<"删除";
    itemModel->setHorizontalHeaderLabels(strList);

    bool ok=query->exec();

    if(!ok){
        QMessageBox::critical(this,"错误","初始化失败:"+query->lastError().text());
        return ;
    }

    QStandardItem *item;
    QList<QStandardItem*> itemList;
    int curRow=0;

    while(query->next()){
        itemList.clear();
        QString TuserName = query->value("userName").toString();
        qint64 Tnumber = query->value("number").toLongLong();
        int Tid = query->value("id").toInt();

        item = new QStandardItem(TuserName);
        itemList.append(item);

        item = new QStandardItem(QString::number(Tnumber));
        itemList.append(item);


        item = new QStandardItem();
        item->setData(Tid,Qt::UserRole+1);
        itemList.append(item);

        item = new QStandardItem();
        item->setData(Tid,Qt::UserRole+1);
        itemList.append(item);

        item = new QStandardItem();
        item->setData(Tid,Qt::UserRole+1);
        itemList.append(item);

        item = new QStandardItem();
        item->setData(Tid,Qt::UserRole+1);
        itemList.append(item);

        itemModel->appendRow(itemList);

        for(int i=2;i<=5;i++){

            QPushButton *btn = new QPushButton(strList[i],ui->loginTableView);
            btn->setFlat(true);
            btn->setStyleSheet("QPushButton { color: blue; background-color: transparent; border: none;}"
                               "QPushButton:hover {font-weight:bold; background-color: lightgray;}");
            btn->setCursor(Qt::PointingHandCursor);
            QModelIndex index = itemModel->index(curRow,i);
            ui->loginTableView->setIndexWidget(index,btn);


            connect(btn,&QPushButton::clicked,this,fun[i]);
        }

        curRow++;
    }
}

void TWidgetTeacher::do_change()
{
    QModelIndex index = ui->loginTableView->currentIndex();
    int curRow = index.row();
    int id = index.data(Qt::UserRole+1).toInt();
    QString TuserName = itemModel->item(curRow,0)->data(Qt::DisplayRole).toString();
    qint64 Tnumber = itemModel->item(curRow,1)->data(Qt::DisplayRole).toLongLong();
    query->prepare("UPDATE login"
                   " SET username = :Tusername, number = :Tnumber"
                   " WHERE id = :id");
    query->bindValue(":Tusername",TuserName);
    query->bindValue(":Tnumber",Tnumber);
    query->bindValue(":id",id);
    bool ok = query->exec();
    if(!ok){
        QMessageBox::critical(this,"错误","修改失败:"+query->lastError().text());
        return ;
    }

    QMessageBox::information(this,"提示","修改成功");
}

void TWidgetTeacher::do_resetPwd()
{
    QModelIndex index = ui->loginTableView->currentIndex();
    int id = index.data(Qt::UserRole+1).toInt();
    query->prepare("UPDATE login"
                   " SET password = '123456'"
                   " WHERE id = :id");
    query->bindValue(":id",id);
    bool ok=query->exec();
    if(!ok){
        QMessageBox::critical(this,"错误","重置失败:"+query->lastError().text());
        return ;
    }

    QMessageBox::information(this,"提示","密码已重置");

}

void TWidgetTeacher::do_clearDebt()
{
    QModelIndex index = ui->loginTableView->currentIndex();
    int id = index.data(Qt::UserRole+1).toInt();
    query->prepare("UPDATE login"
                   " SET debt = 0"
                   " WHERE id = :id");
    query->bindValue(":id",id);
    bool ok=query->exec();
    if(!ok){
        QMessageBox::critical(this,"错误","清除失败:"+query->lastError().text());
        return ;
    }

    QMessageBox::information(this,"提示","清除成功");

}

void TWidgetTeacher::do_delete()
{
    QMessageBox::StandardButton res = QMessageBox::question(this,"警告","是否删除用户？");
    if(res == QMessageBox::StandardButton::No){
        return ;
    }

    QModelIndex index = ui->loginTableView->currentIndex();
    int id = index.data(Qt::UserRole+1).toInt();
    query->prepare("DELETE FROM login"
                   " WHERE id = :id");
    query->bindValue(":id",id);
    bool ok=query->exec();
    if(!ok){
        QMessageBox::critical(this,"错误","删除失败:"+query->lastError().text());
        return ;
    }

    QMessageBox::information(this,"提示","删除成功");
    ui->btnSearch->click();
}

void TWidgetTeacher::setSpinPagMax(int rowCount)
{
    int pags = rowCount/LOGINMAXROW;
    ui->labSum->setText(QString("页,共%1页").arg(rowCount%LOGINMAXROW?pags+1:pags));
    ui->spinPag->setMaximum(rowCount%LOGINMAXROW?pags+1:pags);

}



// 首页
void TWidgetTeacher::on_btnFirst_clicked()
{
    ui->spinPag->setValue(1);
}

// 上一页
void TWidgetTeacher::on_btnLast_clicked()
{
    ui->spinPag->setValue(ui->spinPag->value()-1);
}

// 下一页
void TWidgetTeacher::on_btnNext_clicked()
{
    ui->spinPag->setValue(ui->spinPag->value()+1);
}

// 尾页
void TWidgetTeacher::on_btnEnd_clicked()
{
    ui->spinPag->setValue(ui->spinPag->maximum());
}


void TWidgetTeacher::on_spinPag_valueChanged(int arg1)
{
    ui->btnFirst->setEnabled(arg1!=ui->spinPag->minimum());
    ui->btnLast->setEnabled(arg1!=ui->spinPag->minimum());
    ui->btnNext->setEnabled(arg1!=ui->spinPag->maximum());
    ui->btnEnd->setEnabled(arg1!=ui->spinPag->maximum());
    QString str=ui->lineSearch->text();
    if(ui->radName->isChecked()){
        setLoginTableView(arg1,str);
    }else{

        setLoginTableView(arg1,QString(),str);
    }
}

// 姓名
void TWidgetTeacher::on_radName_clicked()
{
    ui->lineSearch->clear();
    ui->spinPag->setValue(1);

}

// 学号
void TWidgetTeacher::on_radNumber_clicked()
{
    ui->lineSearch->clear();
    ui->spinPag->setValue(1);
}

// 点击搜索
void TWidgetTeacher::on_btnSearch_clicked()
{
    QString str = ui->lineSearch->text();

    if(ui->radName->isChecked()){
        query->prepare("SELECT COUNT(*)"
                       " FROM login"
                       " WHERE role = 'STUDENT'"
                       " AND username LIKE :str");
        query->bindValue(":str","%"+str+"%");
        bool ok=query->exec();
        if(!ok){
            QMessageBox::critical(this,"错误","查询失败:"+query->lastError().text());
            return ;
        }
        query->next();
        int rowCount = query->value(0).toInt();
        setSpinPagMax(rowCount);

        setLoginTableView(ui->spinPag->value(),str);
    }else{

        query->prepare("SELECT COUNT(*)"
                       " FROM login"
                       " WHERE role = 'STUDENT'"
                       " AND number = :str");
        query->bindValue(":str",str.toLongLong());
        bool ok=query->exec();
        if(!ok){
            QMessageBox::critical(this,"错误","查询失败:"+query->lastError().text());
            return ;
        }
        query->next();
        int rowCount = query->value(0).toInt();
        setSpinPagMax(rowCount);
        setLoginTableView(ui->spinPag->value(),QString(),str);
    }
}


void TWidgetTeacher::on_lineSearch_textChanged(const QString &arg1)
{
    ui->btnSearch->click();
}

