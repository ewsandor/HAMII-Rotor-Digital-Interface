
const int left =  8;
const int brake = 9;
const int right = 2;
const int potent = 0;
const int TOLERANCE = 1;
const int AMAX = 940;
const int AMIN = 50;
int leftCount;
int rightCount;
int brakeCount;
int   offCount;
int idleCount;
int apCount;
int target;
boolean brakeOff;
boolean turnRight;
boolean turnLeft;
boolean autoRot;
String serialRead;


void setup() {
  for(int i = 0; i<8; i++){
    pinMode(i+2, OUTPUT);
    digitalWrite(i+2, LOW);
  }
    
    
  serialRead="";
  Serial.begin(4800);
  pinMode(left , OUTPUT);      
  pinMode(right, OUTPUT);     
  pinMode(brake, OUTPUT);     
  
  digitalWrite(left, LOW);
  digitalWrite(right, LOW);
  digitalWrite(brake, LOW);
  
  leftCount = 0;
  rightCount = 0;
  brakeCount = 0;
  idleCount = 0;
  target=0;
  brakeOff=false;
  turnLeft=false;
  turnRight=false;
  stopRot();
  apCount = 0;
}

void loop()
{
  //Serial.println(analogRead(potent));
  delay(1);
  idleCount++;
   
    
  if(idleCount > 2000 && brakeCount > 1000 && !turnLeft && !turnRight){
    brakeOff=false;
  }  
  if(brakeOff){
    brakeCount++;    
    digitalWrite(brake, HIGH);
  }
  else{
    digitalWrite(brake, LOW);
    brakeCount=0;
  }
  
  if(turnLeft && turnRight){
    turnLeft = false;
    turnRight = false;
    stopRot();
  }
  
  
  ////////////////////LEFT TURN/////////////////
  if(turnLeft){
    brakeOff = true;

    if(idleCount > 2000 && brakeCount>1000){
      digitalWrite(left, HIGH);
      leftCount++;
    }
    if(leftCount > 500){
      idleCount=0;
      turnLeft = false;
    }
  }
  else
      digitalWrite(left, LOW);
  ////////////////////RIGHT TURN/////////////////
  if(turnRight){
    brakeOff = true;

    if(idleCount > 2000 && brakeCount>1000){
      digitalWrite(right, HIGH);
      rightCount++;
    }
    if(rightCount > 500){
      idleCount=0;
      turnRight = false;
    }
  }
  else{
      digitalWrite(right, LOW);
  }
  ///////////////////Auto Rotate//////////////////
  int head=getHeading();
  if(autoRot){
    if(abs(head-target)<TOLERANCE){
      stopRot();
      turnLeft=false;
      turnRight=false;
    }
    else if(head>target){
      turnLeft=true;
      turnRight=false;
      leftCount=0;
    }
    else if(head<target){
      turnLeft=false;
      turnRight=true;
      rightCount=0;
    }
  }
}

void serialEvent(){
      
  char inChar='e';
  
    //delay(200);
  while(Serial.available()>0){
    inChar = (char)Serial.read();
    
    switch (inChar){
    case 'b':
      autoRot=false;
      brakeCount = 0;
      brakeOff=true;
      break;
    case 'l':
      autoRot=false;
      leftCount=0;
      turnLeft=true;
      break;
    case 'r':
      autoRot=false;
      rightCount=0;
      turnRight=true;
      break;
    case 'c':
      autoRot=true;
      break;
    case 's':
      stopRot();
      break;
    case 'g':
      reportHeading();
      break;
    case 'w':
      Serial.println(analogRead(potent));
      break;
    }
    
    if(inChar == 'A'){
      serialRead="A";
    }
    else if(serialRead.length()==1 && serialRead.compareTo("A") == 0)
    {
      
      if(inChar=='P' || inChar=='M' || inChar=='I'){
        serialRead=serialRead + inChar;
      }
    }
    else if(serialRead.length()==2 && (serialRead.compareTo("AP")==0 || serialRead.compareTo("AM")==0 || serialRead.compareTo("AI")==0) && inChar=='1'){
        serialRead=serialRead + inChar;
    }
    else if(serialRead.length()==3 && serialRead.compareTo("AM1") == 0 && inChar==';'){
        autoRot=true;
        serialRead="";
    }
    else if(serialRead.length()==3 && serialRead.compareTo("AI1") == 0 && (inChar==';' || inChar=='\r')){
      
        reportHeading();
        serialRead="";
    }
    else if(serialRead.length()>=3 && serialRead.length()<6 && inChar >= '0' && inChar <='9'){
        serialRead=serialRead+inChar;
    }
    else if(serialRead.length()==6){
      if(inChar == ';'){
        stopRot();
      }
      else if(inChar == '\r'){
        autoRot=true;
      }
      else{
        serialRead="";
      }
      if(serialRead.length()==6){
        int tmp = serialRead.substring(3).toInt();
        if(tmp > 180)
          tmp=tmp-360;
        target = tmp;
      }
      serialRead="";
    }
    else if(inChar==';'){
        stopRot();
        serialRead="";
    }
    else
        serialRead="";
  }
  
  
  
  
}
int getHeading(){
  int raw = analogRead(potent);
  int tmp = ( (int) 360*( ( (double) raw-AMIN )/( AMAX-AMIN ) ) )-180;
  return tmp;
}
void reportHeading(){
  int loc = (getHeading());
  while(loc<0)
    loc=loc+360;
  String outStr=";";
  outStr=outStr+loc;
  Serial.println(outStr);
}
void stopRot(){
    autoRot = false;
    turnLeft = false;
    turnRight = false;
    brakeCount = 0;
    idleCount=0;
    brakeOff= true;
}

