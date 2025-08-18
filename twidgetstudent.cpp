#include "twidgetstudent.h"
#include "ui_twidgetstudent.h"
#include "tmyicondelegate.h"
#include <QTabBar>
#include <QtSql>
#include <QMessageBox>
#include <QDebug>

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


void TWidgetStudent::iniTab0()
{
    // 数据库连接,前面登陆界面已经连接过了，不用再连接，直接拿就可以
    QSqlDatabase DB=QSqlDatabase::database();
    QSqlQuery *query=new QSqlQuery(DB);
    query->prepare("SELECT login.username, login.icon, login.debt FROM login WHERE login.id = :ID");
    query->bindValue(":ID",id);
    bool ok=query->exec();
    if(!ok){
        QMessageBox::critical(this,"错误","信息初始化失败:"+query->lastError().text());
        return ;
    }else{
        query->next();
        QString name=query->value("username").toString();
        QByteArray iconData=query->value("icon").toByteArray();
        int debt=query->value("debt").toInt();
        ui->labName->setText("姓名:"+name);
        ui->labid->setText("ID:"+QString::number(id));
        ui->labDebt->setText("欠款:"+QString::number(debt));
        QPixmap map;
        map.loadFromData(iconData);
        ui->labIcon->setPixmap(map);
    }





    //model/view
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


    //8.17 设置完代理，图片不显示
    TMyIconDelegate *delegate=new TMyIconDelegate(ui->tableView);
    delegate->setEdit(false);
    ui->tableView->setItemDelegateForColumn(rec.indexOf("image"),delegate);



    ui->tableView->setModel(queryModel);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
    ui->tableView->setColumnHidden(rec.indexOf("id"),true);//设置隐藏
    ui->tableView->setColumnHidden(rec.indexOf("book_id"),true);//设置隐藏

    ui->tabWidget->setCurrentIndex(0);
}

void TWidgetStudent::on_btnPerson_clicked()
{
    iniTab0();
}

