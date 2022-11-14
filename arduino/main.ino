// 블루투스 모듈(HC-05)
#if __AVR_ATmega2560__
    #define BTSerial Serial3                  // Mega 2560에서 TX3(D14), RX3(D15)에 연결하여 사용
#else
#include <SoftwareSerial.h> 
    int blueTx=2;                       
    int blueRx=3;                       
    SoftwareSerial BTSerial(blueTx, blueRx);  // Uno, Nano에서 Tx(D2), Rx(D3)에 연결하여 사용 
#endif
#define EN 4                            // AT 명령어를 사용하기 위해서는 EN핀을 HIGH로 해야함
#define BAUDRATE 38400                  // AT 명령어 모드에서는 38400, 앱과 연동해서 사용할 때는 9600 로 설정

// LCD 모듈
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

// 초음파 모듈
#define TRIG 8
#define ECHO 9

// 서보 모터
#include<Servo.h> 
Servo myservo; 
int servoPin = 12;
int pos = 0; 

void setup() 
{
  // 직력 통신 설정
  Serial.begin(9600);   
  BTSerial.begin(BAUDRATE);

  // 초음파 센서 핀 설정
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // 블루투스 모듈 핀 설정
  pinMode(EN, OUTPUT); 
  digitalWrite(EN, HIGH);                

  // LCD 모니터 설정
  lcd.init();       // I2C LCD 초기화
  lcd.backlight();  // LCD 백라이트를 켜기

  // 서보 모터 핀 설정
  myservo.attach(servoPin); 
}
void loop()
{ 
  // 블루투스
  if (BTSerial.available()) {            // 블루투스가 데이터를 받으면      
    Serial.write(BTSerial.read());       // 블루투스 데이터를 읽어 시리얼모니터로 보냄
  }
  if (Serial.available()) {              // 시리얼모니터가 데이터를 받으면
    BTSerial.write(Serial.read());       // 시리얼모니터 데이터를 읽어 블루투스로 보냄
  }

  // LCD 출력
  for(int i=0; i<ROWS ; i++)
  {
    lcd.setCursor(0,i);           // 커서 위치 지정: i행 
    lcd.print(comment[i]);     
  }

  // 초음파
  long duration, distance;
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH);
  distance = ((float)(340* duration) / 10000) / 2;

  Serial.print("\nDistance : ");
  Serial.print(distance);
  Serial.println(" cm");

  // 서보 모터
  if (distance <= 10) {
    myservo.write(90);        
    delay(500);
    myservo.write(0);        
    delay(500);
  }
}