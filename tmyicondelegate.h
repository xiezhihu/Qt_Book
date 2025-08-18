#ifndef TMYICONDELEGATE_H
#define TMYICONDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class TMyIconDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    TMyIconDelegate(QWidget *parent = nullptr);
    void setEdit(bool edit=false);

    // QAbstractItemDelegate interface
public:
    bool edit=false;
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};


#endif // TMYICONDELEGATE_H
