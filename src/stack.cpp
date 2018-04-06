#include "stack.h"

#include <QPainter>

const int START_GAP = 15;

StackWidget::StackWidget(QWidget *parent, CPU *cpu, Memory *memory):
    QAbstractScrollArea(parent),
    cpu(cpu),
    memory(memory)
{
    setFont(QFont("Courier", 10));

    m_charWidth = fontMetrics().width(QLatin1Char('9'));
    m_charHeight = fontMetrics().height();
}

void StackWidget::paintEvent(QPaintEvent *event)
{
    QAbstractScrollArea::paintEvent(event);

    if (cpu->isRunning()) return;

    QPainter painter(viewport());

    for (int i = cpu->getSPRegister() - 2; i >= 0; i -= 2) {
        QString line = QString("%1: #%2")
                .arg(i, 2, 16, QChar('0'))
                .arg(memory->getRamWord(0xEFE - i), 3, 16, QChar('0')).toUpper();
        painter.drawText(START_GAP, START_GAP + i * m_charHeight, line);
    }
}

void StackWidget::redraw()
{
    viewport()->update();
}
