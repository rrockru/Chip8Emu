#ifndef TIMER_H
#define TIMER_H

#include <QTimer>

class Timer : public QTimer
{
    int value;

public slots:
    void onTimer() { if(value>0) value--; }
    void setValue(int val) { value = val; }
    int getValue() { return value; }

public:
    Timer();
};

#endif // TIMER_H
