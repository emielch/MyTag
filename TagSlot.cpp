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
		if (currSwitch) slotRemoved();
		else slotReturned();
	}


	if (countingDown) {
		if (rtc.getEpoch() > alarmTime) {
			Serial.println("Slot " + String(id) + " alarming!");
			countingDown = false;
			alarming = true;

			ledSegment->setPulsate(color, 2);
			mp3.playMp3FolderTrack(audioSample);
		}
		else {
			float fac = float(rtc.getEpoch() - startCountTime) / (alarmTime - startCountTime);
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
		int currTime = rtc.getHours() * 60 * 60 + rtc.getMinutes() * 60 + rtc.getSeconds();
		if (savedTime > currTime) alarmTime = startCountTime + savedTime - currTime;
		else alarmTime = startCountTime + 24 * 60 * 60 - currTime + savedTime;
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
	newCol.fade(color, Color(0, 0, 0, RGB_MODE),  fac);
	//ledSegment->setStaticColor(newCol);
	//ledSegment->setFade(color, 1. / (countdownTime*(fac)));
	//alarming = false;
}
