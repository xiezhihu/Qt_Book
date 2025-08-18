#include "dialog.h"
#include "twidgetstudent.h"
#include <QApplication>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    Dialog w;
    w.show();
    int res=w.exec();
    if(res==QDialog::Accepted){
        Dialog::Role type=w.roleType();
        int id=w.userId();
        qDebug()<<int(type)<<" "<<id<<" "<<res;


        if(type==Dialog::Role::STUDENT){
            TWidgetStudent *student=new TWidgetStudent(id);
            student->show();

        }else if(type==Dialog::Role::TEACHER){

        }
        qDebug()<<"登陆成功";
    }else if(res==QDialog::Rejected){
        qDebug()<<"登陆失败";
    }
    return a.exec();
}
