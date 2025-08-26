#ifndef TADDSTUDENTDIALOG_H
#define TADDSTUDENTDIALOG_H

#include <QDialog>

namespace Ui {
class TAddStudentDialog;
}

class TAddStudentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TAddStudentDialog(QWidget *parent = nullptr);
    ~TAddStudentDialog();

private slots:
    void on_btnIcon_clicked();

    void on_btnCommite_clicked();

private:
    Ui::TAddStudentDialog *ui;
};

#endif // TADDSTUDENTDIALOG_H
