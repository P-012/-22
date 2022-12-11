// Bluetooth
#define BTSerial Serial1 // TX1, RX1
#define EN 22 // EN 핀
#define BAUDRATE 9600

// LCD
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define ROWS 3 // 행(줄) 수 
char *comment[ROWS] = {
  "GarbageCollector",
  "Battery : 100%",
  "Capacity : 0%",
};
int col[ROWS] = {0, 2, 4};
LiquidCrystal_I2C lcd(0x27, 16, ROWS);

// HyperSonic
#define TRIG_front 24
#define ECHO_front 25
#define TRIG_in 26
#define TRIG_in 27

// ServoMotor
#include <Servo.h>
Servo servo_l;
Servo servo_r;
int servoPin_l = 28;
int servoPin_r = 29;
int servoPos_l = 0;
int servoPos_l = 0;

// DCMotor
const int ENA =  9;
const int ENB = 11;
const int IN1 =  7;
const int IN2 =  8;
const int IN3 = 10;
const int IN4 = 12;
#define CW  0 // 시계 방향
#define CCW 1 // 반시계 방향
const int BUZ =  6;
int MAS, MBS;
#define LF 0
#define RT 1
int Lduration, Rduration;
const byte encoder0pinA = 2;
const byte encoder0pinB = 5;
const byte encoder1pinA = 3;
const byte encoder1pinB = 4;
#define SPEED 100

void setup() {
  // Bluetooth
  Serial.begin(9600);
  BTSerial.begin(BAUDRATE);
  pinMode(EN, OUTPUT);
  digitalWrite(EN, HIGH);

  // LCD
  lcd.init();
  lcd.backlight();

  // HyperSonic
  pinMode(TRIG_front, OUTPUT);
  pinMode(ECHO_front, INPUT);
  pinMode(TRIG_in, OUTPUT);
  pinMode(ECHO_in, INPUT);

  // ServoMotor
  servo_l.attach(servoPin_l);
  servo_r.attach(servoPin_r);

  // DCMotor
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  // encoder pin
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  pinMode(BUZ, OUTPUT);   // buzzer pin  
 	digitalWrite(BUZ, LOW); // disable buzzer
}

void loop() {
  // Bluetooth
  if (BTSerial.available()) {            // 블루투스가 데이터를 받으면      
    Serial.write(BTSerial.read());       // 블루투스 데이터를 읽어 시리얼모니터로 보냄
  }
  if (Serial.available()) {              // 시리얼모니터가 데이터를 받으면
    BTSerial.write(Serial.read());       // 시리얼모니터 데이터를 읽어 블루투스로 보냄
  }

  // LCD
  for(int i=0; i<ROWS ; i++)
  {
    lcd.setCursor(0,i);           // 커서 위치 지정: i행 
    lcd.print(comment[i]);     
  }

  // HyperSonic
  long duration, distance;
  digitalWrite(TRIG_front, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_front, LOW);

  duration = pulseIn(ECHO_front, HIGH);
  distance = ((float)(340* duration) / 10000) / 2;

  Serial.print("\nDistance : ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // ServoMotor
  if (distance <= 10) {
    servo_l.write(90);        
    servo_r.write(90);        
    delay(500);
    servo_l.write(0);        
    servo_r.write(0);        
    delay(500);
  }

  // DCMOTOR
  if (BTSerial.available()) {       
    String direction = BTSerial.readString();
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

void EnCoderInit()
{
  pinMode(8,INPUT);
  pinMode(9,INPUT);
}

void LwheelSpeed()
{
	if(digitalRead(encoder0pinB))
		Lduration++;
	else Lduration--;
}

void RwheelSpeed()
{
	if(digitalRead(encoder1pinB))
		Rduration--;
	else Rduration++;
}