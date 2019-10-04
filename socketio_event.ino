#include <ArduinoJson.h>
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <SocketIoClient.h>

//WIFI LOGIN
#ifndef STASSID
#define STASSID "SSID" //CHANGE TO YOUR NETWORK NAME
#define STAPSK  "123456789" //CHANGE TO YOUR NETWORK PASSWORD
#endif

//SOCKETIO
#define SOCKETIO "192.168.0.10" //CHANGE TO THE IP OF THE SOCKET IO SERVER

//CODE----------------------------
const char* ssid     = STASSID;
const char* password = STAPSK;


ESP8266WiFiMulti WiFiMulti;
SocketIoClient socket;

void setup() {
  Serial.begin(115200);
  //Setup Pin (intern LED)
  pinMode(D4, OUTPUT);
  //Connect to Wifi Network:
  connectWifi();
  //Connect to SocketIO:
  Serial.println("Connecting to Socket:");
  //Handle 'switch_change' event
  socket.on("switch_change", onswitchchange);
  socket.begin(SOCKETIO);
  Serial.println("Connected!");

}

void loop() {
  //Run Socket
  socket.loop();
}
void onswitchchange(const char * payload, size_t length) {
    //is fired when socketio receives 'switch_change'
    //remove escape characters: '\'
    char * json = charTrim(payload, '\\');
    Serial.println(json);
    //Start json parsing. Buffer size may be to small. Check here: https://arduinojson.org/v6/assistant/
    const int capacity = JSON_OBJECT_SIZE(2) + 20;
    StaticJsonDocument<capacity> doc;
    DeserializationError  error = deserializeJson(doc, json);
    //Handle DeserializationError:
    if (error) {
      Serial.print(F("deserializeJson() failed with code "));
      Serial.println(error.c_str());
      return;
    }
    // doc contains the json object.
    //Get Variables:
    const char* json_name = doc["name"]; // "schalter_1"
    bool json_val = doc["val"]; // true || false
    //do Something with the result:
    //only toggle led if the 'name' of the JSON payload fits
    if (std::string(json_name) == "switch_1"){
      if(json_val == 1){
        //Turn LED ON
        digitalWrite(D4, LOW);
      }else{
        //Turn LED OFF
        digitalWrite(D4, HIGH);
      }
    }
}
void connectWifi() {
  //Connects ESP2866 Wifi
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  //Wait until connection is established:
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  //Wifi connected, print IP
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(500);
}

char* charTrim(const char* stringToTrim, char charsToTrim) {
  //Removes chars from any char sequence (in this case the escape character '\' -> Line 47)
  String output;
  String toTrim = String(stringToTrim);
  for (int i = 0; i < toTrim.length(); i++) {
    if (toTrim.charAt(i) != charsToTrim) {
      output += toTrim.charAt(i);  
    }
  }
  return const_cast<char*>(output.c_str());
}
