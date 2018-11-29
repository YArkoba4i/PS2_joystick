

#include <SoftwareSerial.h>
#include <PS2X_lib\PS2X_lib.h>  //for v1.6



/******************************************************************
* set pins connected to PS2 controller:
*   - 1e column: original
*   - 2e colmun: Stef?
* replace pin numbers by the ones you use
******************************************************************/
//esp

// FFFFFF795AFFFF7E768080000000000000 //

//Arduino
	#define PS2_DAT   2   //brown	miso
	#define PS2_CMD   4   //orange	mosi
	#define PS2_SEL   3   //yellow	cs
	#define PS2_CLK   5  //blue		clk

/*
// esp12e hardware SPI config
#define PS2_DAT   13   //brown	D7  miso 
#define PS2_CMD   12   //orange	D6	mosi
#define PS2_SEL   15   //yellow	D8	cs
#define PS2_CLK   14   //blue	D5 clk

/*
// esp12e software SPI config
#define PS2_DAT   7   //brown	D7  miso 
#define PS2_CMD   8   //orange	D8	mosi
#define PS2_SEL   11   //yellow	D9	cs
#define PS2_CLK   6   //blue	D6	clk

//clock, command, attention, data,

/******************************************************************/
// L298N Driver 
// Motor 1 - UP <--> Down 
#define enA 9	// PWM signal
// Directions
#define in1 8
#define in2 7

// Motor 2 - Left <--> Right 
#define enB 11	// PWM signal
// Directions
#define in3 13
#define in4 12

/******************************************************************/
//	ESP8266-01   
#define TXD 11
#define RXD 10


// software serial #2: RX = digital pin 8, TX = digital pin 9
// on the Mega, use other pins instead, since 8 and 9 don't work on the Mega

/******************************************************************/

/******************************************************************
* select modes of PS2 controller:
*   - pressures = analog reading of push-butttons
*   - rumble    = motor rumbling
* uncomment 1 of the lines for each mode selection
******************************************************************/
#define pressures   false
//#define pressures   false
//#define rumble      true
#define rumble      false

PS2X ps2x; // create PS2 Controller Class

		   //right now, the library does NOT support hot pluggable controllers, meaning 
		   //you must always either restart your Arduino after you connect the controller, 
		   //or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;


void setup() {
	/******************************************************************/
/*	// L298N Driver 
	pinMode(enA, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);

	// Set initial rotation direction
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	/******************************************************************/

	Serial.begin(115200);

	//delay(5000);  //added delay to give wireless ps2 module some time to startup, before configuring it
	while (!Serial) {
		; // wait for serial port to connect. Needed for native USB port only
	}
	/******************************************************************/
	// I2C comunication setup
	//myWire.pins(9, 8);
	//myWire.beginTransmission(9);                // join i2c bus with address #9


	Serial.begin(115200);           // start serial for output
	Serial.println("/// Slave - Start ///");

	/******************************************************************/
	error = configGamePad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);


	type = readType();
	
}

void loop() {
	/* You must Read Gamepad to get new values and set vibration values
	ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
	if you don't enable the rumble, use ps2x.read_gamepad(); with no values
	You should call this at least once a second
	*/
	//if (error == 1) //skip loop if no controller found
		//return;
	//error = configGamePad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
	//type = readType();

	//sendESP_CMD(Serial.readString());
/*
	myWire.beginTransmission(9); // transmit to device #9
	myWire.write("x is ");       // sends five bytes
	myWire.write(error);             // sends one byte
	myWire.endTransmission();    // stop transmitting

	Serial.println(error);

	error++;
	if (error >= 50)
		error = 0;
	*/

	/*
	if (type == 2) { //Guitar Hero Controller
		ps2x.read_gamepad();          //read controller 

		if (ps2x.ButtonPressed(GREEN_FRET))
			Serial.println("Green Fret Pressed");
		if (ps2x.ButtonPressed(RED_FRET))
			Serial.println("Red Fret Pressed");
		if (ps2x.ButtonPressed(YELLOW_FRET))
			Serial.println("Yellow Fret Pressed");
		if (ps2x.ButtonPressed(BLUE_FRET))
			Serial.println("Blue Fret Pressed");
		if (ps2x.ButtonPressed(ORANGE_FRET))
			Serial.println("Orange Fret Pressed");

		if (ps2x.ButtonPressed(STAR_POWER))
			Serial.println("Star Power Command");

		if (ps2x.Button(UP_STRUM))          //will be TRUE as long as button is pressed
			Serial.println("Up Strum");
		if (ps2x.Button(DOWN_STRUM))
			Serial.println("DOWN Strum");

		if (ps2x.Button(PSB_START))         //will be TRUE as long as button is pressed
			Serial.println("Start is being held");
		if (ps2x.Button(PSB_SELECT))
			Serial.println("Select is being held");

		if (ps2x.Button(ORANGE_FRET)) {     // print stick value IF TRUE
			Serial.print("Wammy Bar Position:");
			Serial.println(ps2x.Analog(WHAMMY_BAR), DEC);
		}
	}
	else { //DualShock Controller
		//Serial.println("DualShock Controller");
		ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed

		if (ps2x.Button(PSB_START))         //will be TRUE as long as button is pressed
			Serial.println("Start is being held");
		if (ps2x.Button(PSB_SELECT))
			Serial.println("Select is being held");

		if (ps2x.Button(PSB_PAD_UP)) {      //will be TRUE as long as button is pressed
			Serial.print("Up held this hard: ");
			Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
			digitalWrite(3, HIGH);
			delay(50);
			digitalWrite(3, LOW);

		}
		if (ps2x.Button(PSB_PAD_RIGHT)) {
			Serial.print("Right held this hard: ");
			Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
			digitalWrite(9, HIGH);
			delay(50);
			digitalWrite(9, LOW);
		}
		if (ps2x.Button(PSB_PAD_LEFT)) {
			Serial.print("LEFT held this hard: ");
			Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
			digitalWrite(6, HIGH);
			delay(50);
			digitalWrite(6, LOW);
		}
		if (ps2x.Button(PSB_PAD_DOWN)) {
			Serial.print("DOWN held this hard: ");
			Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
			digitalWrite(5, HIGH);
			delay(50);
			digitalWrite(5, LOW);
		}

		vibrate = ps2x.Analog(PSAB_CROSS);  //this will set the large motor vibrate speed based on how hard you press the blue (X) button
		if (ps2x.NewButtonState()) {        //will be TRUE if any button changes state (on to off, or off to on)
			if (ps2x.Button(PSB_L3))
				Serial.println("L3 pressed");
			if (ps2x.Button(PSB_R3))
				Serial.println("R3 pressed");
			if (ps2x.Button(PSB_L2))
				Serial.println("L2 pressed");
			if (ps2x.Button(PSB_R2))
				Serial.println("R2 pressed");
			if (ps2x.Button(PSB_TRIANGLE))
				Serial.println("Triangle pressed");
		}

		if (ps2x.ButtonPressed(PSB_CIRCLE))               //will be TRUE if button was JUST pressed
			Serial.println("Circle just pressed");
		if (ps2x.NewButtonState(PSB_CROSS))               //will be TRUE if button was JUST pressed OR released
			Serial.println("X just changed");
		if (ps2x.ButtonReleased(PSB_SQUARE))              //will be TRUE if button was JUST released
			Serial.println("Square just released");
		
		if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { //print stick values if either is TRUE
			Serial.print("Stick Values:");
			Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
			Serial.print(",");
			Serial.print(ps2x.Analog(PSS_LX), DEC);
			Serial.print(",");
			Serial.print(ps2x.Analog(PSS_RY), DEC);
			Serial.print(",");
			Serial.println(ps2x.Analog(PSS_RX), DEC);

			

		}

		//motorDriver(ps2x.Analog(PSS_LY), ps2x.Analog(PSS_LX), ps2x.Analog(PSS_RY), ps2x.Analog(PSS_RX));

	}*/

	ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed

	for (size_t i = 0; i < 9; i++)
	{
		Serial.print(ps2x.PS2data[i],HEX);
	}
	Serial.println();

	
	delay(50);
}


int configGamePad(uint8_t clk, uint8_t cmd, uint8_t att, uint8_t dat, bool pressure, bool rumbl) {
		
	int error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressure, rumbl);
	   
	if (error == 0) {
		Serial.print("Found Controller, configured successful ");
		Serial.print("pressures = ");
		if (pressures)
			Serial.println("true ");
		else
			Serial.println("false");
		Serial.print("rumble = ");
		if (rumble)
			Serial.println("true)");
		else
			Serial.println("false");
		Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
		Serial.println("holding L1 or R1 will print out the analog stick values.");
		Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
	}
	else if (error == 1)
		Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

	else if (error == 2)
		Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

	else if (error == 3)
		Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
	
	return error;
}

int readType() {
	int type = ps2x.readType();
	switch (type) {
	case 0:
		Serial.print("Unknown Controller type found \n");
		break;
	case 1:
		Serial.print("DualShock Controller found ");
		break;
	case 2:
		Serial.print("GuitarHero Controller found ");
		break;
	case 3:
		Serial.print("Wireless Sony DualShock Controller found ");
		break;
	}

	return type;
}

void motorDriver(byte LY, byte LX, byte RY, byte RX) {
	//LY - (0-Up, 255-Down)		RY - (0-Up, 255-Down)
	//LX - (0-Left, 255-Right)	RX - (0-Left, 255-Right)


	if (LY < 126) { //Left UP
		int L_up = map(LY, 126, 0, 0, 255);
		Serial.println("PWM_L_Up = " + String(L_up));

		// Send PWM signal to L298N Enable pin
		analogWrite(enA, L_up);
		digitalWrite(in1, HIGH);
		digitalWrite(in2, LOW);
	}
	else {
		if (LY > 130) {	// Left Down
			int L_down = map(LY, 130, 255, 0, 255);
			Serial.println("PWM_L_Down = " + String(L_down));

			// Send PWM signal to L298N Enable pin
			analogWrite(enA, L_down);
			digitalWrite(in1, LOW);
			digitalWrite(in2, HIGH);
		}
		else {
			analogWrite(enA, 0);
		}
	}
	
	/*
	if (RY < 116) { // Right UP
		int R_up = map(RY, 116, 0, 0, 255);
		Serial.println("PWM_R_Up = " + String(R_up));

		analogWrite(enA, R_up);
		digitalWrite(in1, HIGH);
		digitalWrite(in2, LOW);
	}
	else {
		if (RY > 130) {	// Right Down
			int R_down = map(RY, 130, 255, 0, 255);
			Serial.println("PWM_R_Down = " + String(R_down));

			// Send PWM signal to L298N Enable pin
			analogWrite(enA, R_down);
			digitalWrite(in1, LOW);
			digitalWrite(in2, HIGH);
		}
		else {
			analogWrite(enA, 0);
		}
	}*/
	// Left <--> Rigth

	if (LX < 126) { //Left Left
		int L_left = map(LX, 126, 0, 0, 255);
		Serial.println("PWM_L_Left = " + String(L_left));

		// Send PWM signal to L298N Enable pin
		analogWrite(enB, L_left);
		digitalWrite(in3, LOW);
		digitalWrite(in4, HIGH);

	}else
	if (LX > 130) {	// Left Right
		int L_right = map(LX, 130, 255, 0, 255);
		Serial.println("PWM_L_Right = " + String(L_right));

		// Send PWM signal to L298N Enable pin
		analogWrite(enB, L_right);
		digitalWrite(in3, HIGH);
		digitalWrite(in4, LOW);
	}
	else
	{
		analogWrite(enB, 0);
	}

	/*
	if (RX < 120) { // Right UP
		int R_left = map(RX, 120, 0, 0, 255);
		Serial.println("PWM_R_Left = " + String(R_left));

		// Send PWM signal to L298N Enable pin
		analogWrite(enB, R_left);
		digitalWrite(in3, LOW);
		digitalWrite(in4, HIGH);
	}else
	if (RX > 130) {	// Right Down
		int R_right = map(RX, 130, 255, 0, 255);
		Serial.println("PWM_R_Right = " + String(R_right));

		// Send PWM signal to L298N Enable pin
		analogWrite(enB, R_right);
		digitalWrite(in3, HIGH);
		digitalWrite(in4, LOW);
	}else
		analogWrite(enB, 0);
		
	*/
	/*
	if ( ((LY && LX ) < 130) && ((LY && LX) > 120)) {
		analogWrite(enA, 0);
		Serial.println("enA = 0" );
	}else
	if (((RY && RX) < 130) && ((RY && RX) > 116)) {
		analogWrite(enB, 0);
		Serial.println("enB = 0");
	}else
		analogWrite(enB, 0);*/
}

