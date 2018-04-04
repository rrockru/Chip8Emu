#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>

#include "cpu.h"
#include "memory.h"

#include "display.h"
#include "hexview.h"
#include "disasm.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    CPU *cpu;
    Memory *memory;

    DisplayWidget *displayWidget;
    DisasmWidget *disasmWidget;
    HexViewWidget *memoryWidget;
    QTableView *registersView;

    QString prevFilePath;

public slots:
    void onOpenRom();
    void onCpuError(uint16_t op, uint16_t addr);
    void onRun();
    void onStep();
    void onReset();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
