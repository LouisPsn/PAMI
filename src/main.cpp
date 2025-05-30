#include <Arduino.h>
#include <Dynamixel_Serial.h>       // Library needed to control Dynamixal servo
#include <Servo.h>

Servo servo;  // create servo object to control a servo

/*---------------*/
/* Right HC-SR04 */
/*---------------*/
#define TRIG_RIGHT 10
#define ECHO_RIGHT 11

/*--------------*/
/* Left HC-SR04 */
/*--------------*/
#define TRIG_LEFT A6
#define ECHO_LEFT A7

/*----------------*/
/* Middle HC-SR04 */
/*----------------*/
#define TRIG_MIDDLE 12
#define ECHO_MIDDLE 13

/*---------*/
/* Tirette */
/*---------*/
#define TIRETTE_PIN 17

/*--------------*/
/* Interrupteur */
/*--------------*/
#define INTERRUPT_PIN 16

/*------*/
/* LEDS */
/*------*/
int redPin = 5;
int greenPin = 6;
int bluePin = 8;

void move_servo()
{
  servo.write(50);
  delay(15);
  servo.write(75);
  delay(15);
}

void setColor(int redValue, int greenValue,  int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin,  greenValue);
  analogWrite(bluePin, blueValue);
}

void light_LEDs() {
  setColor(255, 0, 0); // Red Color
  delay(1000);
  setColor(0,  255, 0); // Green Color
  delay(1000);
  setColor(0, 0, 255); // Blue Color
  delay(1000);
  setColor(255, 255, 255); // White Color
  delay(1000);
  setColor(170, 0, 255); // Purple Color
  delay(1000);
  setColor(127, 127,  127); // Light Blue
  delay(1000);
}

/*-------*/
/* MX-28 */
/*-------*/
#define SERVO_ID_1 0x01               // ID of which we will set Dynamixel too
#define SERVO_ID_2 0x02               // ID of which we will set Dynamixel too
#define SERVO_ControlPin 0x7        // Control pin of buffer chip, NOTE: this does not matter becasue we are not using a half to full contorl buffer.
#define SERVO_SET_Baudrate 57600    // Baud rate speed which the Dynamixel will be set too (57600)

int read_tirette()
{
  return digitalRead(TIRETTE_PIN);
}

float read_left_HC_SR04()
{
  float duration_left, distance_left;

  digitalWrite(TRIG_LEFT, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_LEFT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_LEFT, LOW);

  duration_left = pulseIn(ECHO_LEFT, HIGH);
  distance_left = (duration_left*.0343)*10/2;

  return distance_left;
}

float read_middle_HC_SR04()
{
  float duration_middle, distance_middle;

  digitalWrite(TRIG_MIDDLE, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_MIDDLE, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_MIDDLE, LOW);

  duration_middle = pulseIn(ECHO_MIDDLE, HIGH);
  distance_middle = (duration_middle*.0343)*10/2;

  return distance_middle;
}

float read_right_HC_SR04()
{
  float duration_right, distance_right;

  digitalWrite(TRIG_RIGHT, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_RIGHT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_RIGHT, LOW);

  duration_right = pulseIn(ECHO_RIGHT, HIGH);
  distance_right = (duration_right*.0343)*10/2;

  return distance_right;
}

void go_straight_MX_28()
{
  Dynamixel.wheel(SERVO_ID_1,RIGHT,0x3FF);              // Comman for Wheel mode, Move left at max speed
  Dynamixel.wheel(SERVO_ID_2,LEFT,0x3FF);              // Comman for Wheel mode, Move left at max speed
}

void stop_MX_28()
{
  Dynamixel.wheel(SERVO_ID_1,RIGHT,0x0);              // Comman for Wheel mode, Move left at max speed
  Dynamixel.wheel(SERVO_ID_2,LEFT,0x0);              // Comman for Wheel mode, Move left at max speed
}

void turn_left_MX_28()
{
  Dynamixel.wheel(SERVO_ID_1,RIGHT,0x3FF);              // Comman for Wheel mode, Move left at max speed
  Dynamixel.wheel(SERVO_ID_2,RIGHT,0x3FF);              // Comman for Wheel mode, Move left at max speed
  delay(580);
  stop_MX_28();
}

void turn_right_MX_28()
{
  Dynamixel.wheel(SERVO_ID_1,LEFT,0x3FF);              // Comman for Wheel mode, Move left at max speed
  Dynamixel.wheel(SERVO_ID_2,LEFT,0x3FF);              // Comman for Wheel mode, Move left at max speed
  delay(580);
  stop_MX_28();
}

void left_to_right_demi_circle_MX_28()
{

}

void clockwise_circle_MX()
{
  Dynamixel.wheel(SERVO_ID_1,LEFT,0x3FF);              // Comman for Wheel mode, Move left at max speed
  delay(2000);
  stop_MX_28();
}

int detect_obstacle()
{
  if (read_middle_HC_SR04() < 75)
  {
    return 2;
  }
  return 0;
}

void setup() {
  /*--------------*/
  /* Serial Debug */
  /*--------------*/
  // Serial.begin(9600);

  /*-------------*/
  /* Setup MX-28 */
  /*-------------*/
  Dynamixel.begin(SERVO_SET_Baudrate);                                    // We now need to set Ardiuno to the new Baudrate speed
  Dynamixel.setDirectionPin(SERVO_ControlPin);                            // Optional. Set direction control pin
  Dynamixel.setMode(SERVO_ID_1, WHEEL, 0, 0);                             // set mode to WHEEL
  Dynamixel.setMode(SERVO_ID_2, WHEEL, 0, 0);                             // set mode to WHEEL

  /*---------------*/
  /* Setup Tirette */
  /*---------------*/
  pinMode(TIRETTE_PIN, INPUT_PULLUP);

  /*-----------------*/
  /* Setup Interrupt */
  /*-----------------*/
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);

  /*--------------------*/
  /* Setup LEFT HC-SR04 */
  /*--------------------*/
  pinMode(TRIG_LEFT, OUTPUT);
	pinMode(ECHO_LEFT, INPUT);

  /*----------------------*/
  /* Setup MIDDLE HC-SR04 */
  /*----------------------*/
  pinMode(TRIG_MIDDLE, OUTPUT);
	pinMode(ECHO_MIDDLE, INPUT);

  /*---------------------*/
  /* Setup RIGHT HC-SR04 */
  /*---------------------*/
  pinMode(TRIG_RIGHT, OUTPUT);
	pinMode(ECHO_RIGHT, INPUT);

  /*------------*/
  /* Setup LEDs */
  /*------------*/
  pinMode(redPin,  OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  setup();

  // go_straight_MX_28();

  // while(1)
  // {
  //   go_straight_MX_28();
  //   delay(10000);
  //   turn_left_MX_28();
  //   go_straight_MX_28();
  //   delay(2000);
  //   stop_MX_28();
  //   delay(60000);
  // }

  // delay(10000);

  // unsigned long duration_1 = 0;
  // unsigned long duration_2 = 0;

  while (digitalRead(TIRETTE_PIN) == 1) {}

  // delay(85000);

  unsigned long start_time_1 = millis();
  
  while (millis() - start_time_1 < 9700)
  {
    if (detect_obstacle() != 0)
    {
      stop_MX_28();
    }
    else {
      go_straight_MX_28();
    }
    while (millis() - start_time_1 > 15000)
    {
      servo.attach(10);
      while (1)
      {
        stop_MX_28();
        move_servo();
        light_LEDs();
      }
    }
  }

  stop_MX_28();
  // If Switch on 0 Left, if on 1 Right
  if (digitalRead(INTERRUPT_PIN))
  {
    turn_left_MX_28();
  }
  else
  {
    turn_right_MX_28();
  }
  stop_MX_28();

  unsigned long start_time_2 = millis();

  while (millis() - start_time_2 < 2300)
  {
    if (detect_obstacle() != 0)
    {
      stop_MX_28();
    }
    else {
      go_straight_MX_28();
    }
    while (millis() - start_time_1 > 15000)
    {
      servo.attach(10);
      while (1)
      {
        stop_MX_28();
        move_servo();
        light_LEDs();
      }
    }
  }

  stop_MX_28();

  /*-------------*/
  /* Setup Servo */
  /*-------------*/
  servo.attach(10);

  while(1) {
    stop_MX_28();
    move_servo();
    light_LEDs();

    // if (detect_obstacle() != 0)
    // {
    //   stop_MX_28();
    // }
    // else
    // {
    //   go_straight_MX_28();
    // }

    // Serial.print("Distance : ");
    // Serial.println(read_middle_HC_SR04());
    // delay(100);
  }
}