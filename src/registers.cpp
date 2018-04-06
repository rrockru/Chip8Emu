#include "registers.h"

#include <QPainter>

const int START_GAP = 15;

RegistersWidget::RegistersWidget(QWidget *parent, CPU *cpu):
    QAbstractScrollArea(parent),
    cpu(cpu)
{
    setFont(QFont("Courier", 10));

    m_charWidth = fontMetrics().width(QLatin1Char('9'));
    m_charHeight = fontMetrics().height();

    setMaximumHeight(START_GAP + m_charHeight * 7);
}


void RegistersWidget::paintEvent(QPaintEvent *event)
{
    QAbstractScrollArea::paintEvent(event);

    if (cpu->isRunning()) return;

    QPainter painter(viewport());

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            QString reg = QString("V%1: #%2").arg(y * 4 + x, 1, 16, QChar('0')).arg(cpu->getVRegister(y * 4 + x), 2, 16, QChar('0')).toUpper();
            painter.drawText(START_GAP + x * m_charWidth * 10, START_GAP + y * m_charHeight, reg);
        }
    }
    QString reg = QString("I: #%2").arg(cpu->getIRegister(), 3, 16, QChar('0')).toUpper();
    painter.drawText(START_GAP, START_GAP + m_charHeight * 5, reg);
    reg = QString("SP: #%2").arg(cpu->getSPRegister(), 3, 16, QChar('0')).toUpper();
    painter.drawText(START_GAP + m_charWidth * 10, START_GAP + m_charHeight * 5, reg);
    reg = QString("PC: #%2").arg(cpu->getPCRegister(), 3, 16, QChar('0')).toUpper();
    painter.drawText(START_GAP + m_charWidth * 20, START_GAP + m_charHeight * 5, reg);
}

void RegistersWidget::redraw()
{
    viewport()->update();
}
