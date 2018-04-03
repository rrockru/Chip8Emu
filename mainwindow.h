#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTimer>

#include "cpu.h"
#include "memory.h"

#include "display.h"
#include "hexview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QTimer *cpuTimer;

    CPU *cpu;
    Memory *memory;

    DisplayWidget *displayWidget;
    HexViewWidget *memoryWidget;
    QTableWidget *registersWidget;

public slots:
    void openRom();
    void run();
    void step();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
