#include "mainwindow.h"

#include <QApplication>
#include <QEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QKeyEvent>
#include <QMessageBox>
#include <QTextEdit>
#include <QToolBar>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    memory = new Memory;
    connect(this, &MainWindow::reset, memory, &Memory::onReset);

    keyboard = new Keyboard;
    connect(this, &MainWindow::reset, keyboard, &Keyboard::onReset);

    cpu = new CPU(memory, keyboard);
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
    connect(cpu, &CPU::tick, memoryWidget, &HexViewWidget::redraw);

    leftVLayout->addWidget(disasmWidget, 2);
    leftVLayout->addWidget(memoryWidget, 1);

    registersWidget = new RegistersWidget(this, cpu);
    registersWidget->setMaximumWidth(displayWidget->width());
    connect(cpu, &CPU::tick, registersWidget, &RegistersWidget::redraw);

    stackWidget = new StackWidget(this, cpu, memory);
    stackWidget->setMaximumWidth(displayWidget->width());
    connect(cpu, &CPU::tick, stackWidget, &StackWidget::redraw);

    rightVLayout->addWidget(displayWidget);
    rightVLayout->addWidget(registersWidget);
    rightVLayout->addWidget(stackWidget);
    hLayout->addLayout(leftVLayout);
    hLayout->addLayout(rightVLayout, 0);

    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(hLayout);

    setCentralWidget(centralWidget);

    prevFilePath = QDir::currentPath();

    qApp->installEventFilter(this);
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

    setWindowTitle(QString("Chip8Emu - %1").arg(file.fileName()));

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

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type()==QEvent::KeyPress || event->type()==QEvent::KeyRelease) {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);
        keyboard->onKeyEvent(key->key(), event->type() == QEvent::KeyPress);
        return true;
    } else {
        return QObject::eventFilter(obj, event);
    }
    return false;
}
