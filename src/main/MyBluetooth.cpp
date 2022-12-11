#include "Arduino.h"
#include "MyBluetooth.h"

#define BTSerial Serial1 // TX1, RX1

MyBluetooth::MyBluetooth(int en_pin, int baudrate)
{
  _en_pin = en_pin;
  _baudrate = baudrate;
}

void MyBluetooth::init()
{
  BTSerial.begin(_baudrate);
  pinMode(_en_pin, OUTPUT);
  digitalWrite(_en_pin, HIGH);
}

int MyBluetooth::available()
{
  return BTSerial.available();
}

void MyBluetooth::send(String str)  // 문자열 보내기
{
  BTSerial.println(str);
}

String MyBluetooth::receive() // 문자열 받기
{
  String str = BTSerial.readString();
  return str;
}