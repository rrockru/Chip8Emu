#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QObject>

class Keyboard : public QObject
{
    Q_OBJECT

    bool state[0x10];

public slots:
    void onKeyEvent(int key, bool keyState);
    void onReset();

public:
    explicit Keyboard(QObject *parent = nullptr);

    bool getKeyState(int key) { return state[key]; }

signals:

public slots:
};

#endif // KEYBOARD_H
