#ifndef CPU_H
#define CPU_H

#include <QThread>

#include "memory.h"

class CPU: public QThread
{
    Q_OBJECT

    uint8_t V[16];
    uint16_t I;
    uint16_t SP;
    uint16_t PC;

    Memory *memory;

    bool stopFlag;

signals:
    void error(int op, int addr);
    void draw();
    void tick(int pc);
    void finished();

public slots:
    void onTick();
    void onReset();
    void onStop();

public:
    CPU(Memory *);

    void run() override;
    int getVRegister(int num) { if ((num >= 0) && (num <= 0xF)) return V[num]; else return 0; }
    int getIRegister() { return I; }
    int getSPRegister() { return SP; }
    int getPCRegister() { return PC; }

};

#endif // CPU_H
