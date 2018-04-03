#include "memory.h"

#include <algorithm>

Memory::Memory()
{
    ram.resize(0x1000);

    Clean();
}

void Memory::Clean()
{
    CleanRam();
    CleanStack();
    CleanVRam();
}

void Memory::CleanRam()
{
    for (int i = 0; i < 0xEA0; i++) {
        ram[i] = 0;
    }
}

void Memory::CleanStack()
{
    for (int i = 0xEA0; i < 0xF00; i++) {
        ram[i] = 0;
    }
}

void Memory::CleanVRam()
{
    for (int i = 0xF00; i < 0x1000; i++) {
        ram[i] = 0;
    }
}

void Memory::LoadRom(QByteArray rom) {
    Clean();
    ram.replace(0x200, rom.size(), rom);
}

void Memory::Reset() {
    CleanStack();
    CleanVRam();
}
