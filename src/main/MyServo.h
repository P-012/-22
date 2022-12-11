#ifndef MyServo_h
#define MyServo_h

#include "Arduino.h"
#include <Servo.h>

class MyServo
{
  public:
    MyServo(int pin);
    void init();
    void rotate();

  private:
    Servo _servo;
    int _pin;
};

#endif