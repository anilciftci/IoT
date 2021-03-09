
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

int random_t;
int random_m;
const char *ssid = "SUPERONLINE_WiFi_0927";  //ENTER YOUR WIFI SETTINGS
const char *password = "vA3enh4VFjy5";

const char *host = "iot-project-28689-default-rtdb.europe-west1.firebasedatabase.app";
const int httpsPort = 443;  
const char fingerprint[] PROGMEM = "08:6A:76:32:10:AD:23:A9:D8:F5:8C:75:AB:8B:D9:FF:21:2C:9A:AD";

void setup() {
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //Only Station No AP, This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); 
}

void loop() {
  WiFiClientSecure httpsClient;   
  random_t = random(30,45);
  random_m = random(25,50); 
  Serial.println(host);
  Serial.printf("Using fingerprint '%s'\n", fingerprint);
  httpsClient.setFingerprint(fingerprint);
  httpsClient.setTimeout(15000); 
  delay(1000);
  
  Serial.print("HTTPS Connecting");
  int r=0; //retry counter
  while((!httpsClient.connect(host, httpsPort)) && (r < 30)){
      delay(100);
      Serial.print(".");
      r++;
  }
  if(r==30) {
    Serial.println("Connection failed");
  }
  else {
    Serial.println("Connected to web");
  }
  
  String  Link;
 
  //GET Data
  Link = "/sensors/sensor0.json";

  Serial.print("requesting URL: ");
  Serial.println(host+Link);
  String data = String("{\"description\":\"yeni_test\",\"tempature\":\"")+random_t+"\",\"moisture\":\""+random_m+"\"}"; 
  httpsClient.print(String("PUT ") + Link + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +   
               "Content-Length: "+  data.length() +"\r\n"+     
               "Connection: close\r\n\r\n" +
               data);

  Serial.println("request sent");
                  
  while (httpsClient.connected()) {
    String line = httpsClient.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }

  Serial.println("reply was:");
  Serial.println("==========");
  String line;
  while(httpsClient.available()){  
      
    line = httpsClient.readStringUntil('\n');  //Read Line by Line
    Serial.println(line); //Print response
  }
  Serial.println("==========");
  Serial.println("closing connection");
    
  delay(2000);  
}
//==
