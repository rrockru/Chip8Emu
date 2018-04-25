#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QThread>
#include <QToolButton>

#include "cpu.h"
#include "memory.h"
#include "keyboard.h"

#include "display.h"
#include "hexview.h"
#include "disasm.h"
#include "registers.h"
#include "stack.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    CPU *cpu;
    Memory *memory;
    Keyboard *keyboard;

    DisplayWidget *displayWidget;
    DisasmWidget *disasmWidget;
    HexViewWidget *memoryWidget;
    RegistersWidget *registersWidget;
    StackWidget *stackWidget;

    QAction *openRomButton;
    QAction *runButton;
    QAction *stepButton;
    QAction *resetButton;
    QAction *breakButton;

    QString prevFilePath;

signals:
    void reset();

public slots:
    void onOpenRomAction();
    void onCpuError(int op, int addr);
    void onRunAction();
    void onStepAction();
    void onResetAction();
    void onBreakpointAction();
    void onBreakpoint();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    bool eventFilter(QObject* obj, QEvent* event);
};

#endif // MAINWINDOW_H
