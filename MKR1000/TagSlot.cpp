// 
// 
// 

#include "TagSlot.h"

extern DFMiniMp3<HardwareSerial, Mp3Notify> mp3;
extern unsigned int currTime;


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
		if (currSwitch) slotRemoved();
		else slotReturned();
	}


	if (countingDown) {
		if (currTime > alarmTime) {
			Serial.println("Slot " + String(id) + " alarming!");
			countingDown = false;
			alarming = true;

			ledSegment->setPulsate(color, 2);
			mp3.playMp3FolderTrack(audioSample);
		}
		else {
			float fac = 1 - float(currTime - startCountTime) / (alarmTime - startCountTime);
			Color newCol;
			newCol.fade(color, Color(0, 0, 0, RGB_MODE), fac);
			ledSegment->setStaticColor(newCol);
		}
	}

}

void TagSlot::slotRemoved() {
	Serial.println("Slot " + String(id) + " removed");

	countingDown = true;
	startCountTime = rtc.getEpoch();

	if (isTimer) {
		alarmTime = startCountTime + savedTime;
	}
	else {
		int currDayTime = rtc.getHours() * 60 * 60 + rtc.getMinutes() * 60 + rtc.getSeconds();
		if (savedTime > currDayTime) alarmTime = startCountTime + savedTime - currDayTime;
		else alarmTime = startCountTime + 24 * 60 * 60 - currDayTime + savedTime;
	}

}



void TagSlot::slotReturned() {
	Serial.println("Slot " + String(id) + " returned");

	ledSegment->setFade(Color(0, 0, 0, RGB_MODE), 2);
	countingDown = false;
	alarming = false;
}



void TagSlot::setTimeFac(float fac)
{
	Color newCol;
	newCol.fade(color, Color(0, 0, 0, RGB_MODE), fac);
	//ledSegment->setStaticColor(newCol);
	//ledSegment->setFade(color, 1. / (countdownTime*(fac)));
	//alarming = false;
}


void TagSlot::setColor(Color _col) {
	color = _col;

	if (ledSegment->getEffectID() == PULSATE) {
		ledSegment->setPulsate(color, 2);
	}
}