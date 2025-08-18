#ifndef TWIDGETSTUDENT_H
#define TWIDGETSTUDENT_H

#include <QWidget>


namespace Ui {
class TWidgetStudent;
}

class TWidgetStudent : public QWidget
{
    Q_OBJECT
private:
    QString name;
    qint64 number;
    int debt;
    QByteArray icon;
private:
    int id;
    void iniTab0();
    void iniTab1();

    void iniStackedPag0();

public:
    explicit TWidgetStudent(int ID,QWidget *parent = nullptr);
    ~TWidgetStudent();
private slots:
    void on_btnPerson_clicked();

    void on_btnMessage_clicked();

    void on_btnStackedPag0_clicked();

    void on_btnMessageSetIcon_clicked();

    void on_btnMessageSave_clicked();

private:
    Ui::TWidgetStudent *ui;
};

#endif // TWIDGETSTUDENT_H
