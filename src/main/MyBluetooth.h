#ifndef MyBluetooth_h
#define MyBluetooth_h

#include "Arduino.h"

class MyBluetooth
{
  public:
    MyBluetooth(int en_pin, int baudrate);
    void init();
    int available();
    void send(String str);
    String receive();

  private:
    int _en_pin;
    int _baudrate;
};

#endif