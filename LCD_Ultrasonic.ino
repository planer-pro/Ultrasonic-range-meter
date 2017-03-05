#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x20 for a 16 chars and 2 line display

#define trigPin 10
#define echoPin 11
#define noCounts 4

unsigned int distance, distanceOut, distanceOutOld;
unsigned long startAfter100ms;
unsigned long startAfter400ms;

void setup()
{
	lcd.init();
	lcd.backlight();
	lcd.print("RangeFinder v.1.0");
	delay(2000);

	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
}

void loop()
{
	if (millis() >= startAfter100ms)
	{
		int distance = GetDistance();
		startAfter100ms = millis() + 100;
	}

	if (millis() >= startAfter400ms)
	{
		if (distanceOut >= 4 && distanceOut <= 400)
		{
			if (distanceOut != distanceOutOld)
			{
				lcd.clear();
				lcd.print("Distance is:");
				lcd.setCursor(8, 2);
				lcd.print(distanceOut);
				lcd.print(" cm");
				distanceOutOld = distanceOut;
				startAfter400ms = millis() + 400;
			}
		}
		else
		{
			lcd.clear();
			lcd.print("Distance is:");
			lcd.setCursor(2, 2);
			lcd.print("Out of Range!!!");
			distanceOutOld = distanceOut;
			startAfter400ms = millis() + 400;
		}
	}
}

int GetDistance()
{
	for (int i = 0; i < noCounts; i++)
	{
		digitalWrite(trigPin, HIGH);
		delayMicroseconds(1000);
		digitalWrite(trigPin, LOW);
		int duration = pulseIn(echoPin, HIGH);
		distance = duration/58;
		//distance = (duration/2) / 29.1;
		delayMicroseconds(1000);

		distanceOut += distance;
	}
	distanceOut /= noCounts;

	return distanceOut;
}