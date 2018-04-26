#ifndef MEMORY_H
#define MEMORY_H

#include <QByteArray>
#include <QObject>

class Memory: public QObject
{
    Q_OBJECT

    QByteArray ram;

    uint16_t currentRomSize;

    void clear();
    void clearRam();
    void clearStack();    

public slots:
    void onReset();

public:
    Memory();

    QByteArray* getRam() { return &ram; }
    void clearVRam();

    void loadRom(QByteArray rom);
    uint16_t getRomSize() { return currentRomSize; }

    char getRamByte(int pos) { return ram[pos]; }
    void setRamByte(int pos, int value) { ram[pos] = value; }
    uint16_t getRamWord(int pos) { return ((uint8_t)ram[pos] << 8) + (uint8_t)ram[pos + 1]; }
    void setRamWord(int pos, uint16_t value) { ram[pos] = value >> 8; ram[pos + 1] = value & 0xFF; }
    bool getVRamBit(int pos) { return (ram[0xF00 + (pos / 8)] >> (7 - (pos % 8))) & 1; }
    void setVRamBit(int pos, bool value) { ram[0xF00 + (pos / 8)] = (ram[0xF00 + (pos / 8)]) ^ (value << (7 - (pos % 8))); }

    void pushToStack(uint8_t addr, uint16_t val);
    uint16_t popFromStack(uint8_t addr);
};

#endif // MEMORY_H
