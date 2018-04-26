#include "cpu.h"

#include <QDebug>
#include <QMessageBox>
#include <QThread>

#include "config.h"

CPU::CPU(Memory *memory, Keyboard *keyboard):
    memory(memory),
    keyboard(keyboard)
{
    delayTimer = new Timer;
    soundTimer = new Timer;

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
    case 0x0: {
        switch (op >> 4) {
        case 0xE: {
            switch (op & 0xF) {
            case 0x0: {
                /* 00E0 CLR clear screen */
                memory->clearVRam();
                emit draw();

                PC += 2;
                break;
            }
            case 0xE: {
                /* 00EE RET return from subroutine */
                SP -= 2;
                PC = memory->popFromStack(SP);

                break;
            }
            }
            break;
        }
        default: {
            /* 0NNN CALL call RCA 1802 program at address NNN */

            PC += 2;
            break;
        }
        }
        break;
    }
    case 0x1: {
        /* 1NNN JP jump to address NNN */
        PC = (op & 0xFFF);

        break;
    }
    case 0x2: {
        /* 2NNN CALL call subroutine at NNN */
        memory->pushToStack(SP, PC + 2);
        SP += 2;
        PC = (op & 0xFFF);

        break;
    }
    case 0x3: {
        /* 3XNN SE skip next if VX equals NN */
        if ((V[(op >> 8) & 0xF]) == (op & 0xFF))
            PC += 2;

        PC += 2;
        break;
    }
    case 0x4: {
        /* 4XNN SNE skip next if VX not equals NN */
        if ((V[(op >> 8) & 0xF]) != (op & 0xFF))
            PC += 2;

        PC += 2;
        break;
    }
    case 0x5: {
        /* 5XY0 SE skip next if VX equals VY */
        if ((V[(op >> 8) & 0xF]) == V[(op >> 4) & 0xF])
            PC += 2;

        PC += 2;
        break;
    }
    case 0x6: {
        /* 6XNN LD set VX to NN */
        V[(op >> 8) & 0xF] = op & 0xFF;

        PC += 2;
        break;
    }
    case 0x7: {
        /* 7XNN ADD add NN to VX */
        V[(op >> 8) & 0xF] += op & 0xFF;

        PC += 2;
        break;
    }
    case 0x8: {
        switch (op & 0xF) {
        case 0x0: {
            /* 8XY0 LD set VX to VY */
            V[(op >> 8) & 0xF] = V[(op >> 4) & 0xF];

            PC += 2;
            break;
        }
        case 0x1: {
            /* 8XY1 OR set VX to VX | VY */
            V[(op >> 8) & 0xF] = V[(op >> 8) & 0xF] | V[(op >> 4) & 0xF];

            PC += 2;
            break;
        }
        case 0x2: {
            /* 8XY2 AND set VX to VX & VY */
            V[(op >> 8) & 0xF] = V[(op >> 8) & 0xF] & V[(op >> 4) & 0xF];

            PC += 2;
            break;
        }
        case 0x3: {
            /* 8XY3 XOR set VX to VX ^ VY */
            V[(op >> 8) & 0xF] = V[(op >> 8) & 0xF] ^ V[(op >> 4) & 0xF];

            PC += 2;
            break;
        }
        case 0x4: {
            /* 8XY4 ADD set VX to VX + VY with carry */
            uint16_t res = V[(op >> 8) & 0xF] + V[(op >> 4) & 0xF];
            V[0xF] = (res > 0xFF) ? 1 : 0;
            V[(op >> 8) & 0xF] = res & 0xFF;

            PC += 2;
            break;
        }
        case 0x5: {
            /* 8XY5 SUB set VX to VX - VY with borrow */
            V[0xF] = V[(op >> 8) & 0xF] > V[(op >> 4) & 0xF] ? 1 : 0;
            V[(op >> 8) & 0xF] -= V[(op >> 4) & 0xF];

            PC += 2;
            break;
        }
        case 0x6: {
            /* 8XY6 SHR shift VY by one and copy result to VX. VF = LSB */
            V[0xF] = V[(op >> 8) & 0xF] & 1;
            V[(op >> 8) & 0xF] = V[(op >> 8) & 0xF] >> 1;

            PC += 2;
            break;
        }
        case 0x7: {
            /* 8XY7 SUB set VX to VY - VX with borrow */
            V[0xF] = V[(op >> 4) & 0xF] > V[(op >> 8) & 0xF];
            V[(op >> 4) & 0xF] -= V[(op >> 8) & 0xF];

            PC += 2;
            break;
        }
        case 0xE: {
            /* 8XYE SHL shift VY by one and copy result to VX. VF = MSB */
            V[0xF] = (V[(op >> 8) & 0xF] >> 7) & 1;
            V[(op >> 8) & 0xF] = (V[(op >> 8) & 0xF] << 1) & 0xFF;

            PC += 2;
            break;
        }
        default: {

            PC += 2;
            break;
        }
        }

        break;
    }
    case 0x9: {
        /* 9XY0 SNE skip next if VX not equals VY */
        if ((V[(op >> 8) & 0xF]) != V[(op >> 4) & 0xF])
            PC += 2;

        PC += 2;
        break;
    }
    case 0xA: {
        /* ANNN LD I set I to NNN */
        I = op & 0xFFF;

        PC += 2;
        break;
    }
    case 0xB: {
        /* 1NNN JP jump to address V0 + NNN */
        PC = V[0] + (op & 0xFFF);

        break;
    }
    case 0xC: {
        /* CXNN RND set VX to bitwise and on random number and NN */
        V[(op >> 8) & 0xF] = (qrand() % 255) & (op & 0xFF);

        PC += 2;
        break;
    }
    case 0xD: {
        /* DXYN DRW draw sprite from register I with height of N on coordinates (VX, VY) */
        int x = V[(op >> 8) & 0xF];
        int y = V[(op >> 4) & 0xF];
        int height = op & 0xF;

        bool flipFlag = false;
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < 8; i++) {
                bool oldBit = memory->getVRamBit((y + j) * 64 + (x + i));
                bool newBit = (memory->getRamByte(I + j) >> (7 - (i % 8))) & 1;
                if (oldBit && newBit)
                    flipFlag = true;
                memory->setVRamBit((y + j) * 64 + (x + i), newBit);
            }
        }

        V[0xF] = (flipFlag) ? 1 : 0;

        emit draw();

        PC += 2;
        break;
    }
    case 0xE: {
        switch (op & 0xFF) {
        case 0x9E: {
            /* EX9E SKP skip next if key in VX is pressed */
            if (keyboard->getKeyState(V[(op >> 8) & 0xF]))
                PC += 2;

            break;
        }
        case 0xA1: {
            /* EXA1 SKNP skip next if key in VX is not pressed */
            if (!keyboard->getKeyState(V[(op >> 8) & 0xF]))
                PC += 2;

            break;
        }
        default:
            break;
        }

        PC += 2;
        break;
    }
    case 0xF: {
        switch (op & 0xFF) {
        case 0x07: {
            /* FX07 LD load value of delay timer to VX register */
            V[(op >> 8) & 0xF] = delayTimer->getValue();

            break;
        }
        case 0x0A: {
            /* FX0A LD wait for key and set its value to VX */
            int currentKey;
            if(currentKey = keyboard->getCurrentKey() == -1)
                PC -= 2;
            else
                V[(op >> 8) & 0xF] = currentKey;

            break;
        }
        case 0x15: {
            /* FX15 LD set delay timer to VX */
            delayTimer->setValue(V[(op >> 8) & 0xF]);

            break;
        }
        case 0x18: {
            /* FX18 LD set sound timer to VX */
            soundTimer->setValue(V[(op >> 8) & 0xF]);

            break;
        }
        case 0x1E: {
            /*FX1E ADD add VX to I */
            I += V[(op >> 8) & 0xF];

            break;
        }
        case 0x29: {
            /* FX29 LD set I to addr of VX sprite */
            I = V[(op >> 8) & 0xF] * 5;

            break;
        }
        case 0x33: {
            /* FX33 BCD BCD VX and store in I - I+3 */
            memory->setRamByte(I, V[(op >> 8) & 0xF] / 100);
            memory->setRamByte(I + 1, (V[(op >> 8) & 0xF] % 100) / 10);
            memory->setRamByte(I + 1, (V[(op >> 8) & 0xF] % 10));

            break;
        }
        case 0x55: {
            /* FX55 ST store V0-VX in I+ */
            for (int i = 0; i <= ((op >> 8) & 0xF); i++) {
                memory->setRamByte(I++, V[i]);
            }

            break;
        }
        case 0x65: {
            /* FX65 LD load V0-VX from I+ */
            for (int i = 0; i <= ((op >> 8) & 0xF); i++) {
                V[i] = memory->getRamByte(I++);
            }

            break;
        }
        default:
            break;
        }

        PC += 2;
        break;
    }
    default: {
        stopFlag = true;
        emit error(op, PC);
        return;
    }
    }    

    emit tick(PC);
}

void CPU::run()
{
    stopFlag = false;

    while(!stopFlag) {
        usleep(2000);
        onTick();

        if (breakpoints.contains(PC)) {
            emit breakpoint();
            break;
        }
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

