#ifndef CPU_H
#define CPU_H

#include <QAbstractTableModel>
#include <QTimer>

#include "memory.h"

class CPU: public QAbstractTableModel
{
    Q_OBJECT

    uint8_t V[16];
    uint16_t I;
    uint16_t SP;
    uint16_t PC;

    QTimer *cpuTimer;

    Memory *memory;

    void tick();

public slots:
    void onRun();
    void onStep();
    void onReset();

signals:
    void error(uint16_t op, uint16_t addr);
    void draw();
    void tickSignal(uint16_t pc);

public:
    CPU(QObject *parent, Memory *);

    void Reset();
    bool isRun() { return cpuTimer->isActive(); }

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // CPU_H
