#ifndef MyDCMotor_h
#define MyDCMotor_h

#include "Arduino.h"

class MyDCMotor
{
  public:
    MyDCMotor(int ena, int enb, int in1, int in2, int in3, int in4, int buz, byte encoder0pinA, byte encoder0pinB, byte encoder1pinA, byte encoder1pinB);
    MyDCMotor(int speed);
    void init();
    void move(String direction);
    void move_forward();
    void move_backward();
    void move_left();
    void move_right();
    void motorA_direction(int dir);
    void motorB_direction(int dir)
    void stop();

  private:
    int _ena;
    int _enb;
    int _in1;
    int _in2;
    int _in3; 
    int _in4; 
    int _buz;
    byte _encoder0pinA;
    byte _encoder0pinB;
    byte _encoder1pinA;
    byte _encoder1pinB;
    int _speed;
};

#endif