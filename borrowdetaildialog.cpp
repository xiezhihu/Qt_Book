#include "borrowdetaildialog.h"
#include "ui_borrowdetaildialog.h"
#include <QMessageBox>
#include <QtSql>
#include <QTimer>

BorrowDetailDialog::BorrowDetailDialog(int borrow_id,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BorrowDetailDialog)
{
    ui->setupUi(this);
    QByteArray Ticon;
    QString TbookName,Tauthor,Tpress,Tcategory,TborrowTime,TreturnTime,Tstatus;

    // 查询
    QSqlDatabase DB=QSqlDatabase::database();
    QSqlQuery *query=new QSqlQuery(DB);
    query->prepare("SELECT borrow.borrow_id, borrow.book_id, borrow.borrow_date, borrow.return_date, borrow.status,"
                   " books.id, books.title, books.image_data, books.author, books.press, books.category"
                   " FROM borrow"
                   " JOIN books ON borrow.book_id = books.id"
                   " WHERE borrow.borrow_id = :Tborrow_id");
    query->bindValue(":Tborrow_id",borrow_id);
    bool ok=query->exec();
    if(!ok){
        QMessageBox::critical(this,"错误","查询失败:"+query->lastError().text());
    }else {
        query->next();
        Ticon=query->value("image_data").toByteArray();
        TbookName=query->value("title").toString();
        Tauthor=query->value("author").toString();
        Tpress=query->value("press").toString();
        Tcategory=query->value("category").toString();
        TborrowTime=query->value("borrow_date").toDateTime().toString("yyyy-MM-dd hh:mm:ss");
        TreturnTime=query->value("return_date").toDateTime().toString("yyyy-MM-dd hh:mm:ss");
        Tstatus=query->value("status").toString();
    }

    //初始化组件
    QPixmap map;
    map.loadFromData(Ticon);
    ui->labIcon->setPixmap(map.scaled(200,200,Qt::KeepAspectRatio,Qt::SmoothTransformation));

    ui->labBookName->setText("书名:"+TbookName);
    ui->labAuthor->setText("作者:"+Tauthor);
    ui->labPress->setText("出版社:"+Tpress);
    ui->labCategory->setText("分类:"+Tcategory);
    ui->labBorroeTime->setText("借阅时间:"+TborrowTime);
    ui->labReturnTime->setText("预计还书时间:"+TreturnTime);
    QString text=Tstatus=="NORMAL"?"正常":"逾期";
    ui->labStatus->setText("状态:"+text);


    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setModal(true);
    this->setFixedSize(1165,367);
}

BorrowDetailDialog::~BorrowDetailDialog()
{
    delete ui;
}
