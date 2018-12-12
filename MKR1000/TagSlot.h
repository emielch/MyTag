// TagSlot.h

#ifndef _TAGSLOT_h
#define _TAGSLOT_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <Colore.h>
#include "Mp3Notify.h"
#include <DFMiniMp3.h>
#include <RTCZero.h>
extern RTCZero rtc;


class TagSlot
{
protected:
	Segment * ledSegment;
	int switchPin;
	bool currSwitch;
	int id;
	Color color;
	int audioSample = 1; // audio sample to be played when alarming, 0 for no audio
	bool isTimer = true;;  // true for timer, false for moment
	unsigned long savedTime = 300; // the time it will countdown from or alarm at
	

	bool countingDown = false;
	bool alarming = false;

	void slotRemoved();
	void slotReturned();

public:
	void init(int _id, int _switchPin, Color _col, Segment * _ledSegment);
	void update();

	unsigned long startCountTime;  // the time the countdown started
	unsigned long alarmTime;  // the time the alarm will go off at

	void setIsTimer(bool val) { isTimer = val; }
	void setSavedTime(unsigned long val) { savedTime = val; }
	void setColor(Color _col);
	void setAudio(int val) { audioSample = val; }


	unsigned long getAlarmTime() { return alarmTime; }
	void setTimeFac(float fac);
	bool isAlarming() { return alarming; }

	static int getHours(unsigned long val) {
		return val / 60 / 60 - (val/60/60/24*24);
	}
	static int getMinutes(unsigned long val) {
		return val / 60 - (val / 60 / 60 * 60);
	}
	static int getSeconds(unsigned long val) {
		return val - (val / 60 * 60);
	}

	String getDataString() {
		String str = "";
		str += id;
		str += ",";
		str += color.red();
		str += ",";
		str += color.green();;
		str += ",";
		str += color.blue();
		str += ",";
		str += isTimer;
		str += ",";
		str += savedTime;
		str += ",";
		str += audioSample;
		str += ",";
		str += currSwitch;
		str += ",";
		str += alarmTime;
		str += ",";
		str += audioSample;
		return str;
	}
	
};

#endif

