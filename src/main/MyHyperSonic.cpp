#include "Arduino.h"
#include "MyHyperSonic.h"

MyHyperSonic::MyHyperSonic(int trig, int echo)
{
  _trig = trig;
  _echo = echo;
}

void MyHyperSonic::init()
{
  pinMode(_trig, OUTPUT);
  pinMode(_echo, INPUT);
}

long MyHyperSonic::getDistance()
{
  long duration, distance;
  digitalWrite(_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trig, LOW);

  duration = pulseIn(_echo, HIGH);
  distance = ((float)(340* duration) / 10000) / 2;

  return distance;
}