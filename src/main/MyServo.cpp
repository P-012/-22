#include "Arduino.h"
#include <Servo.h>
#include "MyServo.h"

MyServo::MyServo(int pin)
{
  _pin = pin;
}

void MyServo::init()
{
  _servo.attach(_pin);
}

long MyHyperSonic::rotate(int pos)
{
  _servo.write(pos);
}