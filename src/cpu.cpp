#include "cpu.h"

#include <QDebug>
#include <QMessageBox>

CPU::CPU(QObject *parent, Memory *memory):
    QAbstractTableModel(parent),
    memory(memory)
{
    Reset();
}

void CPU::Reset()
{
    for (int i = 0; i < 16; i++)
        V[i] = 0;
    I = 0;
    SP = 0;
    PC = 0x200;

    emit dataChanged(createIndex(0, 0), createIndex(19, 0));
}

void CPU::tick()
{
    uint16_t op = memory->GetRamWord(PC);

    switch (op >> 12) {
    case 0x1: {
        PC = (op & 0xFFF) - 2;
    }
    case 0x3: {
        if ((V[(op >> 8) & 0xF]) == (op & 0xFF))
            PC += 2;
        break;
    }
    case 0x6: {
        V[(op >> 8) & 0xF] = op & 0xFF;
        break;
    }
    case 0x7: {
        V[(op >> 8) & 0xF] += op & 0xFF;
        break;
    }
    case 0xA: {
        I = op & 0xFFF;
        break;
    }
    case 0xC: {
        V[(op >> 8) & 0xF] = (qrand() % 255) & (op & 0xFF);
        break;
    }
    case 0xD: {
        int x = V[(op >> 8) & 0xF];
        int y = V[(op >> 4) & 0xF];
        int height = op & 0xF;

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < 8; j++) {
                memory->SetVRamBit((y + i) * 64 + (x + j), (memory->GetRamByte(I + i) >> (7 - (j % 8))) & 1);
            }
        }

        emit draw();
        break;
    }
    default: {
        emit error(op, PC);
        return;
    }
    }

    PC += 2;

    emit dataChanged(createIndex(0, 0), createIndex(19, 0));
}

int CPU::rowCount(const QModelIndex &) const
{
    return 19;
}

int CPU::columnCount(const QModelIndex &) const
{
    return 1;
}

/* Model stuff */

QVariant CPU::data(const QModelIndex &index, int role) const
{
    int row = index.row();

    switch(role) {
    case Qt::DisplayRole:
        if (row < 16) {
            return QString("%1").arg(V[row], 2, 16, QChar('0'));
        }
        switch (row) {
        case 16:
            return QString("%1").arg(I, 4, 16, QChar('0'));
            break;
        case 17:
            return QString("%1").arg(SP, 4, 16, QChar('0'));
            break;
        case 18:
            return QString("%1").arg(PC, 4, 16, QChar('0'));
            break;
        }
        break;
    }
    return QVariant();
}

QVariant CPU::headerData(int section, Qt::Orientation, int role) const
{
    switch(role) {
    case Qt::DisplayRole:
        if (section < 16) {
            return QString("V%1").arg(section, 1, 16);
        }
        switch (section) {
        case 16:
            return "I";
            break;
        case 17:
            return "SP";
            break;
        case 18:
            return "PC";
            break;
        }
        break;
    }
    return QVariant();
}
