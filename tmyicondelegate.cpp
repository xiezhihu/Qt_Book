#include "tmyicondelegate.h"
#include <QLabel>

TMyIconDelegate::TMyIconDelegate(QWidget *parent) {}

void TMyIconDelegate::setEdit(bool edit)
{
    this->edit=edit;
}

QWidget *TMyIconDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(parent);Q_UNUSED(option);
    QLabel *label=new QLabel();
    QByteArray icon=index.data(Qt::DisplayRole).toByteArray();
    QPixmap map;
    map.loadFromData(icon);
    label->setPixmap(map);
    return label;
}

void TMyIconDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    Q_UNUSED(editor);
    QLabel *label=static_cast<QLabel*>(editor);
    QByteArray icon=index.data(Qt::DisplayRole).toByteArray();
    QPixmap map;
    map.loadFromData(icon);
    label->setPixmap(map);
}

void TMyIconDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    Q_UNUSED(editor);
    model->setData(index,index.data(Qt::DisplayRole),Qt::EditRole);
}

void TMyIconDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
