#include <Arduino.h>
#include <Dynamixel2Arduino.h>
#include <SoftwareSerial.h>

/*--------------*/
/* Left TCS3200 */
/*--------------*/
#define OUT_LEFT 2
#define OE_LEFT 14
#define S0_LEFT 16
#define S1_LEFT 15
#define S2_LEFT 3
#define S3_LEFT 4

/*---------------*/
/* Right TCS3200 */
/*---------------*/
#define OUT_RIGHT 5
#define OE_RIGHT 17
#define S0_RIGHT 19
#define S1_RIGHT 18
#define S2_RIGHT 6
#define S3_RIGHT 7

/*---------------*/
/* Right HC-SR04 */
/*---------------*/
#define TRIG_MIDDLE 10
#define ECHO_MIDDLE 11

/*--------------*/
/* Left HC-SR04 */
/*--------------*/
#define TRIG_LEFT 12
#define ECHO_LEFT 13

/*----------------*/
/* Middle HC-SR04 */
/*----------------*/
#define TRIG_RIGHT A6
#define ECHO_RIGHT A7

/*---------*/
/* Tirette */
/*---------*/
#define TIRETTE_PIN 9

/*-----------------*/
/* Sensor Position */
/*-----------------*/
#define LEFT 0
#define MIDDLE 1
#define RIGHT 2

/*-----------------*/
/* Dynamixel MX-28 */
/*-----------------*/
SoftwareSerial soft_serial(1, 0); // DYNAMIXELShield UART RX/TX
#define DXL_SERIAL   Serial
#define DEBUG_SERIAL soft_serial
#define DXL_DIR_PIN 10  // DYNAMIXEL Shield DIR PIN

const uint8_t DXL_ID_1 = 1;
const uint8_t DXL_ID_2 = 2;
const float DXL_PROTOCOL_VERSION = 2.0;

Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);


int RGB_LEFT[3] = {0};
int RGB_RIGHT[3] = {0};

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

void read_left_TCS3200()
{
  // Set sensor to read Red only
	digitalWrite(S2_LEFT, LOW);
	digitalWrite(S3_LEFT, LOW);
	// Read the output Pulse Width
	RGB_LEFT[0] = pulseIn(OUT_LEFT, LOW);
	

  // Set sensor to read Green only
	digitalWrite(S2_LEFT, HIGH);
	digitalWrite(S3_LEFT, HIGH);
	// Read the output Pulse Width
	RGB_LEFT[1] = pulseIn(OUT_LEFT, LOW);
	
  // Set sensor to read Blue only
	digitalWrite(S2_LEFT,LOW);
	digitalWrite(S3_LEFT,HIGH);
	// Read the output Pulse Width
	RGB_LEFT[2] = pulseIn(OUT_LEFT, LOW);
}

void read_right_TCS3200()
{
  // Set sensor to read Red only
	digitalWrite(S2_RIGHT, LOW);
	digitalWrite(S3_RIGHT, LOW);
	// Read the output Pulse Width
	RGB_RIGHT[0] = pulseIn(OUT_RIGHT, LOW);
	

  // Set sensor to read Green only
	digitalWrite(S2_RIGHT, HIGH);
	digitalWrite(S3_RIGHT, HIGH);
	// Read the output Pulse Width
	RGB_RIGHT[1] = pulseIn(OUT_RIGHT, LOW);
	
  // Set sensor to read Blue only
	digitalWrite(S2_RIGHT,LOW);
	digitalWrite(S3_RIGHT,HIGH);
	// Read the output Pulse Width
	RGB_RIGHT[2] = pulseIn(OUT_RIGHT, LOW);
}

void command_MX_28(int rpm)
{
  // Set Goal Velocity using RPM
  dxl.setGoalVelocity(DXL_ID_1, rpm, UNIT_RPM);
  dxl.setGoalVelocity(DXL_ID_2, rpm, UNIT_RPM);
}

void print_MX_28_speed_rpm()
{
  DEBUG_SERIAL.print("Present Velocity(rpm) : ");
  DEBUG_SERIAL.println(dxl.getPresentVelocity(DXL_ID_1, UNIT_RPM));
  DEBUG_SERIAL.println(dxl.getPresentVelocity(DXL_ID_2, UNIT_RPM));
}

int detect_obstacle()
{
  if (read_left_HC_SR04() < 75)
  {
    return 1;
  }
  if (read_middle_HC_SR04() < 75)
  {
    return 2;
  }
  if (read_right_HC_SR04() < 75)
  {
    return 3;
  }
  return 0;
}

void setup() {
  /*--------------*/
  /* Serial Debug */
  /*--------------*/
  // Serial.begin(57600);

  /*---------------*/
  /* Setup Tirette */
  /*---------------*/
  DEBUG_SERIAL.begin(115200);
  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(57600);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  // Get DYNAMIXEL information
  dxl.ping(DXL_ID_1);
  dxl.ping(DXL_ID_2);

  // Turn off torque when configuring items in EEPROM area
  dxl.torqueOff(DXL_ID_1);
  dxl.setOperatingMode(DXL_ID_1, OP_VELOCITY);
  dxl.torqueOn(DXL_ID_1);

  dxl.torqueOff(DXL_ID_2);
  dxl.setOperatingMode(DXL_ID_2, OP_VELOCITY);
  dxl.torqueOn(DXL_ID_2);

  /*---------------*/
  /* Setup Tirette */
  /*---------------*/
  // pinMode(TIRETTE_PIN, INPUT_PULLUP);

  // /*--------------------*/
  // /* Setup LEFT HC-SR04 */
  // /*--------------------*/
  // pinMode(TRIG_LEFT, OUTPUT);  
	// pinMode(ECHO_LEFT, INPUT); 

  // /*----------------------*/
  // /* Setup MIDDLE HC-SR04 */
  // /*----------------------*/
  // pinMode(TRIG_MIDDLE, OUTPUT);  
	// pinMode(ECHO_MIDDLE, INPUT); 

  // /*---------------------*/
  // /* Setup RIGHT HC-SR04 */
  // /*---------------------*/
  // pinMode(TRIG_RIGHT, OUTPUT);  
	// pinMode(ECHO_RIGHT, INPUT);

  // /*--------------------*/
  // /* Setup LEFT TCS3200 */
  // /*--------------------*/
  // // Set S0 - S3 as outputs
	// pinMode(S0_LEFT, OUTPUT);
	// pinMode(S1_LEFT, OUTPUT);
	// pinMode(S2_LEFT, OUTPUT);
	// pinMode(S3_LEFT, OUTPUT);

	// // Set Pulse Width scaling to 20%
	// digitalWrite(S0_LEFT, HIGH);
	// digitalWrite(S1_LEFT, LOW);
	
  // // Set Sensor output as input
	// pinMode(OUT_LEFT, INPUT);

  // /*---------------------*/
  // /* Setup RiGHT TCS3200 */
  // /*---------------------*/
  // // Set S0 - S3 as outputs
	// pinMode(S0_RIGHT, OUTPUT);
	// pinMode(S1_RIGHT, OUTPUT);
	// pinMode(S2_RIGHT, OUTPUT);
	// pinMode(S3_RIGHT, OUTPUT);

	// // Set Pulse Width scaling to 20%
	// digitalWrite(S0_RIGHT, HIGH);
	// digitalWrite(S1_RIGHT, LOW);
	
  // // Set Sensor output as input
	// pinMode(OUT_RIGHT, INPUT);
}

void loop() {
  setup();

  while (1) {
    // Serial.print("Distance Right : ");
    // Serial.println(read_right_HC_SR04());
    // read_right_TCS3200();
    // Serial.print("Red : ");
    // Serial.print(RGB_RIGHT[0]);
    // Serial.print(", ");
    // Serial.print("Green : ");
    // Serial.print(RGB_RIGHT[1]);
    // Serial.print(", ");
    // Serial.print("Blue : ");
    // Serial.println(RGB_RIGHT[2]);
    command_MX_28(10);
    print_MX_28_speed_rpm();
    delay(100);
  }  
}