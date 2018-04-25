#ifndef DISASM_H
#define DISASM_H

#include <QTextEdit>

#include "cpu.h"
#include "memory.h"

class DisasmWidget : public QTextEdit
{
    Q_OBJECT

    int m_charWidth;
    int m_charHeight;

    int m_posAddr;
    int m_posHex;
    int m_posDisasm;

    int currentLine;
    int currentAddr;

    int align;

    CPU *cpu;
    Memory *memory;

public slots:
    void setCurrentCommand(int pc);
    void highLight();

public:
    DisasmWidget(QWidget *parent, CPU *cpu, Memory *memory);
    void Disasm();
    int GetCurrentLineAddr();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *e);
};

#endif // DISASM_H
