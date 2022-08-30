#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#define pixelCount 24
#define pixelPin D1

Adafruit_NeoPixel strip(pixelCount, pixelPin, NEO_GRB + NEO_KHZ800);
const char*  ssid = "#";
const char*  password = "#";

WiFiServer server(80);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(25);
  WiFi.begin(ssid, password);
  server.begin();
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("..."); 
  } 

  Serial.print("");
  Serial.println(WiFi.localIP());
}
 
void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  
  if (request.indexOf("color") != -1) {
      String redValue = request.substring(15, 17);
      String greenValue = request.substring(17, 19);
      String blueValue = request.substring(19, 21);

      //int ledIndex = request.substring(27, 29).toInt();
      
      Serial.println(redValue);
      Serial.println(greenValue);
      Serial.println(blueValue);
      
      char redArray[3];
      char greenArray[3];
      char blueArray[3];
    
      redValue.toCharArray(redArray, 3);
      greenValue.toCharArray(greenArray, 3);
      blueValue.toCharArray(blueArray, 3);
    
      int red = strtol(redArray, 0, 16);
      int green = strtol(greenArray, 0, 16);
      int blue = strtol(blueArray, 0, 16);

      
      strip.fill(strip.Color(red, green, blue) 0, pixelCount);
      strip.show();
  }

  //Start Request
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE html>");
  String htmlResponse = 
    "<html>"
    "<body>"
    " <form>"
    "  <label for=\"color\">Select your favorite color:</label> <br/>"
    "  <input type=\"color\" id=\"color\" name=\"color\" value=\"#ff0000\"> <br/>"  
    //"  <input type='range' name = 'led' min=0 max=23 value=12 oninput='this.nextElementSibling.value = this.value'>"
    //"  <output>12</output>"
    " <input type=\"submit\">"
    " </form>"
    "</body>"
    "<style>"
    "body { font-family: Comic Sans MS }"
    "</style>"
    "</html>";

  client.println(htmlResponse);

}
