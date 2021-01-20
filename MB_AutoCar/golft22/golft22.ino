#include <PID_v1.h>
#define PIN_OUTPUT 3
#define PIN_OUTPUT2 6
int opentmode = 16 ;
int FR = 14;
int OF = 15;
int PWMSPEED = 11 ;
int SW;
int C;
double Setpoint, Feedback, Output;
int b0 = 0, b1 = 0, b2 = 0, b3 = 0, b4 = 0, b5 = 0, b6 = 0, b7 = 0, b8 = 0, b9 = 0, b10 = 0, b11 = 0, b12 = 0;
double Kp = 5, Ki = 0, Kd = 0;
int led1 = 4;
int led2 = 5;
int relay1 = 7;
int relay2 = 8;
int CH1, CH2, CH3, CH4;
int limitSW = 9;
PID myPID(&Feedback, &Output, &Setpoint, Kp, Ki, Kd, DIRECT); //เปิดการใช้งาน PID
int lower;
int upper;
void setup()
{
  pinMode(opentmode, OUTPUT);
  pinMode(FR, OUTPUT);
  pinMode(OF, OUTPUT);
  pinMode(A8, INPUT);
  pinMode(A9, INPUT);
  pinMode(A10, INPUT);
  pinMode(A11, INPUT);
  pinMode(A12, INPUT);
  pinMode(PWMSPEED, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(limitSW, INPUT);
  pinMode(22, INPUT_PULLUP);
  pinMode(24, INPUT_PULLUP);
  pinMode(26, INPUT_PULLUP);
  pinMode(28, INPUT_PULLUP);
  pinMode(30, INPUT_PULLUP);
  pinMode(32, INPUT_PULLUP);
  pinMode(34, INPUT_PULLUP);
  pinMode(36, INPUT_PULLUP);
  pinMode(38, INPUT_PULLUP);
  pinMode(40, INPUT_PULLUP);
  pinMode(42, INPUT_PULLUP);
  pinMode(44, INPUT_PULLUP);
  pinMode(46, INPUT_PULLUP);
  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(1);
  myPID.SetOutputLimits(-150, 150);
  Serial.begin(19200);
  lower = 1000;
  upper = 1900;

}
void loop()
{
  digitalWrite(opentmode, LOW);
  Setpoint = pulseIn(A8, HIGH, 250000);
  CH1 = pulseIn(A10, HIGH, 250000);
  CH2 = pulseIn(A11, HIGH, 250000);
  CH3 = pulseIn(A12, HIGH, 250000);
  CH4 = pulseIn(A9, HIGH, 250000);

  //TRUN
  Setpoint = constrain(Setpoint, lower , upper);
  Setpoint = map(Setpoint, lower , upper, 80, 980);
  //speed
  CH1 = constrain(CH1, lower , upper);
  CH1 = map(CH1, 900 , upper, 0, 200);
  analogWrite(PWMSPEED, CH1);
  //For,Back
  CH2 = map(CH2, 950 , 1900, 1, 0);
  digitalWrite(FR, CH2);
  //ON,OFF car
  CH3 = map(CH3, 950 , 1900, 1, 0);
  digitalWrite(OF, CH3);
  //BREAK
  SW = digitalRead (limitSW);
  Serial.println(SW);
  if (SW == 0 && CH4 < 1600 )
  {
    analogWrite(PIN_OUTPUT2, 0);
    digitalWrite(relay1 , LOW);
    digitalWrite(relay2 , LOW);
  }
  if (CH4 > 1600)
  {
    C = map(CH4, 1600, upper, 0, 120);
    analogWrite(PIN_OUTPUT2, C);
    digitalWrite(relay1 , LOW);
    digitalWrite(relay2 , HIGH);
  }
  if (CH4 < 1600 && SW == 1) //สติ๊กซ้าย เดินหน้า
  {
    analogWrite(PIN_OUTPUT2, 80);
    digitalWrite(relay1 , HIGH);
    digitalWrite(relay2 , LOW);
  }
  //TRUN
  b0 = digitalRead(22) * 1;
  b1 = digitalRead(24) * 2;
  b2 = digitalRead(26) * 4;
  b3 = digitalRead(28) * 8;
  b4 = digitalRead(30) * 10;
  b5 = digitalRead(32) * 20;
  b6 = digitalRead(34) * 40;
  b7 = digitalRead(36) * 80;
  b8 = digitalRead(38) * 100;
  b9 = digitalRead(40) * 200;
  b10 = digitalRead(42) * 400;
  b11 = digitalRead(44) * 800;
  b12 = digitalRead(46) * 1000;
  int sum = b0 + b1 + b2 + b3 + b4 + b5 + b6 + b7 + b8 + b9 + b10 + b11 + b12;
  int ss = sum % 1000;
  Feedback = ss;
  double error = Setpoint - Feedback;
  if ( error < 10 && error > -10)
  {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
  }
  else if (error > 10)
  {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
  }
  else if (error < -10)
  {
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
  }
  myPID.Compute();
  if (abs(error) < 10)
  {
    Output = 0 ;
    analogWrite(PIN_OUTPUT, 0);
  }
  else
  {
    analogWrite(PIN_OUTPUT, abs(Output));
  }
}
