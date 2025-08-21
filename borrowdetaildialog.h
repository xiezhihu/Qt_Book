#ifndef BORROWDETAILDIALOG_H
#define BORROWDETAILDIALOG_H

#include <QDialog>
#include <QByteArray>

namespace Ui {
class BorrowDetailDialog;
}

class BorrowDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BorrowDetailDialog(int Tborrow_id,QWidget *parent = nullptr);
    ~BorrowDetailDialog();

private:
    Ui::BorrowDetailDialog *ui;
};

#endif // BORROWDETAILDIALOG_H
