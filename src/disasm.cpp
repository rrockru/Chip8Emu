#include "disasm.h"

#include <QPainter>
#include <QPaintEvent>
#include <QScrollBar>
#include <QTextBlock>
#include <QTextCursor>

const int GAP = 8;

DisasmWidget::DisasmWidget(QWidget *parent, Memory *memory):
    QTextEdit(parent),
    memory(memory)
{
    setReadOnly(true);
    setTextInteractionFlags(Qt::NoTextInteraction);

    setFont(QFont("Courier", 10));

    m_charWidth = fontMetrics().width(QLatin1Char('9'));
    m_charHeight = fontMetrics().height();

    m_posAddr = 0;
    m_posHex = m_charWidth * 3 + GAP;
    m_posDisasm = m_posHex + m_charWidth * 5 + GAP;
}

void DisasmWidget::Disasm()
{
    clear();
    for (int pc = 0x200; pc < 0x200 + memory->getRomSize(); pc += 2) {
        uint16_t op = memory->getRamWord(pc);
        QString addrOp = QString("%1 %2 %3")
                .arg(pc, 3, 16)
                .arg(op >> 8, 2, 16, QChar('0'))
                .arg(op & 0xFF, 2, 16, QChar('0'));
        switch(op >> 12) {
        case 0x1: {
            append(QString("%1 JP #%2")
                   .arg(addrOp)
                   .arg(op & 0xFFF, 3, 16, QChar('0')).toUpper());
            break;
        }
        case 0x3: {
            append(QString("%1 SE V%2, #%3")
                   .arg(addrOp)
                   .arg((op >> 8) & 0xF, 1, 16)
                   .arg(op & 0xFF, 2, 16, QChar('0')).toUpper());
            break;
        }
        case 0x6: {
            append(QString("%1 LD V%2, #%3")
                   .arg(addrOp)
                   .arg((op >> 8) & 0xF, 1, 16)
                   .arg(op & 0xFF, 2, 16, QChar('0')).toUpper());
            break;
        }
        case 0x7: {
            append(QString("%1 ADD V%2, #%3")
                   .arg(addrOp)
                   .arg((op >> 8) & 0xF, 1, 16)
                   .arg(op & 0xFF, 2, 16, QChar('0')).toUpper());
            break;
        }
        case 0xA: {
            append(QString("%1 LD I, #%2")
                   .arg(addrOp)
                   .arg(op & 0xFFF, 3, 16, QChar('0')).toUpper());
            break;
        }
        case 0xC: {
            append(QString("%1 RND V%2, #%3")
                   .arg(addrOp)
                   .arg((op >> 8) & 0xF, 1, 16)
                   .arg(op & 0xFF, 2, 16, QChar('0')).toUpper());
            break;
        }
        case 0xD: {
            append(QString("%1 DRAW V%2, V%3, %4")
                   .arg(addrOp)
                   .arg((op >> 8) & 0xF, 1, 16)
                   .arg((op >> 4) & 0xF, 1, 16)
                   .arg(op & 0xF).toUpper());
            break;
        }
        default: {
            append(QString("%1 Unknown instruction #%2!")
                   .arg(addrOp)
                   .arg(op, 4, 16, QChar('0')));
            break;
        }
        }
    }
    update();
    highlightCurrentLine(0x200);
}

void DisasmWidget::paintEvent(QPaintEvent *event)
{
    QTextEdit::paintEvent(event);

    QPainter painter(viewport());
    QSize areaSize = viewport()->size();
    verticalScrollBar()->setPageStep(areaSize.height() / m_charHeight);

    painter.setPen(Qt::gray);
    int addrLinePos = m_posHex;
    int disasmLinePos = m_posDisasm;
    painter.drawLine(addrLinePos, event->rect().top(), addrLinePos, height());
    painter.drawLine(disasmLinePos, event->rect().top(), disasmLinePos, height());
    painter.setPen(Qt::black);
}

void DisasmWidget::highlightCurrentLine(int pc)
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    QTextEdit::ExtraSelection selection;
    QColor lineColor = QColor(Qt::yellow).lighter(160);

    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    //selection.cursor = document()->find(QRegExp(QString("^%1 ").arg(pc, 3, 16).toUpper()));
    selection.cursor = QTextCursor(document()->findBlockByLineNumber((pc - 0x200) / 2));
    selection.cursor.clearSelection();
    extraSelections.append(selection);

    setExtraSelections(extraSelections);

    setTextCursor(selection.cursor);
}

void DisasmWidget::cleanHighlight()
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    setExtraSelections(extraSelections);
}
