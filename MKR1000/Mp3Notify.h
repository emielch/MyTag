// Mp3Notify.h

#ifndef _MP3NOTIFY_h
#define _MP3NOTIFY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Mp3Notify
{
public:
	static void OnError(uint16_t errorCode)
	{
		// see DfMp3_Error for code meaning
		Serial.println();
		Serial.print("Com Error ");
		Serial.println(errorCode);
	}

	static void OnPlayFinished(uint16_t globalTrack)
	{
		Serial.println();
		Serial.print("Play finished for #");
		Serial.println(globalTrack);
	}

	static void OnCardOnline(uint16_t code)
	{
		Serial.println();
		Serial.print("Card online ");
		Serial.println(code);
	}

	static void OnCardInserted(uint16_t code)
	{
		Serial.println();
		Serial.print("Card inserted ");
		Serial.println(code);
	}

	static void OnCardRemoved(uint16_t code)
	{
		Serial.println();
		Serial.print("Card removed ");
		Serial.println(code);
	}
};

#endif

