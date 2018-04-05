#include "registers.h"

RegistersWidget::RegistersWidget()
{
    emit dataChanged(createIndex(0, 0), createIndex(19, 0));
}

int RegistersWidget::rowCount(const QModelIndex &) const
{
    return 19;
}

int RegistersWidget::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant RegistersWidget::data(const QModelIndex &index, int role) const
{
    int row = index.row();

    switch(role) {
    case Qt::DisplayRole:
        if (row < 16) {
            //return QString("%1").arg(V[row], 2, 16, QChar('0'));
        }
        switch (row) {
        case 16:
            //return QString("%1").arg(I, 4, 16, QChar('0'));
            break;
        case 17:
            //return QString("%1").arg(SP, 4, 16, QChar('0'));
            break;
        case 18:
            //return QString("%1").arg(PC, 4, 16, QChar('0'));
            break;
        }
        break;
    }
    return QVariant();
}

QVariant RegistersWidget::headerData(int section, Qt::Orientation, int role) const
{
    switch(role) {
    case Qt::DisplayRole:
        if (section < 16) {
            return QString("V%1").arg(section, 1, 16).toUpper();
        }
        switch (section) {
        case 16:
            return "I";
            break;
        case 17:
            return "SP";
            break;
        case 18:
            return "PC";
            break;
        }
        break;
    }
    return QVariant();
}
