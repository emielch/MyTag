#include "TagSlot.h"
#include <WiFi.h>
#include "esp_wpa2.h" //wpa2 library for connections to Enterprise networks
#include <WiFiClient.h>
#include <WebServer.h>
#include <FS.h>
#include "SPIFFS.h"
#include <DNSServer.h>
#include <ArduinoOTA.h>

const byte DNS_PORT = 53;
IPAddress apIP(172, 217, 28, 1);
DNSServer dnsServer;

#define NUM_REC 40

const char* APssid = "MyTag";
const char* APpassword = "mytag123";

const char *OTAName = "ESP32_MyTag";           // A name and a password for the OTA service
const char *OTAPassword = "esp8266";


WebServer server(80);

unsigned long updateTime = 0;

extern HardwareSerial Serial2;
int serialInterruptPin = 32;


#define numberOfSlots 12
TagSlot slot[numberOfSlots];

void setup(void) {
	Serial.begin(115200);
	Serial2.begin(115200, SERIAL_8N1, 22, 23);
	pinMode(serialInterruptPin, OUTPUT);
	digitalWrite(serialInterruptPin, LOW);


	startWiFi();                 // Start a Wi-Fi access point, and try to connect to some given access points. Then wait for either an AP or STA connection

	startOTA();

	startSPIFFS();               // Start the SPIFFS and list all contents

	startServer();               // Start a HTTP server with a file read handler and an upload handler

}


String inputString = "";


void loop(void) {
	dnsServer.processNextRequest();
	checkSerial();
	server.handleClient();                      // run the server
	ArduinoOTA.handle();
}





int counter = 0;
void startWiFi() { // Start a Wi-Fi access point, and try to connect to some given access points. Then wait for either an AP or STA connection
	WiFi.mode(WIFI_AP);

	WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
	WiFi.softAP(APssid, APpassword);             // Start the access point

	// modify TTL associated  with the domain name (in seconds)
	// default is 60 seconds
	//dnsServer.setTTL(300);
	// set which return code will be used for all other domains (e.g. sending
	// ServerFailure instead of NonExistentDomain will reduce number of queries
	// sent by clients)
	// default is DNSReplyCode::NonExistentDomain
	//dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);

	// start DNS server for a specific domain name
	dnsServer.start(DNS_PORT, "*", apIP);

	
	Serial.print("Access Point \"");
	Serial.print(APssid);
	Serial.println("\" started\r\n");


}



void startOTA() { // Start the OTA service
	ArduinoOTA.setHostname(OTAName);
	ArduinoOTA.setPassword(OTAPassword);

	ArduinoOTA.onStart([]() {
		Serial.println("Start");
	});
	ArduinoOTA.onEnd([]() {
		Serial.println("\r\nEnd");
	});
	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
		Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
	});
	ArduinoOTA.onError([](ota_error_t error) {
		Serial.printf("Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
		else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
		else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
		else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
		else if (error == OTA_END_ERROR) Serial.println("End Failed");
	});
	ArduinoOTA.begin();
	Serial.println("OTA ready\r\n");
}



void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
	Serial.printf("Listing directory: %s\r\n", dirname);

	File root = fs.open(dirname);
	if (!root) {
		Serial.println("- failed to open directory");
		return;
	}
	if (!root.isDirectory()) {
		Serial.println(" - not a directory");
		return;
	}

	File file = root.openNextFile();
	while (file) {
		if (file.isDirectory()) {
			Serial.print("  DIR : ");
			Serial.println(file.name());
			if (levels) {
				listDir(fs, file.name(), levels - 1);
			}
		}
		else {
			Serial.print("  FILE: ");
			Serial.print(file.name());
			Serial.print("\tSIZE: ");
			Serial.println(file.size());
		}
		file = root.openNextFile();
	}
}

void startSPIFFS() { // Start the SPIFFS and list all contents
	SPIFFS.begin();                             // Start the SPI Flash File System (SPIFFS)
	Serial.println("SPIFFS started. Contents:");
	listDir(SPIFFS, "/", 0);
}


void startServer() { // Start a HTTP server with a file read handler and an upload handler

	server.on("/set", handleSet);

	server.on("/getData", handleGet);

	server.onNotFound(handleNotFound);          // if someone requests any other file or page, go to function 'handleNotFound'
												// and check if the file exists


	server.begin();



	//server.on("/edit.html", HTTP_POST, []() {  // If a POST request is sent to the /edit.html address,
	//	server.send(200, "text/plain", "");
	//}, handleFileUpload);                       // go to 'handleFileUpload'

	//server.onNotFound(handleNotFound);          // if someone requests any other file or page, go to function 'handleNotFound'
	//											// and check if the file exists

	//server.begin();                             // start the HTTP server
	//Serial.println("HTTP server started.");
}




void handleSet() {
	String message = "";
	Serial.println("sending settings to MyTag:");

	for (uint8_t i = 0; i < server.args(); i++) {
		if (server.argName(i) == "send") {
			digitalWrite(serialInterruptPin, HIGH);
			delay(10);
			Serial2.println(server.arg(i));
			digitalWrite(serialInterruptPin, LOW);
			Serial.println(server.arg(i));
			message += "Message sent to MyTag!";
		}
	}
	Serial.println();

	server.send(200, "text/plain", message);
}


void handleGet() {
	String tagsJSON = "[";  // crease JSON array of current Tag settings

	for (int i = 0; i < numberOfSlots; i++) {
		tagsJSON += slot[i].getJSON();
		if (i < numberOfSlots - 1) tagsJSON += ",";
	}

	tagsJSON += "]";
	
	Serial.println(tagsJSON);

	server.send(200, "text/plain", tagsJSON);
}

String responseHTML = ""
"<meta http-equiv='refresh' content='0; url=gui.html' />";

void handleNotFound() { // if the requested file or page doesn't exist, return a 404 not found error
	//if(server.uri().endsWith("/"))
	//	server.send(200, "text/html", responseHTML);
	
	if (!handleFileRead(server.uri())) {        // check if the file exists in the flash memory (SPIFFS), if so, send it
		server.send(200, "text/html", responseHTML);
	}
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
	Serial.println("handleFileRead: " + path);
	if (path.endsWith("/")) path += "index.html";          // If a folder is requested, send the index file
	String contentType = getContentType(path);             // Get the MIME type
	String pathWithGz = path + ".gz";
	if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) { // If the file exists, either as a compressed archive, or normal
		if (SPIFFS.exists(pathWithGz))                         // If there's a compressed version available
			path += ".gz";                                         // Use the compressed verion
		File file = SPIFFS.open(path, "r");                    // Open the file
		size_t sent = server.streamFile(file, contentType);    // Send it to the client
		file.close();                                          // Close the file again
		Serial.println(String("\tSent file: ") + path);
		return true;
	}
	Serial.println(String("\tFile Not Found: ") + path);   // If the file doesn't exist, return false
	return false;
}

String getContentType(String filename) { // determine the filetype of a given filename, based on the extension
	if (filename.endsWith(".html")) return "text/html";
	else if (filename.endsWith(".css")) return "text/css";
	else if (filename.endsWith(".js")) return "application/javascript";
	else if (filename.endsWith(".ico")) return "image/x-icon";
	else if (filename.endsWith(".gz")) return "application/x-gzip";
	return "text/plain";
}

