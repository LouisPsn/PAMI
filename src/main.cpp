#include <Arduino.h>

/*--------------*/
/* Left TCS3200 */
/*--------------*/
#define OUT_LEFT 5
#define OE_LEFT 19
#define S0_LEFT 21
#define S1_LEFT 20
#define S2_LEFT 6
#define S3_LEFT 7

/*---------------*/
/* Right TCS3200 */
/*---------------*/
#define OUT_RIGHT 8
#define OE_RIGHT 3
#define S0_RIGHT 24
#define S1_RIGHT 23
#define S2_RIGHT 9
#define S3_RIGHT 11

/*--------------*/
/* Left HC-SR04 */
/*--------------*/
#define TRIG_LEFT 13
#define ECHO_LEFT 14

/*----------------*/
/* Middle HC-SR04 */
/*----------------*/
#define TRIG_MIDDLE 15
#define ECHO_MIDDLE 16

/*---------------*/
/* Right HC-SR04 */
/*---------------*/
#define TRIG_RIGHT 25
#define ECHO_RIGHT 26

/*-------*/
/* MX-28 */
/*-------*/
#define MX_28_PIN 10

/*---------*/
/* Tirette */
/*---------*/
#define START_PIN 12

/*-----------------*/
/* Sensor Position */
/*-----------------*/
#define LEFT 0
#define MIDDLE 1
#define RIGHT 2

int read_tirette();

int read_left_HC_SR04();
int read_middle_HC_SR04();
int read_right_HC_SR04();

int read_left_TCS3200();
int read_right_TCS3200();

int command_MX_28();
int get_MX_28_data();

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}
