#include "src/sessionlistmodel.h"

SessionListModel::SessionListModel(QObject *parent):QStringListModel(parent)
{

}

SessionListModel::SessionListModel(const QStringList &strings, QObject *parent):QStringListModel(strings, parent)
{

}

QVariant SessionListModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::UserRole)
        return vars.at(index.row());
    return QStringListModel::data(index, role);
}

bool SessionListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::UserRole) {
        if (index.row() <= vars.size()) {
            if (index.row() == vars.size()) vars.append(value);
            else vars.replace(index.row(),value);
            QVector<int> roles;
            roles.reserve(3);
            roles.append(Qt::DisplayRole);
            roles.append(Qt::EditRole);
            roles.append(Qt::UserRole);
            emit dataChanged(index, index, roles);
        }
        return false;
    }

    return QStringListModel::setData(index, value, role);
}
