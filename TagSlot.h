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


class TagSlot
{
protected:
	Segment * ledSegment;
	int switchPin;
	bool currSwitch;
	int id;
	Color color;
	unsigned long alarmTime = 0;
	bool alarming = false;
	int countdownTime = 15;

public:
	void init(int _id, int _switchPin, Color _col, Segment * _ledSegment);
	void update();
	unsigned long getAlarmTime() { return alarmTime; }
	void setTimeFac(float fac);
	bool isAlarming() { return alarming; }
	void setColor(Color _col) { color = _col; }
};


#endif

