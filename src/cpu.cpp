#include "cpu.h"

#include <QDebug>
#include <QMessageBox>
#include <QThread>

CPU::CPU(Memory *memory):
    memory(memory)
{
    onReset();
}

void CPU::onReset()
{
    for (int i = 0; i < 16; i++)
        V[i] = 0;
    I = 0;
    SP = 0;
    PC = 0x200;

    emit draw();
    emit tick(PC);
}

void CPU::onTick()
{
    uint16_t op = memory->getRamWord(PC);

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

        bool flipFlag = false;
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < 8; i++) {
                bool oldBit = memory->getVRamBit((y + j) * 64 + (x + i));
                bool newBit = (memory->getRamByte(I + j) >> (7 - (i % 8))) & 1;
                if (oldBit && !newBit) flipFlag = true;
                memory->setVRamBit((y + j) * 64 + (x + i), (memory->getRamByte(I + j) >> (7 - (i % 8))) & 1);
            }
        }

        V[0xF] = (flipFlag) ? 1 : 0;

        emit draw();
        break;
    }
    default: {
        stopFlag = true;
        emit error(op, PC);
        return;
    }
    }

    PC += 2;

    emit tick(PC);
}

void CPU::run()
{
    stopFlag = false;

    while(!stopFlag) {
        usleep(2000);
        onTick();
    }

    emit finished();
}

void CPU::onStop()
{
    if (isRunning()) {
        stopFlag = true;
        wait();
    }
}

