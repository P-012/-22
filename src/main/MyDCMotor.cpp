#include "Arduino.h"
#include "MyDCMotor.h"

#define CW  0 // 시계 방향
#define CCW 1 // 반시계 방향

MyDCMotor::MyDCMotor(int ena, int enb, int in1, int in2, int in3, int in4, int buz, byte encoder0pinA, byte encoder0pinB, byte encoder1pinA, byte encoder1pinB)
{
  int _ena = end;
  int _enb = enb;
  int _in1 = in1;
  int _in2 = in2;
  int _in3 = in3; 
  int _in4 = in4; 
  int _buz = buz;
  byte _encoder0pinA = encoder0pinA;
  byte _encoder0pinB = encoder0pinB;
  byte _encoder1pinA = encoder1pinA;
  byte _encoder1pinB = encoder1pinB;
}

MyDCMotor::MyDCMotor(int speed)
{
  int _ena = 9;
  int _enb = 11;
  int _in1 = 7;
  int _in2 = 8;
  int _in3 = 10; 
  int _in4 = 12; 
  int _buz = 6;
  byte _encoder0pinA = 2;
  byte _encoder0pinB = 5;
  byte _encoder1pinA = 3;
  byte _encoder1pinB = 4;
  _speed = speed;
}

void MyDCMotor::init()
{
  pinMode(_in1, OUTPUT);
  pinMode(_in2, OUTPUT);
  pinMode(_in3, OUTPUT);
  pinMode(_in4, OUTPUT);
  pinMode(_ena, OUTPUT);
  pinMode(_enb, OUTPUT);
  
  pinMode(_buz, OUTPUT);   // buzzer pin  
 	digitalWrite(_buz, LOW); // dis
}

void MyDCMotor::move(String direction)
{
  if (direction.equals("f")) // 앞으로
  { 
    move_forward();
    return;
  }
  else if (direction.equals("b")) // 뒤로
  {
    move_backward();
    return;
  }
  else if (direction.equals("l")) // 왼쪽으로
  {
    move_left();
    return;
  }
  else if (direction.equals("r")) // 오른쪽으로
  {
    move_right();
    return;
  }
}

void MyDCMotor::move_forward()
{
  motorA_direction(CW);
  motorB_direction(CCW);

  analogWrite(_ena, _speed);
  analogWrite(_enb, _speed);
}

void MyDCMotor::move_backward()
{
  motorA_direction(CCW);
  motorB_direction(CW);

  analogWrite(_ena, _speed);
  analogWrite(_enb, _speed);
}

void MyDCMotor::move_left()
{
  motorA_direction(CW);
  motorB_direction(CW);

  analogWrite(_ena, _speed);
  analogWrite(_enb, _speed);
}

void MyDCMotor::move_right()
{
  motorA_direction(CCW);
  motorB_direction(CCW);

  analogWrite(_ena, _speed);
  analogWrite(_enb, _speed);
}

void MyDCMotor::stop()
{
  //moter A stop
  digitalWrite(_in1, LOW);
  digitalWrite(_in2, LOW);
  analogWrite(_ena, 0);

  //moter B stop
  digitalWrite(_in1, LOW);
  digitalWrite(_in2, LOW);
  analogWrite(_enb, 0);
}

void MyDCMotor::motorA_direction(int dir)
{
  if(dir) // dir == CCW // 반시계 방향
  {
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, HIGH);  
  }
  else    // dir == CW  // 시계 방향
  {
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, LOW);  
  }
}

void MyDCMotor::motorB_direction(int dir)
{
  if(dir) // dir == CCW // 반시계 방향
  {
    digitalWrite(_in3, LOW);
    digitalWrite(_in4, HIGH);  
  }
  else    // dir == CW  // 시계 방향
  {
    digitalWrite(_in3, HIGH);
    digitalWrite(_in4, LOW);  
  }
}



