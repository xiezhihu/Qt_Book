#include "twidgetstudent.h"
#include "ui_twidgetstudent.h"
#include "borrowdetaildialog.h"
#include "querydetaildialog.h"
#include <QTabBar>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTimer>
#define BORROWMAXCOLUMN 7
#define QUERYMAXROW 15
#define QUERYMAXCOLUMN 6


TWidgetStudent::TWidgetStudent(int ID,QWidget *parent)
    : QWidget(parent),id(ID)
    , ui(new Ui::TWidgetStudent)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->tabWidget->tabBar()->hide(); //隐藏标签
    // 数据库连接,前面登陆界面已经连接过了，不用再连接，直接拿就可以
    DB=QSqlDatabase::database();
    query=new QSqlQuery(DB);
    iconDelegate =new TMyIconDelegate(this);


    // 实时更新数据库借阅状态
    timer=new QTimer(this);
    timer->setTimerType(Qt::CoarseTimer);
    timer->start(1000);
    connect(timer,&QTimer::timeout,this,&TWidgetStudent::do_timeOut);


    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->btnPerson->click();
}

TWidgetStudent::~TWidgetStudent()
{
    delete ui;
}


// 及时更新数据库
void TWidgetStudent::do_timeOut()
{
    QDateTime curTime=QDateTime::currentDateTime();

    bool ok=query->exec("SELECT borrow.borrow_id, borrow.return_date, borrow.status"
                          " FROM borrow");
    if(!ok){
        QMessageBox::critical(this,"错误","数据库查询失败:"+query->lastError().text());
    }

    while(query->next()){
        int Tborrow_id=query->value("borrow_id").toInt();
        QString curStatus=query->value("status").toString();
        QDateTime TreturnTime=query->value("return_date").toDateTime();
        if(curStatus!="TIMEOUT" && curTime>TreturnTime){
            query->prepare("UPDATE borrow"
                           " SET status = 'TIMEOUT' "
                           " WHERE borrow_id = :Tborrow_id");
            query->bindValue(":Tborrow_id",Tborrow_id);
            ok=query->exec();
            if(!ok){
                QMessageBox::critical(this,"错误","数据库更新失败:"+query->lastError().text());
            }else{
                iniTabPerson();
                qDebug()<<"数据库更新成功";
            }

        }
    }


}


// 个人信息界面初始化
void TWidgetStudent::iniTabPerson()
{
    query->prepare("SELECT login.username, login.password, login.icon, login.debt, login.number FROM login WHERE login.id = :ID");
    query->bindValue(":ID",id);
    bool ok=query->exec();
    if(!ok){
        QMessageBox::critical(this,"错误","信息初始化失败:"+query->lastError().text());
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

        this->pwd=Tpwd; // 密码
        this->name=Tname; // 用户名
        this->number=Tnumber; // 学号
        this->debt=Tdebt; // 欠款
        this->icon=TiconData; // 头像
    }


    //query
    query->prepare(R"(
    SELECT login.id, borrow.book_id, borrow.borrow_id, borrow.status, books.image_data,  books.title, books.author, borrow.borrow_date, borrow.return_date
    FROM login
    JOIN borrow ON login.id = borrow.user_id
    JOIN books ON borrow.book_id = books.id
    WHERE login.id = :id
    )");
    query->bindValue(":id",this->id);
    ok=query->exec();

    if(!ok){
        QMessageBox::critical(this,"错误","查询失败:"+query->lastError().text());
        return ;
    }


    //model/view

    QStandardItemModel *borrowItemModel = new QStandardItemModel(0,BORROWMAXCOLUMN,ui->tableView);

    QStringList strList;
    strList<<"封面"<<"书名"<<"作者"<<"借阅日期"<<"预计还书日期"<<"状态"<<"详情信息";
    borrowItemModel->setHorizontalHeaderLabels(strList);
    ui->tableView->setModel(borrowItemModel);
    ui->tableView->setItemDelegateForColumn(0,iconDelegate);

    // 设置数据
    QStandardItem *item;
    QList<QStandardItem*> itemList;
    int curRow=0;
    while(query->next()){
        QPushButton *btnDelegate =new QPushButton("详情",ui->tableView);
        btnDelegate->setFlat(true);
        btnDelegate->setStyleSheet("QPushButton { color: blue; background-color: transparent; border: none;}"
                                   "QPushButton:hover {font-weight:bold; background-color: lightgray;}");

        btnDelegate->setCursor(Qt::PointingHandCursor);


        QByteArray Ticon=query->value("image_data").toByteArray();
        QString TbookName=query->value("title").toString();
        QString Tauthor=query->value("author").toString();
        QString TborrowTime=query->value("borrow_date").toDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString TreturnTime=query->value("return_date").toDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString Tstatus=query->value("status").toString();
        int Tborrow_id=query->value("borrow_id").toInt();



        itemList.clear();

        item=new QStandardItem();
        item->setData(Ticon,Qt::DisplayRole);
        itemList.append(item);

        item=new QStandardItem();
        item->setText(TbookName);
        itemList.append(item);

        item=new QStandardItem();
        item->setText(Tauthor);
        itemList.append(item);

        item=new QStandardItem();
        item->setText(TborrowTime);
        itemList.append(item);

        item=new QStandardItem();
        item->setText(TreturnTime);
        itemList.append(item);

        item=new QStandardItem();
        QString text=Tstatus=="NORMAL"?"正常":"逾期";
        item->setText(text);
        itemList.append(item);

        borrowItemModel->appendRow(itemList);

        QModelIndex index=borrowItemModel->index(curRow,BORROWMAXCOLUMN-1);
        borrowItemModel->setData(index,Tborrow_id,Qt::UserRole+1);
        ui->tableView->setIndexWidget(index,btnDelegate);
        curRow++;


        connect(btnDelegate,&QPushButton::clicked,this,[this]()->void{
            QModelIndex index=ui->tableView->currentIndex();
            int Tborrow_id=index.data(Qt::UserRole+1).toInt();
            qDebug()<<Tborrow_id;
            BorrowDetailDialog *borrowDetail=new BorrowDetailDialog(Tborrow_id,this);
            borrowDetail->show();
            if(borrowDetail->exec()==QDialog::Accepted){
                iniTabPerson();
            }
        });
    }
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();


}

// 个人中心
void TWidgetStudent::on_btnPerson_clicked()
{
    iniTabPerson();
    ui->tabWidget->setCurrentIndex(int(TabWidgetType::Person));
}

// 修改信息
void TWidgetStudent::on_btnMessage_clicked()
{
    /**
     * 刚开始界面就是个人信息界面，直接触发个人信息点击即可
     */
    ui->btnStackedPagMessage->click();
    ui->tabWidget->setCurrentIndex(int(TabWidgetType::Message));
}

//图书查询
void TWidgetStudent::on_btnquery_clicked()
{

    ui->tabWidget->setCurrentIndex(int(TabWidgetType::Query));
    ui->radioAuthor->setChecked(true);
    ui->btnSearch->click();

}


// 点击信息修改界面的信息修改
void TWidgetStudent::on_btnStackedPagMessage_clicked()
{
    /**
     * @brief iniStackedPagMessage
     * 初始化信息修改界面,因为信息可能会修改，所以不能写死，需要动态初始化
     */
    ui->lineMessageName->setText(this->name);
    ui->lineMessageNumber->setText(QString::number(this->number));
    QPixmap map;
    map.loadFromData(this->icon);
    ui->labelMessageIcon->setPixmap(map.scaled(300,300,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->stackedWidget->setCurrentIndex(int(MessageStackWidgetType::Message));
}

//点击信息修改界面的密码修改
void TWidgetStudent::on_btnStackedPagPassword_clicked()
{
    ui->lineOriginPwd->clear();
    ui->lineNewPwd->clear();
    ui->lineAgainPwd->clear();
    ui->stackedWidget->setCurrentIndex(int(MessageStackWidgetType::Password));
}

// 点击信息修改界面的欠款
void TWidgetStudent::on_btnStackedPagDebt_clicked()
{
    ui->lineDebt->setReadOnly(true);
    ui->lineDebt->setText(QString::number(this->debt));
    ui->btnRepay->setEnabled(this->debt);
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
    if(ui->lineMessageNumber->text().length()<10 || ui->lineMessageNumber->text()[0]=='0'){
        QMessageBox::information(this,"提示","学号格式有误！！！");
        return ;
    }
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

    return ;
}

//一键还款
void TWidgetStudent::on_btnRepay_clicked()
{

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

}

void TWidgetStudent::setQueryTabModel(int pag)
{
    if(pag<1)pag = 1;
    int start=(pag-1)*QUERYMAXROW;
    QString sql;
    /**
     * 1.获取行数，设置spinBox最大值
     * 2.设置显示数据
     */
    if(!currAuthor.isEmpty()){

        sql=QString("SELECT books.image_data, books.title, books.author, books.total_count, books.borrow_count, books.id"
                    " FROM books"
                    " WHERE books.author LIKE :author"
                    " LIMIT %1,%2").arg(start).arg(QUERYMAXROW);
        query->prepare(sql);
        query->bindValue(":author","%"+currAuthor+"%");


    }else if(!currBookName.isEmpty()){

        sql=QString("SELECT books.image_data, books.title, books.author, books.total_count, books.borrow_count, books.id"
                      " FROM books"
                      " WHERE books.title LIKE :bookName"
                      " LIMIT %1,%2").arg(start).arg(QUERYMAXROW);
        query->prepare(sql);
        query->bindValue(":bookName","%"+currBookName+"%");


    }else if(!currCategory.isEmpty()){

        sql=QString("SELECT books.image_data, books.title, books.author, books.total_count, books.borrow_count, books.category, books.id"
                      " FROM books"
                      " WHERE books.category = :category"
                      " LIMIT %1,%2").arg(start).arg(QUERYMAXROW);
        query->prepare(sql);
        query->bindValue(":category",currCategory);

    }else{

        sql=QString("SELECT books.image_data, books.title, books.author, books.total_count, books.borrow_count, books.id"
                    " FROM books"
                    " LIMIT %1,%2").arg(start).arg(QUERYMAXROW);
        query->prepare(sql);

    }




    //model/view  8.22完成了图书查询的详情
    QStandardItemModel *queryItemModel = new QStandardItemModel(0,QUERYMAXCOLUMN,ui->queryTabView);
    QStringList strList;
    strList<<"封面"<<"书名"<<"作者"<<"总数"<<"可借阅数"<<"详细信息";
    queryItemModel->setHorizontalHeaderLabels(strList);
    ui->queryTabView->setModel(queryItemModel);
    ui->queryTabView->setItemDelegateForColumn(0,iconDelegate);

    bool ok=query->exec();
    if(!ok){
        QMessageBox::critical(this,"错误","查询失败:"+query->lastError().text());
        return ;
    }

    QList<QStandardItem*> itemList;
    QStandardItem *item;
    int curRow=0;
    while(query->next()){

        QPushButton *btnDelegate =new QPushButton("详情",ui->tableView);
        btnDelegate->setFlat(true);
        btnDelegate->setStyleSheet("QPushButton { color: blue; background-color: transparent; border: none;}"
                                   "QPushButton:hover {font-weight:bold; background-color: lightgray;}");
        btnDelegate->setCursor(Qt::PointingHandCursor);

        int Tbookid=query->value("id").toInt();
        QByteArray TbookIcon=query->value("image_data").toByteArray();
        QString TbookName=query->value("title").toString();
        QString TbookAuthor=query->value("author").toString();
        int Tsum=query->value("total_count").toInt();
        int TborrowCount=query->value("borrow_count").toInt();

        itemList.clear();

        item = new QStandardItem;
        item->setData(TbookIcon,Qt::DisplayRole);
        item->setEnabled(false);
        itemList<<item;

        item = new QStandardItem;
        item->setText(TbookName);
        item->setEnabled(false);
        itemList<<item;

        item = new QStandardItem;
        item->setText(TbookAuthor);
        item->setEnabled(false);
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

        QModelIndex index=queryItemModel->index(curRow,QUERYMAXCOLUMN-1);
        queryItemModel->setData(index,Tbookid,Qt::UserRole+1);
        ui->queryTabView->setIndexWidget(index,btnDelegate);
        curRow++;

        connect(btnDelegate,&QPushButton::clicked,this,[=]()->void{
            QModelIndex index=ui->queryTabView->currentIndex();
            int TbookID=index.data(Qt::UserRole+1).toInt();
            QueryDetailDialog *detailDialog = new QueryDetailDialog(TbookID,this->id,ui->queryTabView);
            if(detailDialog->exec()==QDialog::Accepted){
                setQueryTabModel(pag);
            }
        });
    }

    ui->queryTabView->resizeColumnsToContents();
    ui->queryTabView->resizeRowsToContents();

}


// 设置spinBox
void TWidgetStudent::setSpinBox(int rowCount)
{
    int pags=rowCount/QUERYMAXROW;
    ui->labSum->setText(QString("页,共%1页").arg(rowCount%QUERYMAXROW?pags+1:pags));
    ui->spinPag->setMaximum(rowCount%QUERYMAXROW?pags+1:pags);

}



// 作者
void TWidgetStudent::on_radioAuthor_clicked(bool checked)
{
    if(checked){
        currAuthor.clear();
        currBookName.clear();
        currCategory.clear();
        ui->lineSearch->clear();
        ui->lineSearch->setEnabled(true);
        ui->btnSearch->setEnabled(true);
        ui->queryStackedWidget->setCurrentIndex(int(QueryStackWidgetType::ShowData));
        ui->btnSearch->click();
    }
}

// 书名
void TWidgetStudent::on_radioName_clicked(bool checked)
{
    if(checked){
        currAuthor.clear();
        currBookName.clear();
        currCategory.clear();
        ui->lineSearch->clear();
        ui->lineSearch->setEnabled(true);
        ui->btnSearch->setEnabled(true);
        ui->queryStackedWidget->setCurrentIndex(int(QueryStackWidgetType::ShowData));
        ui->btnSearch->click();
    }

}


//分类
void TWidgetStudent::on_radioCategory_clicked(bool checked)
{
    if(checked){
        currAuthor.clear();
        currBookName.clear();
        currCategory.clear();
        ui->lineSearch->clear();
        ui->lineSearch->setEnabled(false);
        ui->btnSearch->setEnabled(false);
        ui->queryStackedWidget->setCurrentIndex(int(QueryStackWidgetType::Category));

        connect(ui->btnComputer,&QPushButton::clicked,this,&TWidgetStudent::do_AllPushBUtton);
        connect(ui->btnEconomy,&QPushButton::clicked,this,&TWidgetStudent::do_AllPushBUtton);
        connect(ui->btnFood,&QPushButton::clicked,this,&TWidgetStudent::do_AllPushBUtton);
        connect(ui->btnHealth,&QPushButton::clicked,this,&TWidgetStudent::do_AllPushBUtton);
        connect(ui->btnHistory,&QPushButton::clicked,this,&TWidgetStudent::do_AllPushBUtton);
        connect(ui->btnLaw,&QPushButton::clicked,this,&TWidgetStudent::do_AllPushBUtton);
        connect(ui->btnManagement,&QPushButton::clicked,this,&TWidgetStudent::do_AllPushBUtton);
        connect(ui->btnPoetry,&QPushButton::clicked,this,&TWidgetStudent::do_AllPushBUtton);
        connect(ui->btnPsychology,&QPushButton::clicked,this,&TWidgetStudent::do_AllPushBUtton);
        connect(ui->btnRomance,&QPushButton::clicked,this,&TWidgetStudent::do_AllPushBUtton);
        connect(ui->btnSciFi,&QPushButton::clicked,this,&TWidgetStudent::do_AllPushBUtton);
        connect(ui->btnScience,&QPushButton::clicked,this,&TWidgetStudent::do_AllPushBUtton);
        connect(ui->btnSports,&QPushButton::clicked,this,&TWidgetStudent::do_AllPushBUtton);
        connect(ui->btnTravel,&QPushButton::clicked,this,&TWidgetStudent::do_AllPushBUtton);
        connect(ui->btnYouth,&QPushButton::clicked,this,&TWidgetStudent::do_AllPushBUtton);

    }
}


//spinBox
void TWidgetStudent::on_spinPag_valueChanged(int arg1)
{
    ui->btnFirst->setEnabled(arg1!=ui->spinPag->minimum() && arg1!=1);// 对应没有数据和只有一页数据
    ui->btnLast->setEnabled(arg1!=ui->spinPag->minimum() && arg1!=1);
    ui->btnEnd->setEnabled(arg1!=ui->spinPag->maximum());
    ui->btnNext->setEnabled(arg1!=ui->spinPag->maximum());
    qDebug()<<arg1;

    //更新数据
    setQueryTabModel(arg1);
}

// 点击上一页
void TWidgetStudent::on_btnLast_clicked()
{
    int curPag=ui->spinPag->value();
    ui->spinPag->setValue(curPag-1);
}

// 点击首页
void TWidgetStudent::on_btnFirst_clicked()
{
    ui->spinPag->setValue(1);
}

// 点击下一页
void TWidgetStudent::on_btnNext_clicked()
{
    int curPag=ui->spinPag->value();
    ui->spinPag->setValue(curPag+1);
}

// 点击尾页
void TWidgetStudent::on_btnEnd_clicked()
{

    ui->spinPag->setValue(ui->spinPag->maximum());
}


// 搜索
void TWidgetStudent::on_btnSearch_clicked()
{

    QString str = ui->lineSearch->text();

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

        ui->spinPag->setValue(1);
        emit ui->spinPag->valueChanged(1);
        setQueryTabModel(ui->spinPag->value());
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
        ui->spinPag->setValue(1);
        emit ui->spinPag->valueChanged(1);
        setQueryTabModel(ui->spinPag->value());


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
        ui->spinPag->setValue(1);
        emit ui->spinPag->valueChanged(1);
        setQueryTabModel(ui->spinPag->value());
    }


}

// 当lineSearch变化时，及时改变内容，提高灵敏
void TWidgetStudent::on_lineSearch_textChanged(const QString &arg1)
{
    ui->btnSearch->click();

}

// 分类通用槽函数
void TWidgetStudent::do_AllPushBUtton()
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


    ui->spinPag->setValue(1);
    emit ui->spinPag->valueChanged(1);
    setQueryTabModel(ui->spinPag->value());
    ui->queryStackedWidget->setCurrentIndex(int(QueryStackWidgetType::ShowData));
}



void TWidgetStudent::on_btnQuit_clicked()
{
    QMessageBox::StandardButton res=QMessageBox::question(this,"提示","确认退出？");
    if(res==QMessageBox::StandardButton::Yes){
        this->close();
    }

}

