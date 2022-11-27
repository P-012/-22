#if __AVR_ATmega2560__
    #define BTSerial Serial3                  // Mega 2560에서 TX3(D14), RX3(D15)에 연결하여 사용
#else
#include <SoftwareSerial.h> 
    int blueTx=2;                       
    int blueRx=3;                       
    SoftwareSerial BTSerial(blueTx, blueRx);  // Uno, Nano에서 Tx(D2), Rx(D3)에 연결하여 사용 
#endif
#define EN 4                            // AT 명령어를 사용하기 위해서는 EN핀을 HIGH로 해야함
#define BAUDRATE 9600              // AT 명령어 모드에서는 38400, 앱과 연동해서 사용할 때는 9600 로 설정

void setup() 
{
  // 직력 통신 설정
  Serial.begin(9600);   
  BTSerial.begin(BAUDRATE);

  // 블루투스 모듈 핀 설정
  pinMode(EN, OUTPUT); 
  digitalWrite(EN, HIGH);                

}
void loop()
{ 
  BTSerial.println("배터리 잔량: 30%");
  BTSerial.println("쓰레기통 잔량: 57%");
  delay(1000);
  BTSerial.println("배터리 잔량: 100%");
  BTSerial.println("쓰레기통 잔량: 10%");
  delay(1000);
}
