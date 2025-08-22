#include "querydetaildialog.h"
#include "ui_querydetaildialog.h"
#include <QMessageBox>
#define BORROWMAXCOUNT 5


QueryDetailDialog::QueryDetailDialog(int bookID,int userID,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::QueryDetailDialog)
{
    ui->setupUi(this);
    this->bookID=bookID;
    this->userID=userID;
    DB=QSqlDatabase::database();
    query = new QSqlQuery(DB);
    iniDialog();
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setModal(true);
    this->adjustSize();
    this->setFixedSize(this->size());

}

QueryDetailDialog::~QueryDetailDialog()
{
    delete ui;
}


void QueryDetailDialog::iniDialog()
{
    QByteArray Ticon;
    QString TbookName,Tauthor,Tpress,Tcategory;
    int Tsum,TborrowCount;
    query->prepare("SELECT id, title, image_data, author, press, total_count, borrow_count, category"
                   " FROM books"
                   " WHERE id = :bookID");
    query->bindValue(":bookID",bookID);
    bool ok=query->exec();
    if(ok){
        query->next();
        Ticon=query->value("image_data").toByteArray();
        TbookName=query->value("title").toString();
        Tauthor=query->value("author").toString();
        Tpress=query->value("press").toString();
        Tcategory=query->value("category").toString();
        Tsum=query->value("total_count").toInt();
        TborrowCount=query->value("borrow_count").toInt();


        QPixmap map;
        map.loadFromData(Ticon);
        ui->labIcon->setPixmap(map.scaled(200,200,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        ui->labBookName->setText("书名:"+TbookName);
        ui->labAuthor->setText("作者:"+Tauthor);
        ui->labPress->setText("出版社:"+Tpress);
        ui->labCategory->setText("分类:"+Tcategory);
        ui->labSum->setText("总藏书:"+QString::number(Tsum));
        ui->labBorrowCount->setText("剩余:"+QString::number(TborrowCount));

        ui->btnBorrow->setEnabled(TborrowCount!=0);
    }else{
        QMessageBox::critical(this,"错误","查询失败:"+query->lastError().text());
        this->close();
    }

}



void QueryDetailDialog::on_btnBorrow_clicked()
{
    query->prepare("SELECT COUNT(*)"
                " FROM borrow"
                " WHERE user_id = :userID");
    query->bindValue(":userID",userID);
    bool ok=query->exec();
    if(ok){
        query->next();
        int userBorrowCount=query->value(0).toInt();
        if(userBorrowCount>=BORROWMAXCOUNT){
            QMessageBox::information(this,"提示","您借阅数量已达上限！！！\n请先还书");
            return ;
        }
    }else{
        QMessageBox::critical(this,"错误","查询失败:"+query->lastError().text());
        return ;
    }



    // 借阅未达上限
    QMessageBox::StandardButton res=QMessageBox::question(this,"提示","借阅需支付3元,是否借阅？");
    if(res==QMessageBox::StandardButton::Yes){
        DB.transaction();// 开启事务

        // 更新欠款
        query->prepare("UPDATE login"
                       " SET debt = debt+3"
                       " WHERE id = :userID");
        query->bindValue(":userID",userID);
        bool upDebt=query->exec();

        // 更新借阅数量
        query->prepare("UPDATE books"
                       " SET borrow_count = borrow_count-1"
                       " WHERE id = :bookID");
        query->bindValue(":bookID",bookID);
        bool upBorrowCount=query->exec();

        // 插入借阅记录
        QDateTime borrowTime=QDateTime::currentDateTime();
        QDateTime returnTime=borrowTime.addDays(3);
        query->prepare("INSERT INTO borrow (user_id, book_id, borrow_date, return_date, status)"
                       " VALUES (:userID, :bookID, :borrowDate, :returnDate, 'NORMAL')");
        query->bindValue(":userID",userID);
        query->bindValue(":bookID",bookID);
        query->bindValue(":borrowDate",borrowTime);
        query->bindValue(":returnDate",returnTime);
        bool isInsert=query->exec();

        if(upDebt && upBorrowCount && isInsert){
            DB.commit();
            iniDialog(); // 更新面板
            QMessageBox::information(this,"提示","借阅成功！！！");
            this->accept(); // 接受
        }else{
            QMessageBox::information(this,"提示","数据更新失败");
            DB.rollback();
        }
    }


}

