#define INPUT_SIZE 200
char serialBuf[INPUT_SIZE + 1];
int currSerialPos = 0;

void checkSerial() {
	while (Serial2.available()) {
		char inChar = (char)Serial2.read();
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
	//Serial.print("Received: ");
	//Serial.println(serialBuf);

	int messID = atoi(strtok(serialBuf, ","));


	if (messID == 1) parseTagInfo();
	//if (messID == 2) parseEpoch();
	//if (messID == 3) parseVolume();
}


void parseTagInfo() {
	int tagID = atoi(strtok(0, ","));
	tagID--;
	if (tagID < 0 || tagID >= numberOfSlots) return;

	slot[tagID].ID = tagID+1;

	slot[tagID].r = atoi(strtok(0, ","));
	slot[tagID].g = atoi(strtok(0, ","));
	slot[tagID].b = atoi(strtok(0, ","));

	slot[tagID].isTimer = atoi(strtok(0, ","));
	slot[tagID].savedTime = atol(strtok(0, ","));

	slot[tagID].audioSample = atoi(strtok(0, ","));
	slot[tagID].removed = atoi(strtok(0, ","));
	slot[tagID].alarmTime = atol(strtok(0, ","));
}