#ifndef TMYICONDELEGATE_H
#define TMYICONDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class TMyIconDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    TMyIconDelegate(QWidget *parent = nullptr);


    // QAbstractItemDelegate interface
public:


    // QAbstractItemDelegate interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    // QAbstractItemDelegate interface
public:
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};




#endif // TMYICONDELEGATE_H
