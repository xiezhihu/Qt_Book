#include "tmaskdelegate.h"
#include <QLineEdit>

TMaskDelegate::TMaskDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

QWidget *TMaskDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit *editor = new QLineEdit(parent);
    editor->setInputMask("0000000000;_"); // 年-月-日
    return editor;
}
