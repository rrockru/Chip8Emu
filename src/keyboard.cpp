#include "keyboard.h"

#include "config.h"

Keyboard::Keyboard(QObject *parent):
    QObject(parent)
{
    onReset();
}

void Keyboard::onReset()
{
    for (int i = 0; i < 0x10; i++) {
        state[i] = false;
    }
}

void Keyboard::onKeyEvent(int key, bool keyState) {
    for (int i = 0; i < 0x10; i++) {
        if (keyMapping[i] == key) {
            state[i] = keyState;
        }
    }
}
