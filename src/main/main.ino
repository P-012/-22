// Bluetooth
#include <MyBluetooth.h>
MyBluetooth bt(22, 9600); // 블루투스 객체

// LCD
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define COLS 16 // 열
#define ROWS 2 // 행
LiquidCrystal_I2C lcd(0x27, COLS, ROWS);

// HyperSonic
#define MAX_FRONT_DISTANCE 6
#define TRIG_front 24
#define ECHO_front 25
#define TRIG_inner 26
#define ECHO_inner 27

// ServoMotor
#include <Servo.h>
Servo servo_l;
Servo servo_r;

// DCMotor
const int ENA =  9;
const int ENB = 11;
const int IN1 =  7;
const int IN2 =  8;
const int IN3 = 10;
const int IN4 = 12;
const int BUZ =  6;
const byte encoder0pinA = 2;
const byte encoder0pinB = 5;
const byte encoder1pinA = 3;
const byte encoder1pinB = 4;
#define CW  0 // 시계 방향
#define CCW 1 // 반시계 방향
#define SPEED 100

void setup() {
  // Serial
  Serial.begin(9600);
  Serial.println("Select [Both NL & CR] in Serial Monitor");
  Serial.println("AT command"); 
  
  // Bluetooth
  bt.init();

  // LCD
  lcd.init();
  lcd.backlight();

  // HyperSonic
  pinMode(TRIG_front, OUTPUT);
  pinMode(ECHO_front, INPUT);
  pinMode(TRIG_inner, OUTPUT);
  pinMode(ECHO_inner, INPUT);

  // ServoMotor
  servo_l.attach(28);
  servo_r.attach(29);

  // DCMotor
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  pinMode(BUZ, OUTPUT);   // buzzer pin  
 	digitalWrite(BUZ, LOW); // disable buzzer
}

void loop() {
  long duration_front, distance_front;
  digitalWrite(TRIG_front, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_front, LOW);

  duration_front = pulseIn(ECHO_front, HIGH);
  distance_front = ((float)(340 * duration_front) / 10000) / 2;  
  Serial.println(distance_front);

  long duration_inner, distance_inner;
  digitalWrite(TRIG_inner, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_inner, LOW);

  duration_inner = pulseIn(ECHO_inner, HIGH);
  distance_inner = ((float)(340 * duration_inner) / 10000) / 2;  
  Serial.println(distance_inner);
  int capacity = 100 - distance_inner * 100 / 18;

  // ServoTest
  if (distance_front < 6) {

    servo_l.write(0);   
    servo_r.write(180);               
    delay(1500);         
    servo_l.write(40); 
    servo_r.write(140);      
    delay(1500);
    
  }
  
  // LCD
  lcd.clear();
  lcd.setCursor(0,1);    
  lcd.print(String("Capacity : ") + String(capacity) + String("%"));   
  bt.send("쓰레기통 용량 : " + String(capacity) + "%");
  delay(1000);
  
  // Bluetooth & DCMOTOR
  if (bt.available()) {       
    String direction = bt.receive();
    Serial.println(direction);
    
    if (direction.equals("s"))
    {
      stop();
      return;
    }
    move(direction, SPEED);
  }
}

void move(String direction, int speed)
{
  if (direction.equals("f")) // 앞으로
  { 
    move_forward(speed);
    return;
  }
  else if (direction.equals("b")) // 뒤로
  {
    move_backward(speed);
    return;
  }
  else if (direction.equals("l")) // 왼쪽으로
  {
    move_left(speed);
    return;
  }
  else if (direction.equals("r")) // 오른쪽으로
  {
    move_right(speed);
    return;
  }
}

void move_forward(int speed)
{
  motorA_direction(CW);
  motorB_direction(CCW);

  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void move_backward(int speed)
{
  motorA_direction(CCW);
  motorB_direction(CW);

  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void move_left(int speed)
{
  motorA_direction(CW);
  motorB_direction(CW);

  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);
}

void move_right(int speed)
{
  motorA_direction(CCW);
  motorB_direction(CCW);

  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);
}

void stop()
{
  //moter A stop
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);

  //moter B stop
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENB, 0);
}

void motorA_direction(int dir)
{
  if(dir) // dir == CCW // 반시계 방향
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);  
  }
  else    // dir == CW  // 시계 방향
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);  
  }
}

void motorB_direction(int dir)
{
  if(dir) // dir == CCW // 반시계 방향
  {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);  
  }
  else    // dir == CW  // 시계 방향
  {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);  
  }
}
