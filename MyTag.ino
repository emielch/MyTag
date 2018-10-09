#include "Mp3Notify.h"
#include "TagSlot.h"
#include <Colore.h>
#include <FastLED.h>
#include <DFMiniMp3.h>

#define LED_PIN 12
#define LED_AM 36
#define BEAM_AM 10

Beam beams[BEAM_AM];

CRGB leds[LED_AM];
//Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_AM, PIN, NEO_GRB + NEO_KHZ800);

Segment seg[] = {
	Segment(0,35),  // 12
	Segment(0,35),  // 12
	Segment(0,2),  // 0
	Segment(3,5),  // 1
	Segment(6,8),  // 2
	Segment(9,11),  // 3
	Segment(12,14),  // 4
	Segment(15,17),  // 5
	Segment(18,20),  // 6
	Segment(21,23),  // 7
	Segment(24,26),  // 8
	Segment(27,29),  // 9
	Segment(30,32),  // 10
	Segment(33,35)  // 11
};
int inputPins[] = { 10,8,9,2,7,1,3,4,0,5,6,11 };

byte segAm = sizeof(seg) / sizeof(Segment);
Colore colore(LED_AM, seg, segAm, beams, BEAM_AM, &set_ledLib, &get_ledLib, &show_ledLib, &reset_ledLib);

#define numberOfSlots 12
TagSlot slot[numberOfSlots];

DFMiniMp3<HardwareSerial, Mp3Notify> mp3(Serial1);

void setup() {
	Serial.begin(9600);

	LEDS.addLeds<WS2812, LED_PIN, GRB>(leds, LED_AM);
	LEDS.setBrightness(100);

	for (int i = 0; i < numberOfSlots; i++) {
		slot[i].init(i + 1, inputPins[i], Color(i*360. / 12, 100, 100, HSB_MODE), &seg[i+2]);
	}

	slot[0].setColor(Color(255, 0, 255, RGB_MODE));
	slot[1].setColor(Color(255, 0, 0, RGB_MODE));
	slot[2].setColor(Color(255, 0, 0, RGB_MODE));
	slot[3].setColor(Color(255, 255, 0, RGB_MODE));
	slot[4].setColor(Color(255, 255, 0, RGB_MODE));
	slot[5].setColor(Color(0, 255, 0, RGB_MODE));
	slot[6].setColor(Color(0, 255, 0, RGB_MODE));
	slot[7].setColor(Color(0, 255, 255, RGB_MODE));
	slot[8].setColor(Color(0, 255, 255, RGB_MODE));
	slot[9].setColor(Color(0, 0, 255, RGB_MODE));
	slot[10].setColor(Color(0, 0, 255, RGB_MODE));
	slot[11].setColor(Color(255, 0, 255, RGB_MODE));

	mp3.begin();
	mp3.setVolume(100);
	mp3.playMp3FolderTrack(1);
	mp3.loop();
	seg[1].setBlendMode(MULTIPLY);
	seg[0].setRainbow(0.3,2,100);
	seg[1].setFadeInOut(Color(255, 255, 255, RGB_MODE), Color(0, 0, 0, RGB_MODE), 0.8, 0.3);
}

void loop() {
	checkSnooze();
	for (int i = 0; i < numberOfSlots; i++) {
		slot[i].update();
	}
	colore.update();
	//printFramerate();
}

bool touching = false;
TagSlot * snoozeSlot;
unsigned long lastPressTime = 0;
void checkSnooze() {
	int potVal = analogRead(A6);
	if (potVal > 50) {
		if (!touching) {
			touching = true;
			snoozeSlot = getLastAlarmSlot();
		}
		lastPressTime = millis();
		float potFac = potVal / 1024.;
		if (snoozeSlot != NULL)
			snoozeSlot->setTimeFac(potFac);

	}
	else if (potVal < 20 && touching && millis() > lastPressTime + 200) {
		touching = false;
	}
}

TagSlot * getLastAlarmSlot() {
	TagSlot * maxSlot = NULL;
	for (int i = 0; i < numberOfSlots; i++) {
		if (slot[i].isAlarming()) {

			if (maxSlot==NULL || slot[i].getAlarmTime() > maxSlot->getAlarmTime()) {
				maxSlot = &slot[i];
			}
		}
	}
	return maxSlot;
}

void printFramerate() {
	Serial.print("FrameRate: ");
	Serial.println(colore.getFPS()); // print framerate
}


/* ---- Library Interface Functions ---- */

void set_ledLib(int pixel, byte r, byte g, byte b) {
	//leds.setPixelColor(pixel, r, g, b);
	leds[pixel] = CRGB(r, g, b);
}

void show_ledLib() {
	//leds.show();
	FastLED.show();
}

void reset_ledLib() {
	for (int i = 0; i < LED_AM; i++) {
		leds[i] = 0;
	}
}

Color get_ledLib(int i) {
	Color pixelCol(leds[i].r, leds[i].g, leds[i].b, RGB_MODE);
	return pixelCol;
}