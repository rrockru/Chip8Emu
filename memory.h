#ifndef MEMORY_H
#define MEMORY_H

#include <QByteArray>

class Memory
{
    QByteArray ram;

    void Clean();
    void CleanRam();
    void CleanStack();

public:
    Memory();

    QByteArray* GetRam() { return &ram; }

    void LoadRom(QByteArray rom);
    void CleanVRam();

    char GetRamByte(int pos) { return ram[pos]; }
    void SetRamByte(int pos, int value) { ram[pos] = value; }
    bool GetVRamBit(int pos) { return (ram[0xF00 + (pos / 8)] >> (pos % 8)) & 1; }
    void SetVRamBit(int pos, bool value) {ram[0xF00 + (pos / 8)] = (ram[0xF00 + (pos / 8)]) | (value << (pos % 8)); }
};

#endif // MEMORY_H
