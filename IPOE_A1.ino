#define S1_threshold 920
#define S4_threshold 920
#define S2_min 540
#define S2_max 985
#define S3_min 570
#define S3_max 990

#define Motor_B 44
#define Motor_C 46

bool S1(){
  if(analogRead(A12) > S1_threshold){
    return true;
  }else{
    return false;
  }
}

bool S4(){
  if(analogRead(A8) > S4_threshold){
    return true;
  }else{
    return false;
  }
}

int S2(){
  return map(analogRead(A11), S2_max, S2_min, 0, 100);
}

int S3(){
  return map(analogRead(A9), S3_max, S3_min, 0, 100);
}

void Move(int Left_Pwr, int Right_Pwr){
    //Left_Pwr = map(Left_Pwr, 0, 100, 0 ,255);
    //Right_Pwr = map(Right_Pwr, 0, 100, 0 ,255);
    analogWrite(44, Left_Pwr);
    analogWrite(46, Right_Pwr);
}

void Stop(){
    analogWrite(44, 0);
    analogWrite(46, 0);
    analogWrite(12, 0);
    analogWrite(45, 0);
}

void Back(int Left_Pwr, int Right_Pwr){
    analogWrite(12, Left_Pwr);
    analogWrite(45, Right_Pwr);
}

float err = 0;
float err_old = 0;
float diff = 0;

void PD(float Kp, float Kd, int pwr){
    err = S2() - S3();
    diff = (err * Kp) + ((err - err_old) * Kd);
    Move(pwr + diff, pwr - diff);
    err_old = err;
}

void SenDbgView() {
//  Serial.print(analogRead(A8));
//  Serial.print(",");
//  Serial.print(analogRead(A9));
//  Serial.print(",");
//  Serial.print(analogRead(A11));
//  Serial.print(",");
//  Serial.println(analogRead(A12));
  Serial.print(S1());
  Serial.print(",");
  Serial.print(S4());
  Serial.print(",,");
  Serial.print(S2());
  Serial.print(",");
  Serial.print(S3());
  Serial.println(",");
  delay(100);
}

void setup() {
  Serial.begin(9600);
  //IR CNM10010
  pinMode(A8, INPUT);
  pinMode(A9, INPUT);
  pinMode(A11, INPUT);
  pinMode(A12, INPUT);
  //MotorPWM
  pinMode(12, OUTPUT);
  pinMode(44, OUTPUT);
  pinMode(45, OUTPUT);
  pinMode(46, OUTPUT);
  //Buzzer
  pinMode(35, OUTPUT);
} 

void loop() {
  if( S1() ){
    while (S3() > 40){
      Move(0, 100);
    }
  }else if ( S4() ){
    while (S2() > 40){
      Move(100, 0);
    }
  }else{
    PD(0.13, 2, 70);
  }
  
}
