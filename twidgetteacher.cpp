#include "twidgetteacher.h"
#include "ui_twidgetteacher.h"
#include "tmaskdelegate.h"
#include "taddstudentdialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QInputDialog>
#define LOGINMAXCOLOMN 6
#define LOGINMAXROW 2
#define QUERYMAXROW 15
#define QUERYMAXCOLUMN 8


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

    fun[101] = [this](){do_addBookSum();};
    fun[102] = [this](){do_SetBook();};
    fun[103] = [this](){do_deleleBook();};

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
void TWidgetTeacher::on_btnCommitPwd_clicked()
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
        item->setEnabled(false);
        itemList.append(item);

        item = new QStandardItem(QString::number(Tnumber));
        item->setEnabled(false);
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
    emit ui->spinPag->valueChanged(ui->spinPag->value());

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

// 添加学生
void TWidgetTeacher::on_btnAddStudent_clicked()
{
    TAddStudentDialog *dialog = new TAddStudentDialog(ui->loginTableView);
    dialog->show();
    if(dialog->exec()==QDialog::Accepted){
        ui->btnSearch->click();
    }
}




// 图书管理
void TWidgetTeacher::on_btnBook_clicked()
{


    ui->stackedWidget->setCurrentIndex(2);
    ui->radioAuthor->setChecked(true);
    ui->btnSearchBook->click();
}


void TWidgetTeacher::setQueryTabModel(int pag){
    if(pag<1)pag = 1;
    int start=(pag-1)*QUERYMAXROW;
    QString sql;
    /**
     * 1.获取行数，设置spinBox最大值
     * 2.设置显示数据
     */
    if(!currAuthor.isEmpty()){

        sql=QString("SELECT books.title, books.author, books.press, books.total_count, books.borrow_count, books.id"
                      " FROM books"
                      " WHERE books.author LIKE :author"
                      " LIMIT %1,%2").arg(start).arg(QUERYMAXROW);
        query->prepare(sql);
        query->bindValue(":author","%"+currAuthor+"%");


    }else if(!currBookName.isEmpty()){

        sql=QString("SELECT books.title, books.author, books.press, books.total_count, books.borrow_count, books.id"
                      " FROM books"
                      " WHERE books.title LIKE :bookName"
                      " LIMIT %1,%2").arg(start).arg(QUERYMAXROW);
        query->prepare(sql);
        query->bindValue(":bookName","%"+currBookName+"%");


    }else if(!currCategory.isEmpty()){

        sql=QString("SELECT books.title, books.author, books.press, books.total_count, books.borrow_count, books.category, books.id"
                      " FROM books"
                      " WHERE books.category = :category"
                      " LIMIT %1,%2").arg(start).arg(QUERYMAXROW);
        query->prepare(sql);
        query->bindValue(":category",currCategory);

    }else{

        sql=QString("SELECT books.title, books.author, books.press, books.total_count, books.borrow_count, books.id"
                      " FROM books"
                      " LIMIT %1,%2").arg(start).arg(QUERYMAXROW);
        query->prepare(sql);

    }




    //model/view  8.22完成了图书查询的详情
    QStandardItemModel *queryItemModel = new QStandardItemModel(0,QUERYMAXCOLUMN,ui->queryTabView);
    QStringList strList;
    strList<<"书名"<<"作者"<<"出版社"<<"总数"<<"可借阅数"<<"增加库存"<<"修改图书"<<"删除图书";
    queryItemModel->setHorizontalHeaderLabels(strList);
    ui->queryTabView->setModel(queryItemModel);


    bool ok=query->exec();
    if(!ok){
        QMessageBox::critical(this,"错误","查询失败:"+query->lastError().text());
        return ;
    }

    QList<QStandardItem*> itemList;
    QStandardItem *item;
    int curRow=0;
    while(query->next()){

        int Tbookid=query->value("id").toInt();
        QString TbookName=query->value("title").toString();
        QString TbookAuthor=query->value("author").toString();
        QString TbookPress=query->value("press").toString();
        int Tsum=query->value("total_count").toInt();
        int TborrowCount=query->value("borrow_count").toInt();

        itemList.clear();

        item = new QStandardItem;
        item->setText(TbookName);
        itemList<<item;

        item = new QStandardItem;
        item->setText(TbookAuthor);
        itemList<<item;

        item = new QStandardItem;
        item->setText(TbookPress);
        itemList<<item;

        item = new QStandardItem;
        item->setText(QString::number(Tsum));
        item->setEnabled(false);
        itemList<<item;

        item = new QStandardItem;
        item->setText(QString::number(TborrowCount));
        item->setEnabled(false);
        itemList<<item;

        queryItemModel->appendRow(itemList);

        for(int i=QUERYMAXCOLUMN-3;i<QUERYMAXCOLUMN;i++){
            QPushButton *btnDelegate =new QPushButton(strList[i],ui->queryTabView);
            btnDelegate->setFlat(true);
            btnDelegate->setStyleSheet("QPushButton { color: blue; background-color: transparent; border: none;}"
                                       "QPushButton:hover {font-weight:bold; background-color: lightgray;}");
            btnDelegate->setCursor(Qt::PointingHandCursor);

            QModelIndex index=queryItemModel->index(curRow,i);
            queryItemModel->setData(index,Tbookid,Qt::UserRole+1);
            ui->queryTabView->setIndexWidget(index,btnDelegate);

            connect(btnDelegate,&QPushButton::clicked,this,fun[96+i]);
        }

        curRow++;


    }

    ui->queryTabView->resizeColumnsToContents();
    ui->queryTabView->resizeRowsToContents();
}

// 增加库存
void TWidgetTeacher::do_addBookSum()
{
    int add = QInputDialog::getInt(ui->queryTabView,"增加库存","输入增加的数量:",0,0,INT_MAX,1);
    if(!add) return ;

    QModelIndex index = ui->queryTabView->currentIndex();
    int id = index.data(Qt::UserRole+1).toInt();

    query->prepare("UPDATE books"
                   " SET total_count = total_count + :add, borrow_count = borrow_count + :add"
                   " WHERE id = :id");
    query->bindValue(":add",add);
    query->bindValue(":id",id);
    bool ok = query->exec();
    if(!ok){
        QMessageBox::critical(this,"错误","增加失败:"+query->lastError().text());
        return ;
    }
    QMessageBox::information(this,"提示","增加成功");
    setQueryTabModel(ui->spinPagBook->value());
}

void TWidgetTeacher::do_SetBook(){


}

// 删除书本
void TWidgetTeacher::do_deleleBook(){
    QMessageBox::StandardButton res = QMessageBox::question(this,"警告","会来连同借阅表记录一起删除。\n是否要删除图书？");
    if(res == QMessageBox::StandardButton::No)return ;

    QModelIndex index = ui->queryTabView->currentIndex();
    int id = index.data(Qt::UserRole+1).toInt();

    DB.transaction();
    // 删除书本
    query->prepare("DELETE FROM books"
                   " WHERE id = :id");
    query->bindValue(":id",id);
    bool isDeleteBook = query->exec();

    // 删除借阅记录
    query->prepare("DELETE FROM borrow"
                   " WHERE book_id = :id");
    query->bindValue(":id",id);
    bool isDeleteBorrow = query->exec();

    if(isDeleteBook && isDeleteBorrow){
        DB.commit();
        QMessageBox::information(this,"提示","删除成功");
        setQueryTabModel(ui->spinPagBook->value());
    }else{
        DB.rollback();
        QMessageBox::critical(this,"错误","删除失败:"+query->lastError().text());

    }

}

// 设置spinBox
void TWidgetTeacher::setSpinBox(int rowCount)
{
    int pags=rowCount/QUERYMAXROW;
    ui->labSumBook->setText(QString("页,共%1页").arg(rowCount%QUERYMAXROW?pags+1:pags));
    ui->spinPagBook->setMaximum(rowCount%QUERYMAXROW?pags+1:pags);
}



void TWidgetTeacher::on_radioAuthor_clicked(bool checked)
{
    if(checked){
        currAuthor.clear();
        currBookName.clear();
        currCategory.clear();
        ui->lineSearchBook->clear();
        ui->lineSearchBook->setEnabled(true);
        ui->btnSearchBook->setEnabled(true);
        ui->queryStackedWidget->setCurrentIndex(int(QueryStackWidgetType::ShowData));
        ui->btnSearchBook->click();
    }
}


void TWidgetTeacher::on_radioName_clicked(bool checked)
{
    if(checked){
        currAuthor.clear();
        currBookName.clear();
        currCategory.clear();
        ui->lineSearchBook->clear();
        ui->lineSearchBook->setEnabled(true);
        ui->btnSearchBook->setEnabled(true);
        ui->queryStackedWidget->setCurrentIndex(int(QueryStackWidgetType::ShowData));
        ui->btnSearchBook->click();
    }

}


void TWidgetTeacher::on_radioCategory_clicked(bool checked)
{
    if(checked){
        currAuthor.clear();
        currBookName.clear();
        currCategory.clear();
        ui->lineSearchBook->clear();
        ui->lineSearchBook->setEnabled(false);
        ui->btnSearchBook->setEnabled(false);
        ui->queryStackedWidget->setCurrentIndex(int(QueryStackWidgetType::Category));

        connect(ui->btnComputer,&QPushButton::clicked,this,&TWidgetTeacher::do_AllPushBUtton);
        connect(ui->btnEconomy,&QPushButton::clicked,this,&TWidgetTeacher::do_AllPushBUtton);
        connect(ui->btnFood,&QPushButton::clicked,this,&TWidgetTeacher::do_AllPushBUtton);
        connect(ui->btnHealth,&QPushButton::clicked,this,&TWidgetTeacher::do_AllPushBUtton);
        connect(ui->btnHistory,&QPushButton::clicked,this,&TWidgetTeacher::do_AllPushBUtton);
        connect(ui->btnLaw,&QPushButton::clicked,this,&TWidgetTeacher::do_AllPushBUtton);
        connect(ui->btnManagement,&QPushButton::clicked,this,&TWidgetTeacher::do_AllPushBUtton);
        connect(ui->btnPoetry,&QPushButton::clicked,this,&TWidgetTeacher::do_AllPushBUtton);
        connect(ui->btnPsychology,&QPushButton::clicked,this,&TWidgetTeacher::do_AllPushBUtton);
        connect(ui->btnRomance,&QPushButton::clicked,this,&TWidgetTeacher::do_AllPushBUtton);
        connect(ui->btnSciFi,&QPushButton::clicked,this,&TWidgetTeacher::do_AllPushBUtton);
        connect(ui->btnScience,&QPushButton::clicked,this,&TWidgetTeacher::do_AllPushBUtton);
        connect(ui->btnSports,&QPushButton::clicked,this,&TWidgetTeacher::do_AllPushBUtton);
        connect(ui->btnTravel,&QPushButton::clicked,this,&TWidgetTeacher::do_AllPushBUtton);
        connect(ui->btnYouth,&QPushButton::clicked,this,&TWidgetTeacher::do_AllPushBUtton);

    }
}

// 分类通用槽函数
void TWidgetTeacher::do_AllPushBUtton()
{
    QPushButton *btn = dynamic_cast<QPushButton*>(this->sender());
    if(!btn) return ;

    currCategory=btn->text();

    query->prepare("SELECT COUNT(*) "
                   " FROM books"
                   " WHERE books.category = :category");
    query->bindValue(":category",currCategory);
    bool ok=query->exec();
    if(!ok){
        QMessageBox::critical(this,"错误","查询失败:"+query->lastError().text());
        return ;
    }
    query->next();
    int rowCount=query->value(0).toInt();
    setSpinBox(rowCount);

    if(!rowCount) return ;


    ui->spinPagBook->setValue(1);
    emit ui->spinPagBook->valueChanged(1);
    setQueryTabModel(ui->spinPagBook->value());
    ui->queryStackedWidget->setCurrentIndex(int(QueryStackWidgetType::ShowData));
}


// spinPagBook
void TWidgetTeacher::on_spinPagBook_valueChanged(int arg1)
{
    ui->btnFirstBook->setEnabled(arg1!=ui->spinPagBook->minimum() && arg1!=1);// 对应没有数据和只有一页数据
    ui->btnLastBook->setEnabled(arg1!=ui->spinPagBook->minimum() && arg1!=1);
    ui->btnEndBook->setEnabled(arg1!=ui->spinPagBook->maximum());
    ui->btnNextBook->setEnabled(arg1!=ui->spinPagBook->maximum());
    qDebug()<<arg1;

    //更新数据
    setQueryTabModel(arg1);
}

// 首页
void TWidgetTeacher::on_btnFirstBook_clicked()
{
    ui->spinPagBook->setValue(1);
}

// 上一页
void TWidgetTeacher::on_btnLastBook_clicked()
{
    int curPag=ui->spinPagBook->value();
    ui->spinPagBook->setValue(curPag-1);
}

// 下一页
void TWidgetTeacher::on_btnNextBook_clicked()
{
    int curPag=ui->spinPagBook->value();
    ui->spinPagBook->setValue(curPag+1);
}

//尾页
void TWidgetTeacher::on_btnEndBook_clicked()
{
    ui->spinPagBook->setValue(ui->spinPagBook->maximum());
}

// 搜索
void TWidgetTeacher::on_btnSearchBook_clicked()
{
    QString str = ui->lineSearchBook->text();

    if(str.isEmpty()){ // 三者为空等价于当前搜索框为空

        currBookName.clear();
        currCategory.clear();
        currAuthor.clear();
        query->exec("SELECT COUNT(*) FROM books");
        if(query->lastError().isValid()){
            QMessageBox::critical(this,"错误","查询失败:"+query->lastError().text());
            return ;
        }
        query->next();
        int rowCount=query->value(0).toInt();
        setSpinBox(rowCount);

        if(!rowCount) return ;

        ui->spinPagBook->setValue(1);
        emit ui->spinPagBook->valueChanged(1);
        setQueryTabModel(ui->spinPagBook->value());
        return ;
    }

    if(ui->radioAuthor->isChecked()){

        currAuthor = str;

        query->prepare("SELECT COUNT(*) "
                       " FROM books"
                       " WHERE books.author LIKE :author");
        query->bindValue(":author","%"+currAuthor+"%");
        bool ok=query->exec();
        if(!ok){
            QMessageBox::critical(this,"错误","查询失败:"+query->lastError().text());
            return ;
        }
        query->next();
        int rowCount=query->value(0).toInt();
        setSpinBox(rowCount);

        if(!rowCount) return ;


        currBookName.clear();
        currCategory.clear();
        ui->spinPagBook->setValue(1);
        emit ui->spinPagBook->valueChanged(1);
        setQueryTabModel(ui->spinPagBook->value());


    }else if(ui->radioName->isChecked()){
        currAuthor.clear();
        currBookName=str;

        query->prepare("SELECT COUNT(*) "
                       " FROM books"
                       " WHERE books.title LIKE :bookName");
        query->bindValue(":bookName","%"+currBookName+"%");
        bool ok=query->exec();
        if(!ok){
            QMessageBox::critical(this,"错误","查询失败:"+query->lastError().text());
            return ;
        }
        query->next();
        int rowCount=query->value(0).toInt();
        setSpinBox(rowCount);

        if(!rowCount) return ;


        currCategory.clear();
        ui->spinPagBook->setValue(1);
        emit ui->spinPagBook->valueChanged(1);
        setQueryTabModel(ui->spinPagBook->value());
    }

}

// 当lineSearch变化时，及时改变内容，提高灵敏
void TWidgetTeacher::on_lineSearchBook_textChanged(const QString &arg1)
{
    ui->btnSearchBook->click();
}



