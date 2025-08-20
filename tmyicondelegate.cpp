#include "tmyicondelegate.h"
#include <QLabel>
#include <QPainter>

TMyIconDelegate::TMyIconDelegate(QWidget *parent) {}

void TMyIconDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!index.data(Qt::DisplayRole).isNull()){
        QByteArray iconData=index.data().toByteArray();
        QPixmap map;
        map.loadFromData(iconData);
        painter->drawPixmap(option.rect,map.scaled(option.rect.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    }else{
        QStyledItemDelegate::paint(painter,option,index);
    }
}


QSize TMyIconDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QByteArray iconData=index.data(Qt::DisplayRole).toByteArray();
    QPixmap map;
    map.loadFromData(iconData);
    return map.size();
}
