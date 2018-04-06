#include "mainwindow.h"

#include <QFileInfo>
#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    memory = new Memory;
    connect(this, &MainWindow::reset, memory, &Memory::onReset);

    cpu = new CPU(memory);
    connect(cpu, &CPU::error, this, &MainWindow::onCpuError);
    connect(this, &MainWindow::reset, cpu, &CPU::onReset);

    setMinimumSize(1024, 800);

    QToolBar *toolBar = new QToolBar(this);
    toolBar->addAction(style()->standardIcon(QStyle::SP_DirIcon), "Open", this, &MainWindow::onOpenRom);
    toolBar->addAction(style()->standardIcon(QStyle::SP_MediaPlay), "Run", this, &MainWindow::onRun);
    toolBar->addAction(style()->standardIcon(QStyle::SP_MediaSkipForward), "Step", this, &MainWindow::onStep);
    toolBar->addAction(style()->standardIcon(QStyle::SP_BrowserReload), "Reset", this, &MainWindow::onReset);
    addToolBar(toolBar);

    QHBoxLayout *hLayout = new QHBoxLayout;
    QVBoxLayout *leftVLayout = new QVBoxLayout;
    QVBoxLayout *rightVLayout = new QVBoxLayout;

    displayWidget = new DisplayWidget(this, memory);
    connect(cpu, &CPU::draw, displayWidget, &DisplayWidget::draw);

    disasmWidget = new DisasmWidget(this, memory);
    connect(cpu, &CPU::tick, disasmWidget, &DisasmWidget::highlightCurrentLine);

    memoryWidget = new HexViewWidget(this, memory);
    leftVLayout->addWidget(disasmWidget, 2);
    leftVLayout->addWidget(memoryWidget, 1);

    registersWidget = new RegistersWidget(this, cpu);
    registersWidget->setMaximumWidth(displayWidget->width());
    connect(cpu, &CPU::tick, registersWidget, &RegistersWidget::redraw);

    rightVLayout->addWidget(displayWidget);
    rightVLayout->addWidget(registersWidget);
    hLayout->addLayout(leftVLayout);
    hLayout->addLayout(rightVLayout, 0);

    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(hLayout);

    setCentralWidget(centralWidget);

    prevFilePath = QDir::currentPath();
}

MainWindow::~MainWindow()
{

}

void MainWindow::onOpenRom()
{
    QFile file(QFileDialog::getOpenFileName(this, tr("Open ROM"), prevFilePath, tr("ROM Files (*.rom)")));
    if (file.fileName().isEmpty())
        return;
    prevFilePath = QFileInfo(file).absolutePath();
    if (file.size() > 0xFFF - 0x200) {
        QMessageBox::critical(this, tr("Error!"), tr("File ") + file.fileName() + tr(" is too large"), QMessageBox::Ok);
        return;
    }
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, tr("Error!"), tr("Unable to open file ") + file.fileName(), QMessageBox::Ok);
        return;
    }
    memory->loadRom(file.readAll());
    disasmWidget->Disasm();
    memoryWidget->redraw();

    emit reset();
}

void MainWindow::onCpuError(int op, int addr)
{
    QMessageBox::information(this, "Info!", QString("Unknown instruction %1 at 0x%2")
                             .arg(op, 4, 16, QChar('0'))
                             .arg(addr, 3, 16, QChar('0')), QMessageBox::Ok);
    memoryWidget->setEnabled(true);
    registersWidget->setEnabled(true);
}

void MainWindow::onRun()
{
    if (cpu->isRunning())
        return;
    memoryWidget->setEnabled(false);
    registersWidget->setEnabled(false);
    disasmWidget->cleanHighlight();
    cpu->start();
}

void MainWindow::onStep()
{
    if (cpu->isRunning())
        return;
    cpu->onTick();
}

void MainWindow::onReset()
{
    cpu->onStop();
    memoryWidget->setEnabled(true);
    registersWidget->setEnabled(true);

    emit reset();
}
