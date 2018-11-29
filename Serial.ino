#define INPUT_SIZE 200
char serialBuf[INPUT_SIZE + 1];
int currSerialPos = 0;

void checkSerial() {
	while (Serial.available()) {
		char inChar = (char)Serial.read();
		if (inChar == '\n' || inChar == '\r') {
			if (!currSerialPos == 0) {
				serialBuf[currSerialPos] = 0;  // add termination character
				parseSerial();
				currSerialPos = 0;
			}
		}
		else {
			serialBuf[currSerialPos] = inChar;
			currSerialPos++;
		}
	}
}



void parseSerial() {
	int messID = atoi(strtok(serialBuf, ","));
	Serial.println(messID);

	if (messID == 1) parseTagInfo();
	if (messID == 2) parseEpoch();
}


void parseTagInfo() {
	int tagID = atoi(strtok(0, ","));
	tagID--;
	if (tagID < 0 || tagID >= numberOfSlots) return;

	int r = atoi(strtok(0, ","));
	int g = atoi(strtok(0, ","));
	int b = atoi(strtok(0, ","));
	int isTimer = atoi(strtok(0, ","));

	unsigned long savedTime = atol(strtok(0, ","));
	slot[tagID].setSavedTime(savedTime);

	int audio = atoi(strtok(0, ","));
	

	slot[tagID].setColor(Color(r, g, b, RGB_MODE));
	slot[tagID].setIsTimer(isTimer);
	
	slot[tagID].setAudio(audio);

	
	//int removed = atoi(strtok(0, ","));
	//int hourRem = atoi(strtok(0, ","));
	//int minRem = atoi(strtok(0, ","));
	//int secRem = atoi(strtok(0, ","));

}

void parseEpoch() {
	long long ts = atoll(strtok(0, ","));
	long long timeDiff = ts - rtc.getEpoch();

	for (int i = 0; i < numberOfSlots; i++) {
		slot[i].startCountTime += timeDiff;
		slot[i].alarmTime += timeDiff;
	}

	rtc.setEpoch(ts);
}


void sendTagData() {
	for (int i = 0; i < numberOfSlots; i++) {
		Serial.print(1);
		Serial.print(",");
		Serial.print(slot[i].getDataString());
		Serial.println();
	}
}