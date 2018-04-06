#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QThread>

#include "cpu.h"
#include "memory.h"

#include "display.h"
#include "hexview.h"
#include "disasm.h"
#include "registers.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    CPU *cpu;
    Memory *memory;

    DisplayWidget *displayWidget;
    DisasmWidget *disasmWidget;
    HexViewWidget *memoryWidget;
    RegistersWidget *registersWidget;

    QString prevFilePath;

signals:
    void reset();

public slots:
    void onOpenRom();
    void onCpuError(int op, int addr);
    void onRun();
    void onStep();
    void onReset();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
