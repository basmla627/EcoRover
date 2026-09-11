#include<Servo.h>

void forward();
void back();
void right();
void left();
void flames();
# define soilPin       
# define mq2Pin             
# define waterLevelPin  
# define relayPin

# define flame1Pin 
# define flame2Pin 
# define flame3Pin 
# define flame4Pin 
# define flame5Pin   

#define left1 7
#define left2 8
#define right1 9
#define right2 4
#define en1 5
#define en2 3

Servo seed,pumb,soil,dig;
bool waterlev=false;
\\ دى مش قيم حقيقيه محتاجه تتقاس و نحددها بالظبط 
int soilMin = 1000, soilMax = 3000;
int gasLimit = 2000;
int waterLevelMin = 1500;
int flame1,flame2,flame3,flame4,flame5;

void setup() {

  Serial.begin(115200);
  pinMode(flamePin, INPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  pinMode(left1,OUTPUT);
  pinMode(left2,OUTPUT);
  pinMode(en1,OUTPUT);
  pinMode(right1,OUTPUT);
  pinMode(right2,OUTPUT);
  pinMode(en2,OUTPUT);
  seed.attach();
  pumb.attach(110);
  soil.attach();
  dig.attach();
  Serial.println("System Started");
}


void loop() {
  int soilValue = analogRead(soilPin);
  Serial.print("Soil Moisture: ");
  Serial.println(soilValue);
   //send to app the soil value 
  if (soilValue >= soilMin && soilValue <= soilMax) {
    Serial.println("Soil isn't Dry");
    digitalWrite(relayPin, LOW);
    //send to app
  } 
  else {
    Serial.println("Soil is Dry");
    digitalWrite(relayPin, HIGH);
    //warning to app
  }

  int gasValue = analogRead(mq2Pin);
  Serial.print("Gas Value: ");
  Serial.println(gasValue);
  //send to app
  if (gasValue > gasLimit) {
    Serial.println("WARNING! Harmful Gas Detected!");
    // بعدين هنبعت Alert للـMIT App
  } else {
    Serial.println("Gas Level is Normal");
  }

  int waterlev=readanalog(waterLevelPin);
  //waterlev t app 
  while(waterlev<waterLevelMin){
    //warning to app 
    Serial.println("Water not enough");
  }


  flame1=digitalRead(flame1Pin);
  flame2=digitalRead(flame2Pin);
  flame3=digitalRead(flame3Pin);
  flame4=digitalRead(flame4Pin);
  flame5=digitalRead(flame5Pin);
  flames();
}

void left(){
  digitalWrite(right1,HIGH);
    digitalWrite(right2,LOW);
    digitalWrite(en1,HIGH);
    digitalWrite(left1,LOW);
    digitalWrite(left2,HIGH);
    digitalWrite(en2,HIGH); 
}

void right(){
  digitalWrite(right1,LOW);
    digitalWrite(right2,HIGH);
    digitalWrite(en1,HIGH);
    digitalWrite(left1,HIGH);
    digitalWrite(left2,LOW);
    digitalWrite(en2,HIGH);
}

void back(){
  digitalWrite(right1,LOW);
    digitalWrite(right2,HIGH);
    digitalWrite(en1,HIGH);
    digitalWrite(left1,LOW);
    digitalWrite(left2,HIGH);
    digitalWrite(en2,HIGH);
}

void forward(){
  digitalWrite(right1,HIGH);
    digitalWrite(right2,LOW);
    digitalWrite(en1,HIGH);
    digitalWrite(left1,HIGH);
    digitalWrite(left2,LOW);
    digitalWrite(en2,HIGH);
}

void flames(){

  if(flame1==0 && flame2==0 && flame3==0 && flame4==0 && flame5==0){
    //warning sent to app
    digitalWrite(relayPin,HIGH);
    for(int i=110;i<180;i++){
      pumb.write(i);
      delay(50);
    }
    for(int i=180;i==110;i--){
      pumb.write(i);
      delay(50);
    }

  }
  //momken n send lel app direction el flame
  else if(flame1==0){
    pumb.write(50);
    digitalWrite(relayPin,HIGH);
  }
  else if(flame2==0){
    pumb.write(80);
    digitalWrite(relayPin,HIGH);
  }
  else if(flame2==0){
    pumb.write(110);
    digitalWrite(relayPin,HIGH);
  }
  else if(flame4==0){
    pumb.write(140);
    digitalWrite(relayPin,HIGH);
  }
  else if(flame5==0){
    pumb.write(170);
    digitalWrite(relayPin,HIGH);
  }
  else{
    digitalWrite(relayPin,LOW);
    pumb.write();
  }

}




