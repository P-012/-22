/*

@Authorship: Chang-Hoon Lee

*/

#define BTSerial Serial1  // Mega 2560에서 TX3(D14), RX3(D15)에 연결하여 사용
#define EN 22  // AT 명령어를 사용하기 위해서는 EN핀을 HIGH로 해야함
#define BAUDRATE 9600

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

// 엔코더(Encoder)
#define LF 0
#define RT 1

int Lduration, Rduration;
//boolean LcoderDir,RcoderDir;
const byte encoder0pinA = 2;
const byte encoder0pinB = 5;
//byte encoder0PinALast;
const byte encoder1pinA = 3;
const byte encoder1pinB = 4;
//byte encoder1PinALast;

#define SPEED 100

void setup() {
  Serial.begin(9600);   
  BTSerial.begin(BAUDRATE); 

  pinMode(EN, OUTPUT);                   //HC-05,   EN
  digitalWrite(EN, HIGH); 
  Serial.println("Select [Both NL & CR] in Serial Monitor");
  Serial.println("AT command"); 

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
  //LcoderDir = true;
  //RcoderDir = true;	
  pinMode(8,INPUT);
  pinMode(9,INPUT);
  //attachInterrupt(LF, LwheelSpeed, RISING);
  //attachInterrupt(RT, RwheelSpeed, RISING);
}

void LwheelSpeed()
{
	if(digitalRead(encoder0pinB))
		Lduration++;
	else Lduration--;
/*  int Lstate = digitalRead(encoder0pinA);
  if((encoder0PinALast == LOW)&&Lstate==HIGH)
  {
    int val = digitalRead(encoder0pinB);
    if(val == LOW && LcoderDir)  LcoderDir = false; //Lreverse
    else if(val == HIGH && !LcoderDir)  LcoderDir = true;  //Lforward
  } 
  encoder0PinALast = Lstate;

  if(!LcoderDir)  Lduration++;
  else  Lduration--;
*/   
}

void RwheelSpeed()
{
	if(digitalRead(encoder1pinB))
		Rduration--;
	else Rduration++;
/*  int Rstate = digitalRead(encoder1pinA);
  if((encoder1PinALast == LOW)&&Rstate==HIGH)
  {
    int val = digitalRead(encoder1pinB);
    if(val == LOW && RcoderDir)  RcoderDir = false; //Rreverse
    else if(val == HIGH && !RcoderDir)  RcoderDir = true;  //Rforward
  }
  encoder1PinALast = Rstate;

  if(!RcoderDir)  Rduration--;
  else  Rduration++;
*/  
}