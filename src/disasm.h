#ifndef DISASM_H
#define DISASM_H

#include <QTextEdit>

#include "memory.h"

class DisasmWidget : public QTextEdit
{
    Q_OBJECT

    int m_charWidth;
    int m_charHeight;

    int m_posAddr;
    int m_posHex;
    int m_posDisasm;

    Memory *memory;

public slots:
    void highlightCurrentLine(int pc);
    void cleanHighlight();

public:
    DisasmWidget(QWidget *parent, Memory *memory);
    void Disasm();

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // DISASM_H
