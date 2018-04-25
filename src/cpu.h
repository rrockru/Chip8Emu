#ifndef CPU_H
#define CPU_H

#include <QThread>

#include "memory.h"
#include "keyboard.h"
#include "timer.h"

class CPU: public QThread
{
    Q_OBJECT

    uint8_t V[16];
    uint16_t I;
    uint16_t SP;
    uint16_t PC;

    Timer *delayTimer;
    Timer *soundTimer;

    Memory *memory;
    Keyboard *keyboard;

    bool stopFlag;

    QList<int> breakpoints;

signals:
    void error(int op, int addr);
    void draw();
    void tick(int pc);
    void finished();
    void breakpoint();

public slots:
    void onTick();
    void onReset();
    void onStop();

public:
    CPU(Memory *memory, Keyboard *keyboard);

    void run() override;
    int getVRegister(int num) { if ((num >= 0) && (num <= 0xF)) return V[num]; else return 0; }
    int getIRegister() { return I; }
    int getSPRegister() { return SP; }
    int getPCRegister() { return PC; }

    void addBreakpoint(int addr) { if (!breakpoints.contains(addr)) breakpoints.append(addr); }
    void removeBreakpoint(int addr) { if (breakpoints.contains(addr)) breakpoints.removeOne(addr); }
    bool isBreakpointSets(int addr) { return breakpoints.contains(addr); }
    QList<int> getBreakpoints() { return breakpoints; }
    void cleanBreakpoints() { breakpoints.clear(); }

};

#endif // CPU_H
