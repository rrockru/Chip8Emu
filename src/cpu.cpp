#include "cpu.h"

CPU::CPU(Memory *memory):
    memory(memory)
{
    I = 0;
    SP = 0;
    PC = 0x200;
}

void CPU::tick()
{
    memory->CleanVRam();
    memory->SetVRamBit(I % 0x800, true);
    I++;
    PC += 2;
}
