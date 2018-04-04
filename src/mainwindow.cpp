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
    memory = new Memory();
    cpu = new CPU(this, memory);
    connect(cpu, &CPU::error, this, &MainWindow::onCpuError);

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
    connect(cpu, &CPU::tickSignal, disasmWidget, &DisasmWidget::highlightCurrentLine);

    memoryWidget = new HexViewWidget(this, memory);
    leftVLayout->addWidget(disasmWidget, 2);
    leftVLayout->addWidget(memoryWidget, 1);

    registersView = new QTableView(this);
    registersView->horizontalHeader()->hide();
    registersView->setFixedWidth(displayWidget->width());
    registersView->setModel(cpu);

    rightVLayout->addWidget(displayWidget);
    rightVLayout->addWidget(registersView);
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
    memory->LoadRom(file.readAll());
    disasmWidget->Disasm();
    memoryWidget->redraw();
}

void MainWindow::onCpuError(uint16_t op, uint16_t addr)
{
    memoryWidget->setEnabled(true);
    registersView->setEnabled(true);
    QMessageBox::information(this, "Info!", QString("Unknown instruction %1 at 0x%2")
                             .arg(op, 4, 16, QChar('0'))
                             .arg(addr, 3, 16, QChar('0')), QMessageBox::Ok);
}

void MainWindow::onRun()
{
    memoryWidget->setEnabled(false);
    registersView->setEnabled(false);
    disasmWidget->cleanHighlight();
    cpu->onRun();
}

void MainWindow::onStep()
{
    if (cpu->isRun())
        return;
    cpu->onStep();
}

void MainWindow::onReset()
{
    memoryWidget->setEnabled(true);
    registersView->setEnabled(true);
    disasmWidget->highlightCurrentLine(0x200);
    cpu->onReset();
}
