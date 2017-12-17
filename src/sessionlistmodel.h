#ifndef SESSIONLISTMODEL_H
#define SESSIONLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QStringListModel>

class SessionListModel : public QStringListModel
{
    Q_OBJECT
public:
    explicit SessionListModel(QObject *parent = Q_NULLPTR);
    explicit SessionListModel(const QStringList &strings, QObject *parent = Q_NULLPTR);

//    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
//    QModelIndex sibling(int row, int column, const QModelIndex &idx) const Q_DECL_OVERRIDE;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

//    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

//    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
//    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;

//    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) Q_DECL_OVERRIDE;

//    QStringList stringList() const;
//    void setStringList(const QStringList &strings);

//    Qt::DropActions supportedDropActions() const Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(SessionListModel)
    // QStringList lst;
    QList<QVariant> vars;
};

#endif // SESSIONLISTMODEL_H
