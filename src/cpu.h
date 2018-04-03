#ifndef CPU_H
#define CPU_H

#include <QObject>
#include <QAbstractTableModel>

#include "memory.h"

class CPU: public QAbstractTableModel
{
    Q_OBJECT

    uint8_t V[16];
    uint16_t I;
    uint16_t SP;
    uint16_t PC;

    Memory *memory;

public slots:
    void tick();

signals:
    void error(uint16_t op, uint16_t addr);
    void draw();

public:
    CPU(QObject *parent, Memory *);

    void Reset();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // CPU_H
