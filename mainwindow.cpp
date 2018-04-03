#include "mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    memory = new Memory();
    cpu = new CPU(memory);

    //setFixedSize(640, 480);

    QToolBar *toolBar = new QToolBar(this);
    toolBar->addAction(style()->standardIcon(QStyle::SP_DirIcon), "Open", this, &MainWindow::openRom);
    toolBar->addAction(style()->standardIcon(QStyle::SP_MediaPlay), "Run", this, &MainWindow::run);
    toolBar->addAction(style()->standardIcon(QStyle::SP_MediaSkipForward), "Step", this, &MainWindow::step);
    addToolBar(toolBar);

    QHBoxLayout *hLayout = new QHBoxLayout;
    QVBoxLayout *vLayout = new QVBoxLayout;

    displayWidget = new DisplayWidget(this, memory);
    memoryWidget = new HexViewWidget(this, memory);
    registersWidget = new QTableWidget(this);

    vLayout->addWidget(displayWidget);
    vLayout->addWidget(registersWidget);
    hLayout->addWidget(memoryWidget);
    hLayout->addLayout(vLayout);

    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(hLayout);

    setCentralWidget(centralWidget);

    cpuTimer = new QTimer(this);
    connect(cpuTimer, &QTimer::timeout, cpu, &CPU::tick);
    //connect(timer, &QTimer::timeout, memoryWidget, &HexViewWidget::redraw);
    //connect(timer, &QTimer::timeout, displayWidget, &DisplayWidget::redraw);

}

MainWindow::~MainWindow()
{

}

void MainWindow::openRom()
{
    QFile file(QFileDialog::getOpenFileName(this, tr("Open ROM"), QDir::currentPath(), tr("ROM Files (*.rom)")));
    if (file.fileName().isEmpty())
        return;
    if (file.size() > 0xFFF - 0x200) {
        QMessageBox::critical(this, tr("Error!"), tr("File ") + file.fileName() + tr(" is too large"), QMessageBox::Ok);
        return;
    }
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, tr("Error!"), tr("Unable to open file ") + file.fileName(), QMessageBox::Ok);
        return;
    }
    memory->LoadRom(file.readAll());
    memoryWidget->redraw();
}

void MainWindow::run()
{
    cpuTimer->start(2);
}

void MainWindow::step()
{
    cpu->tick();
    memoryWidget->redraw();
}
