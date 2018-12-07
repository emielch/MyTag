// TagSlot.h

#ifndef _TAGSLOT_h
#define _TAGSLOT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


class TagSlot
{
public:
	byte ID = 0;
	byte r = 0, g = 0, b = 0;
	bool isTimer = 0;
	unsigned long savedTime = 0;
	int audioSample = 1;
	bool removed = 0;
	unsigned long alarmTime = 0;


	String getJSON() {
		String s = "{\"ID\":";
		s += ID;
		s += ",\"r\":";
		s += r;
		s += ",\"g\":";
		s += g;
		s += ",\"b\":";
		s += b;
		s += ",\"isTimer\":";
		s += isTimer;
		s += ",\"savedTime\":";
		s += savedTime;
		s += "}";

		return s;
	}
};


#endif

