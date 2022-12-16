// Bluetooth
#include <MyBluetooth.h> // 블루투스 헤더파일 포함
MyBluetooth bt(22, 9600); // 블루투스 객체 생성(EN핀: 22, baudrate: 9600으로 설정)

// LCD
#include <LiquidCrystal_I2C.h> // LCD 헤더파일 포함
#include <Wire.h> // Wire 헤더파일 포함(I2C 통신에 필요)
#define COLS 16 // 열
#define ROWS 2 // 행
LiquidCrystal_I2C lcd(0x27, COLS, ROWS); // LCD 객체 생성

// HyperSonic
#define MAX_FRONT_DISTANCE 6 // 쓰레기가 놓여지지 않았다고 판단할 최대 거리(cm)
#define TRIG_front 24 // 전면 초음파 센서 TRIG 핀 번호
#define ECHO_front 25 // 전면 초음파 센서 ECHO 핀 번호
#define TRIG_inner 26 // 내부 초음파 센서 TRIG 핀 번호
#define ECHO_inner 27 // 내부 초음파 센서 ECHO 핀 번호

// ServoMotor
#include <Servo.h> // Servo 헤더파일 포함
Servo servo_l; // 왼쪽 서보모터 객체 생성
Servo servo_r; // 오른쪽 서보모터 객체 생성

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
#define SPEED 100 // DC 모터 속도 설정

void setup() {
  // Serial
  Serial.begin(9600); // 시리얼 입력 시작
  Serial.println("Select [Both NL & CR] in Serial Monitor"); // 시리얼 입력이 활성화되었음을 알리는 출력 문구
  Serial.println("AT command"); 
  
  // Bluetooth
  bt.init(); // 블루투스 통신 핀 설정(EN핀)

  // LCD
  lcd.init(); // lcd 객체 초기화(핀 설정?)
  lcd.backlight(); // lcd 불 켜기

  // HyperSonic 
  // 초음파 센서 핀 설정
  pinMode(TRIG_front, OUTPUT);
  pinMode(ECHO_front, INPUT);
  pinMode(TRIG_inner, OUTPUT);
  pinMode(ECHO_inner, INPUT);

  // ServoMotor
  // 서보 모터 핀 설정
  servo_l.attach(28);
  servo_r.attach(29);

  // DCMotor
  // DC 모터 핀 설정
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
  long duration_front, distance_front; // 전면 초음파 센서의 duration과 distance를 저장할 변수 선언
  // 초음파 센서 값 읽어오기
  digitalWrite(TRIG_front, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_front, LOW);

  duration_front = pulseIn(ECHO_front, HIGH); // 전면 초음파 센서 duration 가져오기
  distance_front = ((float)(340 * duration_front) / 10000) / 2; // 전면 초음파 센서에서 감지된 거리를 cm로 변환 후 distance에 저장
  Serial.println(distance_front); // 전면 초음파 센서에서 감지된 거리 출력(디버깅용)

  long duration_inner, distance_inner; // 내부 초음파 센서의 duration과 distance를 저장할 변수 선언(용량 확인용)
  // 초음파 센서 값 읽어오기
  digitalWrite(TRIG_inner, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_inner, LOW);

  duration_inner = pulseIn(ECHO_inner, HIGH); // 내부 초음파 센서 duration 가져오기
  distance_inner = ((float)(340 * duration_inner) / 10000) / 2;  // 내부 초음파 센서에서 감지된 거리를 cm로 변환 후 distance에 저장
  Serial.println(distance_inner); // 내부 초음파 센서에서 감지된 거리 출력(디버깅용)
  int capacity = 100 - distance_inner * 100 / 18; // distance를 용량 백분율로 변환 후 capacity에 저장

  // ServoMotor
  if (distance_front < 6) { // 전면 초음파 센서에서 감지된 거리가 6cm보다 작을 경우 쓰레기가 놓여졌다고 판단

    // 당기기
    servo_l.write(0);   
    servo_r.write(180);               
    delay(1500);         

    // 원래대로
    servo_l.write(40); 
    servo_r.write(140);      
    delay(1500);
    
  }
  
  // LCD
  lcd.clear(); // LCD 텍스트 초기화(초기화하지 않으면 텍스트가 쌓임)
  lcd.setCursor(0,1); // 커서 중앙으로 이동
  lcd.print(String("Capacity : ") + String(capacity) + String("%")); // 쓰레기통 용량 표시
  bt.send("쓰레기통 용량 : " + String(capacity) + "%"); // GCM 앱으로 용량 상태 전송
  delay(1000); // 딜레이 설정하지 않으면 LCD에 제대로 표시되지 않음
  
  // Bluetooth & DCMOTOR
  if (bt.available()) { // 블루투스가 통신 가능하다면
    String direction = bt.receive(); // 블루투스에서 입력된 문자열을 direction에 저장
    Serial.println(direction); // 입력된 문자열 표시(디버깅용)
    
    if (direction.equals("s")) // 입력된 문자열이 s일 경우 멈춤
    {
      stop();
      return;
    }
    move(direction, SPEED); // s가 아니라면 입력된 방향으로 이동
  }
}

void move(String direction, int speed) // 쓰레기통 이동 함수(방향과 이동속도를 입력 받음)
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

void move_forward(int speed) // 전진
{
  motorA_direction(CW);
  motorB_direction(CCW);

  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void move_backward(int speed) // 후진
{
  motorA_direction(CCW);
  motorB_direction(CW);

  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void move_left(int speed) // 좌회전
{
  motorA_direction(CW);
  motorB_direction(CW);

  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);
}

void move_right(int speed) // 우회전
{
  motorA_direction(CCW);
  motorB_direction(CCW);

  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);
}

void stop() // 정지
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

void motorA_direction(int dir) // 모터A 방향 설정
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

void motorB_direction(int dir) // 모터B 방향 설정
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
