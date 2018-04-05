#ifndef REGISTERS_H
#define REGISTERS_H

#include <QAbstractTableModel>

class RegistersWidget : public QAbstractTableModel
{
public:
    RegistersWidget();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // REGISTERS_H
