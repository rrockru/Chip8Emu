#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>

#include "memory.h"

class DisplayWidget : public QOpenGLWidget
{
    Q_OBJECT

    Memory *memory;

public slots:
    void draw();

public:
    DisplayWidget(QWidget *parent, Memory *memory);

protected:
    void paintEvent(QPaintEvent *) override;
};

#endif // GLWIDGET_H
