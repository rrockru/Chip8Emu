#ifndef CPU_H
#define CPU_H

#include <QObject>

#include "memory.h"

class CPU: public QObject
{
    Q_OBJECT

    char V[16];
    uint16_t I;
    uint16_t SP;
    uint16_t PC;

    Memory *memory;

public slots:
    void tick();

public:
    CPU(Memory *);
};

#endif // CPU_H
