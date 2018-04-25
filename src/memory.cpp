#include "memory.h"

#include "config.h"

Memory::Memory()
{
    currentRomSize = 0;

    ram.resize(0x1000);

    for (int i = 0; i < 0x200; i++)
        ram[i] = 0;

    //ram.replace(0, 0x200, QByteArray::fromRawData((const char*)rom, 0x200));
    ram.replace(0, 0x200, QByteArray::fromRawData((const char*)ascii, 0x200));

    clear();
}

void Memory::clear()
{
    clearRam();
    clearStack();
    clearVRam();
}

void Memory::clearRam()
{
    for (int i = 0x200; i < 0xEA0; i++) {
        ram[i] = 0;
    }
}

void Memory::clearStack()
{
    for (int i = 0xEA0; i < 0xF00; i++) {
        ram[i] = 0;
    }
}

void Memory::clearVRam()
{
    for (int i = 0xF00; i < 0x1000; i++) {
        ram[i] = 0;
    }
}

void Memory::loadRom(QByteArray rom) {
    clear();
    ram.replace(0x200, rom.size(), rom);
    currentRomSize = rom.size();
}

void Memory::onReset() {
    clearStack();
    clearVRam();
}

void Memory::pushToStack(uint8_t addr, uint16_t val) {
    if (addr <= 96) {
        setRamWord(0xEFE - addr, val);
    }
}

uint16_t Memory::popFromStack(uint8_t addr) {
    if (addr >= 0) {
        return getRamWord(0xEFE - addr);
    }

    return 0;
}
