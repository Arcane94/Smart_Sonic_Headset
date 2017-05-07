//State machine states
enum STATE {OFF,ON,LOGOUT};

//Pin variables
#define VH A0
#define VL A1
#define S1 6
#define S2 7
#define RELAY 4
#define RELAY_C 5

#define ST1 10
#define ST2 11
#define ST3 12

//Current state is OFF
STATE current=OFF;

void setup() {

//Set the I/O
  pinMode(VH,INPUT);
  pinMode(VL,INPUT);
  pinMode(S1,INPUT);
  pinMode(S2,INPUT);
  pinMode(RELAY,OUTPUT);
  pinMode(RELAY_C,OUTPUT);
  pinMode(ST1,OUTPUT);
  pinMode(ST2,OUTPUT);
  pinMode(ST3,OUTPUT);
  
  Serial.begin(9600);  
}

void loop() {

//State machine
while(1){

 if(current == OFF){
    if((isOFF() == 0) & (isRemoved()== 0)){
      current=ON;
      sendOnOFF();
    }     
 }else if(current == ON){
    if(isOFF()){
      current=OFF;
    }else if(isRemoved() == 1){
      current=LOGOUT;
      sendOnOFF();
    }
 }else if(current == LOGOUT){
    if(isOFF()){
      current=OFF;
    }else if((isRemoved() == 0)){
       current=ON;
       sendOnOFF();
    } 
 }
 printCurrent();
 delay(300);

}
}

//Read the voltage values in VH,VL pins and get voltage
//Determine whether the headset is plugged in or not
int isOFF(){
  int voltDiff=analogRead(VH)-analogRead(VL);
  Serial.print("VDidd: ");
  Serial.print(voltDiff);
  Serial.print("\n");

  if(voltDiff > 300){
     return 0;  
  }else{
     return 1;  
  }
  }
 
void printCurrent(){

  digitalWrite(ST1,LOW);
  digitalWrite(ST2,LOW);
  digitalWrite(ST3,LOW);
  
  if(current == OFF){
     Serial.println("OFF");
     digitalWrite(ST1,HIGH);  
  }else if(current == ON){
     Serial.println("ON");
     digitalWrite(ST2,HIGH);
  }else{
    Serial.println("LOGOUT");
    digitalWrite(ST3,HIGH);
  }
  
}

//Check the touch sensor readings to determine weather the headset is puton
int isRemoved(){
  int v=(digitalRead(S1) + digitalRead(S2))/2;
  Serial.print("sensors: ");
  Serial.print(v);
  Serial.print("\n");
  return v;
}


//Send a pulse signal through 3.5mm jack turn OFF/ON the music player     
void sendOnOFF(){
   digitalWrite(RELAY_C,HIGH);
   delay(100);
   
   digitalWrite(RELAY,0); 
   delay(150);        
   digitalWrite(RELAY,1);
   delay(100);
   digitalWrite(RELAY_C,LOW);
   delay(100);
}
