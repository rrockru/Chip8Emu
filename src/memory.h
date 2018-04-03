#ifndef MEMORY_H
#define MEMORY_H

#include <QByteArray>

class Memory
{
    QByteArray ram;

    void Clean();
    void CleanRam();
    void CleanStack();
    void CleanVRam();

public:
    Memory();

    QByteArray* GetRam() { return &ram; }

    void LoadRom(QByteArray rom);
    void Reset();

    char GetRamByte(int pos) { return ram[pos]; }
    uint16_t GetRamWord(int pos) { return (ram[pos] << 8) + ram[pos + 1]; }
    void SetRamByte(int pos, int value) { ram[pos] = value; }
    bool GetVRamBit(int pos) { return (ram[0xF00 + (pos / 8)] >> (7 - (pos % 8))) & 1; }
    void SetVRamBit(int pos, bool value) { ram[0xF00 + (pos / 8)] = (ram[0xF00 + (pos / 8)]) | (value << (7 - (pos % 8))); }
};

#endif // MEMORY_H
