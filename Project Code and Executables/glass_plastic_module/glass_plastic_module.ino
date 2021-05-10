#include<Servo.h>
#define usEchoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define usTrigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04
#define setPinState( pin, state ) digitalWrite(pin,state);
#define setPinType( pin, mode ) pinMode(pin, mode);
long timeInterval; // variable for the timeInterval of sound wave travel
int proximity; // variable for the proximity measurement
Servo mServo;

int enablePinB = 10; //purple wire
int inputPin3 = 11; //yellow wire
int inputPin4 = 12; //brown wire

int enablePinA = 6; //
int inputPin1 = 8; //brown wire
int inputPin2 = 7; //orange wire


void stopDC(){
    setPinState(inputPin1, LOW);
    setPinState(inputPin2, LOW);
    
    setPinState(inputPin3, LOW);
    setPinState(inputPin4, LOW);
}

void startDC()
{
    analogWrite(enablePinA, 255);
    analogWrite(enablePinB, 255);

    setPinState(inputPin1, HIGH);
    setPinState(inputPin2, LOW);

    setPinState(inputPin3, LOW);
    setPinState(inputPin4, HIGH);
}

void setup() {
    Serial.begin(9600); 
    mServo.attach(5);
    mServo.write(135);
    //US 
    setPinType(usTrigPin, OUTPUT); // Sets the usTrigPin as an OUTPUT
    setPinType(usEchoPin, INPUT);
    //DC motor
    setPinType(enablePinA, OUTPUT);
    setPinType(inputPin1, OUTPUT);
    setPinType(inputPin2, OUTPUT);
    // Set all the motor control pins to outputs
    setPinType(enablePinB, OUTPUT);
    setPinType(inputPin3, OUTPUT);
    setPinType(inputPin4, OUTPUT);
    
    // Turn off motors 
    setPinState(inputPin1, LOW);
    setPinState(inputPin2, LOW);
    
    // Turn off motors
    setPinState(inputPin3, LOW);
    setPinState(inputPin4, LOW);
    delay(1000);
}

int ultraSonic(){
  setPinState(usTrigPin, LOW);
  delayMicroseconds(2);
  setPinState(usTrigPin, HIGH);
  delayMicroseconds(10);
  setPinState(usTrigPin, LOW);
  timeInterval = pulseIn(usEchoPin, HIGH);
  proximity = timeInterval * 0.034 / 2;
  return proximity;
}

void rotateServoMotorClockwise(Servo mServo,int startDegree,int endDegree)
{
    int angle;
    for(angle = startDegree; angle>=endDegree; angle-=10)     // command to move from 180 degrees to 0 degrees 
    {                                
        mServo.write(angle);              //command to rotate the servo to the specified angle
        delay(5);                       
    }
}

void rotateServoMotorAnticlockwise(Servo mServo,int startDegree,int endDegree)
{
    int angle;
    for(angle = startDegree; angle <=endDegree; angle += 10)    // command to move from 0 degrees to 180 degrees 
    {                                  
      mServo.write(angle);                 //command to rotate the servo to the specified angle
      delay(5);                       
    } 
}

void pushWasteFromBelt(Servo mServo)
{
    rotateServoMotorClockwise(mServo,135,85);
    startDC();
    delay(2000);
    stopDC();
  
    rotateServoMotorClockwise(mServo,85,60);
    delay(500);
    rotateServoMotorAnticlockwise(mServo,60,135);
    delay(500);
}

void loop() {
    char data;
    

    int dis = ultraSonic();
    if(dis>=2 && dis <=9)
    {
        stopDC();
        Serial.println("1");
        while(!Serial.available());
        data = Serial.read();
        Serial.flush();
        //data ='1';
        if(data == '1')
            pushWasteFromBelt(mServo);
        else
        {
            startDC();
            delay(800);
            stopDC();
            delay(500);
        }
    }
}
