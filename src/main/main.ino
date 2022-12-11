// Bluetooth
#include <MyBluetooth.h>
MyBluetooth bt(22, 9600); // 블루투스 객체

// LCD
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define COLS 16 // 열
#define ROWS 3 // 행
char *comments[ROWS] = {
  "GarbageCollector",
  "Battery : 100%",
  "Capacity : 0%",
};

// HyperSonic
#include <MyHyperSonic.h>
#define MAX_FRONT_DISTANCE 10
MyHyperSonic front(24, 25);
MyHyperSonic inner(26, 27);

// ServoMotor
#include <MyServo.h>
MyServo servo_l(28);
MyServo servo_r(29);

// DCMotor
#include <MyDCMotor.h>
MyDCMotor dcm(100);

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
  front.init();
  inner.init();

  // ServoMotor
  servo_l.init();
  servo_r.init();

  // DCMotor
  dcm.init();
}

void loop() {
  // LCD
  for(int i=0; i<ROWS ; i++)
  {
    lcd.setCursor(0,i);    
    lcd.print(comment[i]);     
  }

  // Bluetooth & HyperSonic
  distance = front.getDistance();
  bt.send("전면 초음파 거리 : " + String(distance));
  distance_in = inner.getDistance();
  bt.send("내부 초음파 거리 : " + String(distance_in));
  
  // ServoMotor
  if (distance <= MAX_FRONT_DISTANCE) {
    servo_l.rotate(90);        
    servo_r.rotate(90);        
    delay(500);
    servo_l.rotate(0);        
    servo_r.rotate(0);        
    delay(500);
  }

  // Bluetooth & DCMOTOR
  if (bt.available()) {       
    String direction = bt.receive();
    Serial.println(direction);
    
    if (direction.equals("s"))
    {
      dcm.stop();
      return;
    }
    dcm.move(direction);
  }
}
