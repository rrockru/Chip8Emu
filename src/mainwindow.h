#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
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
    QTableView *registersView;

public slots:
    void onOpenRom();
    void onRun();
    void onStep();
    void onReset();
    void onCpuError(uint16_t op, uint16_t addr);

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
