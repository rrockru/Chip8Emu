#ifndef REGISTERS_H
#define REGISTERS_H

#include <QAbstractScrollArea>

#include "cpu.h"

class RegistersWidget : public QAbstractScrollArea
{
    Q_OBJECT

    CPU *cpu;

    int m_charWidth;
    int m_charHeight;

public slots:
    void redraw();

public:
    RegistersWidget(QWidget *parent, CPU *cpu);

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // REGISTERS_H
