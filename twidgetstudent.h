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
    int id;
    void iniTab0();

public:
    explicit TWidgetStudent(int ID,QWidget *parent = nullptr);
    ~TWidgetStudent();
private slots:
    void on_btnPerson_clicked();

private:
    Ui::TWidgetStudent *ui;
};

#endif // TWIDGETSTUDENT_H
