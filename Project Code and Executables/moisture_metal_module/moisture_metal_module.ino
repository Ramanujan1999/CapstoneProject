#include<Servo.h> //Include Servo Library.
 
#define usEchoPin 2 // Connect D2 pin of Arduino to Echo pin of Ultrasonic sensor : white wire
#define usTrigPin 3 // Connect D3 pin of Arduino to Trigger pin of Ultrasonic sensor : yellow wire
#define setPinState( pin, state ) digitalWrite(pin,state);
#define setPinType( pin, mode ) pinMode(pin, mode);

int enablePinB = 10; //purple wire
int inputPin3 = 11; //yellow wire
int inputPin4 = 12; //brown wire

int enablePinA = 6; //
int inputPin1 = 8; //brown wire
int inputPin2 = 7; //orange wire

//Servo motor
int moistureSensorPin = A0; // moisture detection sensor
int moistureSensorOutput ;

//vcc - red wire
//gnd - violet wire


// proximitys greater than 2cm and less than 9cm is taken into consideration


// defines variables
long timeInterval; // For time taken for the sound wave to travel.
int proximity; // For proximity calculation.


Servo mMoistureServo;               
Servo mIpsServo;

//IPS - brown=+12V  blue=-12V  black= Near green resistor end
int IPS_TO_ARDUINO = 13;
int STATE = LOW;


void stop() {
    setPinState(inputPin1, LOW);
    setPinState(inputPin2, LOW);
    setPinState(inputPin3, LOW);
    setPinState(inputPin4, LOW);
}

void start()
{
    analogWrite(enablePinA, 255);
    analogWrite(enablePinB, 255);

    setPinState(inputPin1, HIGH);
    setPinState(inputPin2, LOW);

    setPinState(inputPin3, LOW);
    setPinState(inputPin4, HIGH);
}

void setup() {
    setPinType(usTrigPin, OUTPUT); // usTrigPin is set as an OUTPUT.
    setPinType(usEchoPin, INPUT); // usEchoPin is set as an INPUT.
    Serial.begin(9600); // 9600 of baudrate speed is used to begin the Serial Communication.
    Serial.println("Testing of the Ultrasonic sensor"); // displays it on Serial Monitor.
    Serial.println("with Arduino UNO R3");


    mMoistureServo.attach(9);
    mIpsServo.attach(5);
    mMoistureServo.write(135);
    mIpsServo.write(135);
  
    setPinType(enablePinA, OUTPUT);
    setPinType(inputPin1, OUTPUT);
    setPinType(inputPin2, OUTPUT);
    // The motor control pins are set to outputs.
    setPinType(enablePinB, OUTPUT);
    setPinType(inputPin3, OUTPUT);
    setPinType(inputPin4, OUTPUT);
    
    //start state :  Powering off the motors
    setPinState(inputPin1, LOW);
    setPinState(inputPin2, LOW);
    
    setPinState(inputPin3, LOW);
    setPinState(inputPin4, LOW);

     //IPS
     setPinType(IPS_TO_ARDUINO,INPUT);
  
     delay(1000);
}



int isMoisture()
{
    moistureSensorOutput= analogRead(moistureSensorPin);
    moistureSensorOutput = map(moistureSensorOutput,550,0,0,100);
    Serial.print("Mositure : ");
    Serial.print(moistureSensorOutput);
    Serial.println("%");
    if(moistureSensorOutput > -75)
    {
        Serial.println("wet waste");
        return 1;
    }
    Serial.println("dry waste");
    return 0;
}

int isMetal() 
{
    int mSensorOutput = digitalRead(IPS_TO_ARDUINO);

    if( mSensorOutput != STATE )
    {
       STATE = mSensorOutput;
       if(STATE == 0)
         return 1;
       else
         return 0;
    }
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
    start();
    delay(2200);
    stop();
  
    rotateServoMotorClockwise(mServo,85,55);
    delay(200);
    rotateServoMotorAnticlockwise(mServo,55,135);
}

void loop() {
    // usTrigPin condition is cleared.
    setPinState(usTrigPin, LOW);
    delayMicroseconds(2);

    // usTrigPin is set high for 10 microseconds.
    setPinState(usTrigPin, HIGH);
    delayMicroseconds(10);

    setPinState(usTrigPin, LOW);

    // usEchoPin is read, time taken for the sound wave to travel is returned.
    timeInterval = pulseIn(usEchoPin, HIGH);

    // proximity calculation.
    proximity = timeInterval * 0.034 / 2; // half of the speed of the sound wave (forward and backward). 
  
    Serial.println(proximity);
  
    if(proximity>=2 && proximity<9)
    {
             
        start();
        delay(200);
        stop();
        
        delay(1500);
        if(isMoisture())
        {
            pushWasteFromBelt(mMoistureServo);
        }
        else
        {
            start();
            delay(1100);
            stop();
            delay(1500);
            if(isMetal())
            {
                pushWasteFromBelt(mIpsServo);   
            }
            else
            {
                start();
                delay(1000);
                stop();  
            }
        }
    } 
}
