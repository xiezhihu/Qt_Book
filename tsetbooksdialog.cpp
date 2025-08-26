#include "tsetbooksdialog.h"
#include "ui_tsetbooksdialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QPixmap>

void TSetBooksDialog::seleBook()
{
    query->prepare("SELECT image_data, title, author, press, category"
                   " FROM books"
                   " WHERE id = :id");
    query->bindValue(":id",this->id);
    bool ok = query->exec();
    if(!ok){
        QMessageBox::critical(this,"错误","查询错误:"+query->lastError().text());
        return ;
    }
    query->next();

    QByteArray icon = query->value("image_data").toByteArray();
    QString name = query->value("title").toString();
    QString author = query->value("author").toString();
    QString press = query->value("press").toString();
    QString category = query->value("category").toString();

    QPixmap map;
    map.loadFromData(icon);
    QIcon Ticon(map.scaled(200,200,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->btnIcon->setIcon(Ticon);
    ui->lineName->setText(name);
    ui->lineAuthor->setText(author);
    ui->linePress->setText(press);
    ui->comboBox->setCurrentText(category);
}

TSetBooksDialog::TSetBooksDialog(int id,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TSetBooksDialog)
{
    ui->setupUi(this);
    this->id =  id;
    DB = QSqlDatabase::database();
    query = new QSqlQuery(DB);

    QStringList strList;
    strList << "言情"
            << "青春文学"
            << "旅游"
            << "保健"
            << "管理"
            << "中国古诗词"
            << "科幻"
            << "历史"
            << "心理学"
            << "计算机"
            << "自然科学"
            << "法律"
            << "美食"
            << "体育"
            << "经济";
    ui->comboBox->addItems(strList);
    seleBook();
    ui->btnIcon->setFixedSize(200,200);
    ui->btnIcon->setIconSize(ui->btnIcon->size());
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setModal(true);
    this->adjustSize();
    this->setFixedSize(this->size());

}

TSetBooksDialog::~TSetBooksDialog()
{
    qDebug()<<ui->btnIcon->width()<<" "<<ui->btnIcon->height();
    delete ui;
}

void TSetBooksDialog::on_btnIcon_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,"选择图片",QCoreApplication::applicationFilePath(),"图片(*.jpg *.png)");
    if(fileName.isEmpty())return ;
    QIcon Ticon(fileName);
    ui->btnIcon->setIcon(Ticon);
}


void TSetBooksDialog::on_btnSave_clicked()
{
    QPixmap map = ui->btnIcon->icon().pixmap(200,200);
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    map.save(&buffer, "PNG");   // 也可以 "JPG"

    QString name = ui->lineName->text();
    QString author = ui->lineAuthor->text();
    QString press = ui->linePress->text();
    QString category = ui->comboBox->currentText();

    query->prepare("UPDATE books"
                   " SET image_data = :byteArray, title = :name, author = :author, press = :press, category = :category"
                   " WHERE id = :id");
    query->bindValue(":byteArray",byteArray);
    query->bindValue(":name",name);
    query->bindValue(":author",author);
    query->bindValue(":press",press);
    query->bindValue(":category",category);
    query->bindValue(":id",id);
    bool ok = query->exec();

    if(!ok){
        QMessageBox::critical(this,"错误","修改失败:"+query->lastError().text());
        return ;
    }
    QMessageBox::information(this,"提示","修改成功！！！");
    this->accept();

}

