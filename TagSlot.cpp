// 
// 
// 

#include "TagSlot.h"

extern DFMiniMp3<HardwareSerial, Mp3Notify> mp3;


void TagSlot::init(int _id, int _switchPin, Color _col, Segment * _ledSegment)
{
	id = _id;
	switchPin = _switchPin;
	ledSegment = _ledSegment;
	color = _col;
	currSwitch = true;
	pinMode(switchPin, INPUT_PULLUP);
}

void TagSlot::update()
{
	bool _currSwitch = digitalRead(switchPin);
	if (_currSwitch != currSwitch) {
		currSwitch = _currSwitch;
		alarming = false;
		if (currSwitch) {
			Serial.println("Slot " + String(id) + " removed");
			ledSegment->setFade(color, 1. / countdownTime);
		}
		else {
			Serial.println("Slot " + String(id) + " returned");
			ledSegment->setFade(Color(0, 0, 0, RGB_MODE), 2);
		}
	}

	if (ledSegment->getEffectID() == STATIC) {
		ledSegment->setPulsate(color, 2);
		mp3.playMp3FolderTrack(1);
		alarmTime = millis();
		alarming = true;
	}

}

void TagSlot::setTimeFac(float fac)
{
	Color newCol;
	newCol.fade(color, Color(0, 0, 0, RGB_MODE),  fac);
	ledSegment->setStaticColor(newCol);
	ledSegment->setFade(color, 1. / (countdownTime*(fac)));
	alarming = false;
}
