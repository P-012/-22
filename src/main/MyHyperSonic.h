#ifndef MyHyperSonic_h
#define MyHyperSonic_h

#include "Arduino.h"

class MyHyperSonic
{
  public:
    MyHyperSonic(int trig, int echo);
    void init();
    long getDistance();

  private:
    int _trig;
    int _echo;
};

#endif