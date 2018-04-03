#ifndef HEXVIEW_H
#define HEXVIEW_H

#include <QAbstractScrollArea>
#include <QSize>

#include "memory.h"

class HexViewWidget : public QAbstractScrollArea
{

    QSize FullSize() const;

    int m_charWidth;
    int m_charHeight;

    int m_posAddr;
    int m_posHex;

    Memory *memory;

public slots:
    void redraw();

public:
    HexViewWidget(QWidget *parent, Memory *memory);

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // HEXVIEW_H
