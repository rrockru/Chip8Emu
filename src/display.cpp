#include "display.h"

#include <QPainter>
#include <QPaintEvent>

DisplayWidget::DisplayWidget(QWidget *parent, Memory *memory):
    QOpenGLWidget(parent),
    memory(memory)
{
    setFixedSize(321, 161);
    setAutoFillBackground(false);
}

void DisplayWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(event->rect(), Qt::black);

    for (int i = 0; i < 0x800; i++) {
        if (memory->getVRamBit(i)) {
            int x = (i % 64) * 5 + 1;
            int y = (i / 64) * 5 + 1;

            painter.fillRect(x, y, 4, 4, Qt::darkGreen);
        }
    }

    painter.end();
}

void DisplayWidget::draw()
{
    update();
}
