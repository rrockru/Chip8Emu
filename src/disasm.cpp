#include "disasm.h"

#include <QDebug>

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

    currentLine = -1;
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
        case 0x0: {
            switch (op >> 4) {
            case 0xE: {
                switch (op & 0xF) {
                case 0x0: {
                    append(QString("%1 CLS")
                           .arg(addrOp).toUpper());
                    break;
                }
                case 0xE: {
                    append(QString("%1 RET")
                           .arg(addrOp).toUpper());
                    break;
                }
                }
                break;
            }
            default: {
                append(QString("%1 CALL #%2")
                       .arg(addrOp)
                       .arg(op & 0xFFF, 3, 16, QChar('0')).toUpper());
                break;
            }
            }
            break;
        }
        case 0x1: {
            append(QString("%1 JP #%2")
                   .arg(addrOp)
                   .arg(op & 0xFFF, 3, 16, QChar('0')).toUpper());
            break;
        }
        case 0x2: {
            append(QString("%1 CALL #%2")
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
        case 0x4: {
            append(QString("%1 SNE V%2, #%3")
                   .arg(addrOp)
                   .arg((op >> 8) & 0xF, 1, 16)
                   .arg(op & 0xFF, 2, 16, QChar('0')).toUpper());
            break;
        }
        case 0x5: {
            append(QString("%1 SE V%2, V%3")
                   .arg(addrOp)
                   .arg((op >> 8) & 0xF, 1, 16)
                   .arg((op >> 4) & 0xF, 1, 16).toUpper());
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
        case 0x8: {
            switch (op & 0xF) {
            case 0x0: {
                append(QString("%1 LD V%2, V%3")
                       .arg(addrOp)
                       .arg((op >> 8) & 0xF, 1, 16)
                       .arg((op >> 4) & 0xF, 1, 16).toUpper());
                break;
            }
            case 0x1: {
                append(QString("%1 OR V%2, V%3")
                       .arg(addrOp)
                       .arg((op >> 8) & 0xF, 1, 16)
                       .arg((op >> 4) & 0xF, 1, 16).toUpper());
                break;
            }
            case 0x2: {
                append(QString("%1 AND V%2, V%3")
                       .arg(addrOp)
                       .arg((op >> 8) & 0xF, 1, 16)
                       .arg((op >> 4) & 0xF, 1, 16).toUpper());
                break;
            }
            case 0x3: {
                append(QString("%1 XOR V%2, V%3")
                       .arg(addrOp)
                       .arg((op >> 8) & 0xF, 1, 16)
                       .arg((op >> 4) & 0xF, 1, 16).toUpper());
                break;
            }
            case 0x4: {
                append(QString("%1 ADD V%2, V%3")
                       .arg(addrOp)
                       .arg((op >> 8) & 0xF, 1, 16)
                       .arg((op >> 4) & 0xF, 1, 16).toUpper());
                break;
            }
            case 0x5: {
                append(QString("%1 SUB V%2, V%3")
                       .arg(addrOp)
                       .arg((op >> 8) & 0xF, 1, 16)
                       .arg((op >> 4) & 0xF, 1, 16).toUpper());
                break;
            }
            case 0x6: {
                append(QString("%1 SHR V%2, V%3")
                       .arg(addrOp)
                       .arg((op >> 8) & 0xF, 1, 16)
                       .arg((op >> 4) & 0xF, 1, 16).toUpper());
                break;
            }
            case 0x7: {
                append(QString("%1 SUBN V%2, V%3")
                       .arg(addrOp)
                       .arg((op >> 8) & 0xF, 1, 16)
                       .arg((op >> 4) & 0xF, 1, 16).toUpper());
                break;
            }
            case 0xE: {
                append(QString("%1 SHL V%2, V%3")
                       .arg(addrOp)
                       .arg((op >> 8) & 0xF, 1, 16)
                       .arg((op >> 4) & 0xF, 1, 16).toUpper());
                break;
            }
            default:
                break;
            }
            break;
        }
        case 0x9: {
            append(QString("%1 SNE V%2, V%3")
                   .arg(addrOp)
                   .arg((op >> 8) & 0xF, 1, 16)
                   .arg((op >> 4) & 0xF, 1, 16).toUpper());
            break;
        }
        case 0xA: {
            append(QString("%1 LD I, #%2")
                   .arg(addrOp)
                   .arg(op & 0xFFF, 3, 16, QChar('0')).toUpper());
            break;
        }
        case 0xB: {
            append(QString("%1 JP V0, #%2")
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
            append(QString("%1 DRW V%2, V%3, %4")
                   .arg(addrOp)
                   .arg((op >> 8) & 0xF, 1, 16)
                   .arg((op >> 4) & 0xF, 1, 16)
                   .arg(op & 0xF).toUpper());
            break;
        }
        case 0xE: {
            switch (op & 0xFF) {
            case 0x9E: {
                append(QString("%1 SKP V%2")
                       .arg(addrOp)
                       .arg((op >> 8) & 0xF, 1, 16).toUpper());
                break;
            }
            case 0xA1: {
                append(QString("%1 SKNP V%2")
                       .arg(addrOp)
                       .arg((op >> 8) & 0xF, 1, 16).toUpper());
                break;
            }
            default:
                break;
            }
            break;
        }
        default: {
            append(QString("%1 ???")
                   .arg(addrOp).toUpper());
            break;
        }
        }
    }
    update();
    setCurrentCommand(0x200);
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

void DisasmWidget::setCurrentCommand(int pc)
{
    currentCommand = (pc - 0x200) / 2;

    highLight();
}

void DisasmWidget::mousePressEvent(QMouseEvent *e)
{
    QTextEdit::mousePressEvent(e);
    QTextCursor cursor = cursorForPosition(e->pos());

    currentLine = cursor.blockNumber();

    highLight();
}

void DisasmWidget::highLight()
{
    QList<QTextEdit::ExtraSelection> extSelections;

    QTextEdit::ExtraSelection currentCommandSelection;
    QColor currentCommandLineColor = QColor(Qt::yellow).lighter(160);

    QTextEdit::ExtraSelection currentLineSelection;
    QColor currentLineColor = QColor(Qt::cyan).lighter(160);
    currentLineSelection.format.setBackground(currentLineColor);
    currentLineSelection.format.setProperty(QTextFormat::FullWidthSelection, true);
    currentLineSelection.cursor = QTextCursor(document()->findBlockByLineNumber(currentLine));
    currentLineSelection.cursor.clearSelection();
    extSelections.append(currentLineSelection);

    currentCommandSelection.format.setBackground(currentCommandLineColor);
    currentCommandSelection.format.setProperty(QTextFormat::FullWidthSelection, true);
    currentCommandSelection.cursor = QTextCursor(document()->findBlockByLineNumber(currentCommand));
    currentCommandSelection.cursor.clearSelection();
    extSelections.append(currentCommandSelection);

    setExtraSelections(extSelections);
}

int DisasmWidget::GetCurrentLineAddr()
{
    QTextCursor cursor = QTextCursor(document()->findBlockByLineNumber(currentLine));
    cursor.select(QTextCursor::WordUnderCursor);
    QString code = cursor.selectedText();

    if (code.length() > 0) {
        int addr = code.mid(0, 3).toInt(nullptr, 16);
        return addr;
    }

    return 0;
}
