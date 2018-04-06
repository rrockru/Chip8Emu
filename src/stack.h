#ifndef STACK_H
#define STACK_H

#include <QAbstractScrollArea>

#include "cpu.h"
#include "memory.h"

class StackWidget : public QAbstractScrollArea
{
    Q_OBJECT

    int m_charWidth;
    int m_charHeight;

    CPU *cpu;
    Memory *memory;

public slots:
    void redraw();

public:
    StackWidget(QWidget *parent, CPU *cpu, Memory *memory);

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // STACK_H
