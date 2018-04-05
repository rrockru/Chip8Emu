#include "hexview.h"

#include <QPainter>
#include <QPaintEvent>
#include <QScrollBar>

const int HEXCHARS_IN_LINE = 51;
const int GAP_ADR_HEX = 10;
const int GAP_HEX_END = 10;
const int BYTES_PER_LINE = 16;

HexViewWidget::HexViewWidget(QWidget *parent, Memory *memory):
    QAbstractScrollArea(parent),
    memory(memory)
{
    setFont(QFont("Courier", 10));

    m_charWidth = fontMetrics().width(QLatin1Char('9'));
    m_charHeight = fontMetrics().height();

    m_posAddr = 0;
    m_posHex = m_charWidth * 3 + GAP_ADR_HEX;

    setMinimumWidth(m_posHex + HEXCHARS_IN_LINE * m_charWidth);
}

QSize HexViewWidget::FullSize() const
{
    if(!memory->getRam()->size())
        return QSize(0, 0);

    int width = m_posHex + HEXCHARS_IN_LINE * m_charWidth + GAP_HEX_END;
    int height = memory->getRam()->size() / BYTES_PER_LINE;
    if (memory->getRam()->size() % BYTES_PER_LINE)
        height++;

    height *= m_charHeight;

    return QSize(width, height);
}

void HexViewWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(viewport());

    QSize areaSize = viewport()->size();
    QSize widgetSize = FullSize();
    verticalScrollBar()->setPageStep(areaSize.height() / m_charHeight);
    verticalScrollBar()->setRange(0, (widgetSize.height() - areaSize.height()) / m_charHeight + 1);

    int firstLineIdx = verticalScrollBar()->value();
    int lastLineIdx = firstLineIdx + areaSize.height() / m_charHeight;
    if(lastLineIdx > memory->getRam()->size() / BYTES_PER_LINE) {
        lastLineIdx = memory->getRam()->size() / BYTES_PER_LINE;
        if (memory->getRam()->size() % BYTES_PER_LINE)
            lastLineIdx++;
    }

    painter.fillRect(event->rect(), this->palette().color(QPalette::Base));

    QColor addressAreaColor = QColor(0xd4, 0xd4, 0xd4, 0xff);
    painter.fillRect(QRect(m_posAddr, event->rect().top(), m_posHex - GAP_ADR_HEX + 2, height()), addressAreaColor);

    int yPosStart = m_charHeight;

    QByteArray data = memory->getRam()->mid(firstLineIdx * BYTES_PER_LINE, (lastLineIdx - firstLineIdx) * BYTES_PER_LINE);
    for (int lineIdx = firstLineIdx, yPos = yPosStart; lineIdx < lastLineIdx; lineIdx += 1, yPos += m_charHeight) {
        QString address = QString("%1").arg(lineIdx * 16, 3, 16, QChar('0'));
        painter.drawText(m_posAddr, yPos, address);

        for (int xPos = m_posHex, i = 0; i < BYTES_PER_LINE && ((lineIdx - firstLineIdx) * BYTES_PER_LINE + i) < data.size(); i++, xPos += 3 * m_charWidth) {
            QString val = QString::number((data.at((lineIdx - firstLineIdx) * BYTES_PER_LINE + i) & 0xF0) >> 4, 16);
            painter.drawText(xPos, yPos, val);
            val = QString::number((data.at((lineIdx - firstLineIdx) * BYTES_PER_LINE + i) & 0xF), 16);
            painter.drawText(xPos+m_charWidth, yPos, val);
        }
    }
}

void HexViewWidget::redraw()
{
    viewport()->update();
}
