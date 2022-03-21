#include <AFMotor.h>  
#include <NewPing.h>
#include <Servo.h> 
#include <Adafruit_NeoPixel.h>


//constante senzor ultrasunete
#define TRIG_PIN A5
#define ECHO_PIN A4 
#define MAX_DISTANCE 200 

//constante senzori infrarosii
#define MAX_SPEED 190 
#define MAX_SPEED_OFFSET 20
#define left A1
#define right A2
#define pinLed 14

//instantiere senzor ultrasunete
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 

//instantiere strip led-uri
Adafruit_NeoPixel strip = Adafruit_NeoPixel(6, 14, NEO_GRB + NEO_KHZ800);

//instantiere motoare
AF_DCMotor motor1(1, MOTOR12_1KHZ); 
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

//instantiere servormotor
Servo myservo;   

//variabile auxiliare
boolean goesForward=false;
int distance = 100;
int speedSet = 0;

void setup() {

  //configuare pini
  pinMode(pinLed, OUTPUT); 
  pinMode(left,INPUT);
  pinMode(right,INPUT);
  
  //pornire strip led-uri
  strip.begin();
  
  //pornire servormotor
  myservo.attach(10);  
  
  //resetare pozitie servomotor
  myservo.write(152); 
  
  //test senzor 
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
}

void loop() {

////////////FUNCTIONALITATE LINE FOLLOWER//////////////////////////////////////////////////

  //linia nedetectata
  if(digitalRead(left)==0 && digitalRead(right)==0){
	  motor1.run(RELEASE); 
    motor2.run(RELEASE);
    motor3.run(RELEASE);
    motor4.run(RELEASE);
  }
  //lina detectata de catre senzorul stang
  else if(digitalRead(left)==0 && !analogRead(right)==0){
    //rotire stanga
    motor1.run(FORWARD);
    motor1.setSpeed(180);
    motor2.run(FORWARD);
    motor2.setSpeed(180);
    motor3.run(BACKWARD);
    motor3.setSpeed(180);
    motor4.run(BACKWARD);
    motor4.setSpeed(180);
  }
  
  //linia detectata de catre senzorul dreapta
  else if(!digitalRead(left)==0 && digitalRead(right)==0){
    //rotire dreapta
    motor1.run(BACKWARD);
    motor1.setSpeed(180);
    motor2.run(BACKWARD);
    motor2.setSpeed(180);
    motor3.run(FORWARD);
    motor3.setSpeed(180);
    motor4.run(FORWARD);
    motor4.setSpeed(180);
  }
  //line detectata de catre cei doi senzori
  else if(!digitalRead(left)==0 && !digitalRead(right)==0){
	  motor1.run(FORWARD);
    motor1.setSpeed(120);      
    motor2.run(FORWARD);
    motor2.setSpeed(120);
    motor3.run(FORWARD); 
    motor3.setSpeed(120);
    motor4.run(FORWARD);     
    motor4.setSpeed(120);
    obstacleAvoid();
  }

}


////////////FUNCTII FOLOSITE//////////////////////////////////////////////////

	int lookRight()
	{
		myservo.write(80); 
		delay(500);
		int distance = readPing();
		delay(100);
		myservo.write(152); 
		return distance;
	}

	int lookLeft()
	{
		myservo.write(180); 
		delay(500);
		int distance = readPing();
		delay(100);
		myservo.write(152); 
		return distance;
		delay(100);
	}

	void moveStop() {
	  motor1.run(RELEASE); 
	  motor2.run(RELEASE);
	  motor3.run(RELEASE);
	  motor4.run(RELEASE);
	  } 
	  
	void moveForward() {

	 if(!goesForward)
	  {
		goesForward=true;
		motor1.run(FORWARD);
    motor1.setSpeed(200);      
		motor2.run(FORWARD);
    motor2.setSpeed(200);
		motor3.run(FORWARD); 
    motor3.setSpeed(200);
		motor4.run(FORWARD);     
    motor4.setSpeed(200);
    
    
    
	  }
	}

	void moveBackward() {
		goesForward=false;
		motor1.run(BACKWARD);      
		motor2.run(BACKWARD);
		motor3.run(BACKWARD);
		motor4.run(BACKWARD);  
	  for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
	  {
		motor1.setSpeed(speedSet);
		motor2.setSpeed(speedSet);
		motor3.setSpeed(speedSet);
		motor4.setSpeed(speedSet);
		delay(5);
	  }
	}  

	void turnLeft() {
	  chase_left(strip.Color(255, 0, 255)); 
	  motor1.run(FORWARD);
    motor1.setSpeed(200);
	  motor2.run(FORWARD);
    motor2.setSpeed(200);
	  motor3.run(BACKWARD);
    motor3.setSpeed(200);
	  motor4.run(BACKWARD);  
    motor4.setSpeed(200);   
	  delay(500);
	  motor1.run(FORWARD);      
	  motor2.run(FORWARD);
	  motor3.run(FORWARD);
	  motor4.run(FORWARD);      
	} 
	 
	void turnRight() {
	  chase_right(strip.Color(255, 0, 255)); 
	  motor1.run(BACKWARD);     
    motor1.setSpeed(200);
	  motor2.run(BACKWARD);  
    motor2.setSpeed(200);
	  motor3.run(FORWARD);
    motor3.setSpeed(200);
	  motor4.run(FORWARD);  
    motor4.setSpeed(200); 
	  delay(500);
	  motor1.run(FORWARD);     
	  motor2.run(FORWARD);
	  motor3.run(FORWARD);
	  motor4.run(FORWARD);
	}  

	int readPing() { 
	  delay(20);
	  int cm = sonar.ping_cm();
	  if(cm==0)
	  {
		cm = 250;
	  }
	  return cm;
	}

    static void chase_right(uint32_t c) {
    for(uint16_t i=0; i<strip.numPixels()+4; i++) {
    strip.setPixelColor(i , c); 
    strip.setPixelColor(i-4, 0); 
    strip.show();
    delay(50);
      }
    }

    static void chase_left(uint32_t c) {
    for(uint16_t i=strip.numPixels()+4; i>=1; i--) {
    strip.setPixelColor(i-1-4 , c); 
    strip.setPixelColor(i-1, 0); 
    strip.show();
    delay(50);
      }
    }

    static void obstacleAvoid(){
      ////////////FUNCTIONALITATE OCOLIRE OBSTACOLE//////////////////////////////////////////////////
        
       //resetare distante la fiecare iteratie  
       int distanceR = 0;
       int distanceL =  0;
             
       //daca distanta este mai mica de 30cm pana la obstacol
       if(distance<=20)
       {
        moveStop();
        delay(100);
        distanceR = lookRight();
        delay(200);
        distanceL = lookLeft();
        delay(200);
      
        if(distanceR>=distanceL)
        {
        turnRight();
        delay(150);
        moveStop();
        delay(100);
    motor1.setSpeed(200);
    motor1.run(FORWARD);
    motor2.setSpeed(200);      
    motor2.run(FORWARD);
    motor3.setSpeed(200);
    motor3.run(FORWARD); 
    motor4.setSpeed(200);
    motor4.run(FORWARD);     
        delay(700);
        moveStop();
        delay(100);
        
        turnLeft();
        delay(80);
        moveStop();
        delay(100);

    motor1.run(FORWARD);
    motor1.setSpeed(200);      
    motor2.run(FORWARD);
    motor2.setSpeed(200);
    motor3.run(FORWARD); 
    motor3.setSpeed(200);
    motor4.run(FORWARD);     
    motor4.setSpeed(200);
        delay(700);
        moveStop();
        delay(100);

        
        turnLeft();
        delay(80);
        moveStop();
        delay(100);

    motor1.run(FORWARD);
    motor1.setSpeed(200);      
    motor2.run(FORWARD);
    motor2.setSpeed(200);
    motor3.run(FORWARD); 
    motor3.setSpeed(200);
    motor4.run(FORWARD);     
    motor4.setSpeed(200);
    
        delay(350);
        moveStop();
        delay(100);

        turnRight();
        delay(90);
        moveStop();
        delay(100);
        }else
        {
         turnLeft();
        delay(150);
        moveStop();
        delay(100);
    motor1.setSpeed(200);
    motor1.run(FORWARD);
    motor2.setSpeed(200);      
    motor2.run(FORWARD);
    motor3.setSpeed(200);
    motor3.run(FORWARD); 
    motor4.setSpeed(200);
    motor4.run(FORWARD);     
        delay(500);
        moveStop();
        delay(100);
        
        turnRight();
        delay(80);
        moveStop();
        delay(100);

    motor1.run(FORWARD);
    motor1.setSpeed(200);      
    motor2.run(FORWARD);
    motor2.setSpeed(200);
    motor3.run(FORWARD); 
    motor3.setSpeed(200);
    motor4.run(FORWARD);     
    motor4.setSpeed(200);
        delay(700);
        moveStop();
        delay(100);

        
        turnRight();
        delay(80);
        moveStop();
        delay(100);

    motor1.run(FORWARD);
    motor1.setSpeed(200);      
    motor2.run(FORWARD);
    motor2.setSpeed(200);
    motor3.run(FORWARD); 
    motor3.setSpeed(200);
    motor4.run(FORWARD);     
    motor4.setSpeed(200);
    
        delay(1150);
        moveStop();
        delay(100);

        turnLeft();
        delay(90);
        moveStop();
        delay(100);
        }
       }
       distance = readPing();
    }
