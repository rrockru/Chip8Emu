#include "mainwindow.h"

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

    //setFixedSize(640, 480);

    QToolBar *toolBar = new QToolBar(this);
    toolBar->addAction(style()->standardIcon(QStyle::SP_DirIcon), "Open", this, &MainWindow::onOpenRom);
    toolBar->addAction(style()->standardIcon(QStyle::SP_MediaPlay), "Run", this, &MainWindow::onRun);
    toolBar->addAction(style()->standardIcon(QStyle::SP_MediaSkipForward), "Step", this, &MainWindow::onStep);
    toolBar->addAction(style()->standardIcon(QStyle::SP_BrowserReload), "Reset", this, &MainWindow::onReset);
    addToolBar(toolBar);

    QHBoxLayout *hLayout = new QHBoxLayout;
    QVBoxLayout *vLayout = new QVBoxLayout;

    displayWidget = new DisplayWidget(this, memory);
    connect(cpu, &CPU::draw, displayWidget, &DisplayWidget::draw);

    memoryWidget = new HexViewWidget(this, memory);

    registersView = new QTableView(this);
    registersView->horizontalHeader()->hide();

    vLayout->addWidget(displayWidget);
    vLayout->addWidget(registersView);
    hLayout->addWidget(memoryWidget);
    hLayout->addLayout(vLayout);

    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(hLayout);

    setCentralWidget(centralWidget);

    cpuTimer = new QTimer(this);
    connect(cpuTimer, &QTimer::timeout, cpu, &CPU::tick);
    //connect(timer, &QTimer::timeout, memoryWidget, &HexViewWidget::redraw);

}

MainWindow::~MainWindow()
{

}

void MainWindow::onOpenRom()
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

void MainWindow::onRun()
{
    registersView->setModel(NULL);

    cpuTimer->start(2);
}

void MainWindow::onStep()
{

    registersView->setModel(cpu);

    cpu->tick();
    memoryWidget->redraw();
}

void MainWindow::onReset() {
    if (cpuTimer->isActive())
        cpuTimer->stop();
    cpu->Reset();
    memory->Reset();
    displayWidget->draw();
}

void MainWindow::onCpuError(uint16_t op, uint16_t addr)
{
    if (cpuTimer->isActive())
        cpuTimer->stop();
    QMessageBox::information(this, "Info!", QString("Unknown instruction %1 at 0x%2")
                             .arg(op, 4, 16, QChar('0'))
                             .arg(addr, 3, 16, QChar('0')), QMessageBox::Ok);
}
