
#include <MKRNB.h>
#include <ArduinoHttpClient.h>
#include "src/propeller.h" 

// PIN Number
const char PINNUMBER[]  = "1503";


// initialize the library instance
NBClient nbClient;
GPRS gprs;
NB nbAccess(true);

// URL, path and port (for example: example.org)
char server[] = "example.org";
char path[] = "/";
int port = 80; // port 80 is the default for HTTP

HttpClient httpClient = HttpClient(nbClient, server, port);

void setup() {

  Serial.begin(115200);

  while (!Serial);
  Serial.println("start");
  prop_setup();

  Serial.println("Starting Arduino web client.");

  boolean connected = false;
  while (!connected) {
    if ((nbAccess.begin(PINNUMBER) == NB_READY) &&
        (gprs.attachGPRS() == GPRS_READY)) {
      connected = true;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("connecting...");

}


void loop() {

  float windSpeed = getWindSpeed();
  Serial.print(windSpeed);
  Serial.println(" Km/h");

  Serial.println("making POST request");
  String contentType = "application/x-www-form-urlencoded";
  String postData = String(windSpeed);

  httpClient.post("/", contentType, postData);

  // read the status code and body of the response
  int statusCode = httpClient.responseStatusCode();
  String response = httpClient.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

  delay(2000);

}

