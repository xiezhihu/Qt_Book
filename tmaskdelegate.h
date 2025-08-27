#ifndef TMASKDELEGATE_H
#define TMASKDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class TMaskDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TMaskDelegate(QObject *parent = nullptr);

    // QAbstractItemDelegate interface
public:
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};


#endif // TMASKDELEGATE_H
