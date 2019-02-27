

#include <TinyWireM.h>                  // I2C Master lib for ATTinys which use USI

#define DS1621_ADDR   0x48              // 7 bit I2C address for DS1621 temperature sensor
#define LED1_PIN         4              // ATtiny Pin 3
#define LED2_PIN         1              // ATtiny Pin 6
#define LED3_PIN         3              // ATtiny Pin 2

int tempC = 0;                          // holds temp in C
int tempF = 0;                          // holds temp in F


void setup() {
	pinMode(LED1_PIN, OUTPUT);
	pinMode(LED2_PIN, OUTPUT);
	pinMode(LED3_PIN, OUTPUT);
	Blink(LED1_PIN, 2);                    // show it's alive
	TinyWireM.begin();                    // initialize I2C lib
	Init_Temp();                          // Setup DS1621
	delay(3000);
}


void loop() {
	Get_Temp();
	Blink(LED1_PIN, tempC / 10);             // blink 10's of temperature on LED 1
	delay(1000);
	Blink(LED2_PIN, tempC % 10);             // blink 1's of temperature on LED 2
	delay(4000);                         // wait a few sec before next reading
}


void Init_Temp() { // Setup the DS1621 for one-shot mode
	TinyWireM.beginTransmission(DS1621_ADDR);
	TinyWireM.send(0xAC);                 // Access Command Register
	TinyWireM.send(B00000001);            // Using one-shot mode for battery savings
	//TinyWireM.send(B00000000);          // if setting continious mode for fast reads
	TinyWireM.endTransmission();          // Send to the slave
}


void Get_Temp() {  // Get the temperature from a DS1621
	TinyWireM.beginTransmission(DS1621_ADDR);
	TinyWireM.send(0xEE);                 // if one-shot, start conversions now
	TinyWireM.endTransmission();          // Send 1 byte to the slave
	delay(750);                           // if one-shot, must wait ~750 ms for conversion
	TinyWireM.beginTransmission(DS1621_ADDR);
	TinyWireM.send(0xAA);                 // read temperature (for either mode)
	TinyWireM.endTransmission();          // Send 1 byte to the slave
	TinyWireM.requestFrom(DS1621_ADDR, 1); // Request 1 byte from slave
	tempC = TinyWireM.receive();          // get the temperature
	tempF = tempC * 9 / 5 + 32;           // convert to Fahrenheit 
}


void Blink(byte led, byte times) { // poor man's GUI
	for (byte i = 0; i < times; i++) {
		digitalWrite(led, HIGH);
		delay(400);
		digitalWrite(led, LOW);
		delay(175);
	}
}
