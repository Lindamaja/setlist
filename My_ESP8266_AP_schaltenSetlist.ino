// Access Point modus  WLAN-Hotspot-LED "Setlist"
 
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
 
String html1 = "<!DOCTYPE html>\r\n<html>\r\n\
<head>\r\n<meta content=\"text/html; charset=ISO-8859-1\" http-equiv=\"content-type\">\r\n\
<title>WebSchalter</title>\r\n\
<form action=\"";
// String((val)?"/1.html":"/0.html")
String html2 = "\">\r\n<input value=\"ON/OFF\" style=\"";
String html3 = " width:5em;height:3em; font-size: 64px;\" type=\"submit\">\
</form>\r\n</head>\r\n<body>\r\n</body>\r\n</html>";

IPAddress Ip(192, 168, 1, 1);
  IPAddress NMask(255, 255, 255, 0);



 
ESP8266WebServer server(80);    // Server Port
int val = 1;                    //state of server: off
String Temp = "";
 
void Ereignis_SchalteON()       // will execute when "http://<ip address>/1.html"
{
  val = 0;                      // Relay off
  digitalWrite(12, val);         // GPI12
  Temp = html1 + String((val) ? "/1.html" : "/0.html");
  Temp += html2 + String((val) ? "BACKGROUND-COLOR: Chartreuse;" : "BACKGROUND-COLOR: DarkGray;") + html3; // Changing Backgroundcolor on/off
  server.send(200, "text/html", Temp);
}
 
void Ereignis_SchalteOFF()  // will execute "http://<ip address>//0.html"
{
  val = 1;                      // Relay on
  digitalWrite(12, val);         // GPI12
  Temp = html1 + String((val) ? "/1.html" : "/0.html");
  Temp += html2 + String((val) ? "BACKGROUND-COLOR: Chartreuse;" : "BACKGROUND-COLOR: DarkGray;") + html3;
  server.send(200, "text/html", Temp);
}
 
void Ereignis_Index()           // will execute when "http://<ip address>/" 
{
  Temp = html1 + String((val) ? "/1.html" : "/0.html");
  Temp += html2 + String((val) ? "BACKGROUND-COLOR: Chartreuse;" : "BACKGROUND-COLOR: DarkGray;") + html3;
  server.send(200, "text/html", Temp);
}
 
void setup()
{
  digitalWrite(0, 1);           // initial state at 1 (Relay off)
  pinMode(12, OUTPUT);           // GPI12 = output
  pinMode(2, INPUT_PULLUP);     // GPIO2 = input with Pullup 
  digitalWrite(0, 1);           // initial state 1 (Relay off)
 
  Serial.begin(115200);         // serial interface
  Serial.println("");           // empty line
  Serial.println("Starte WLAN-Hotspot \"Setlist\"");
  WiFi.mode(WIFI_AP);           // access point modus
  WiFi.softAPConfig(Ip, Ip, NMask);

  IPAddress myIP = WiFi.softAPIP();
  WiFi.softAP("Setlist", "dernordensingt");    // NAME + PASSWORD of Wi-Fi
  delay(500);                   //wait for 0,5s
  Serial.print("IP Adresse ");  //current IP address of server
  Serial.println(WiFi.softAPIP());
 
  server.on("/", Ereignis_Index);
  server.on("/1.html", Ereignis_SchalteON);
  server.on("/0.html", Ereignis_SchalteOFF);
 
  server.begin();               // Start  server
  Serial.println("HTTP Server gestartet");
}
 
void loop()
{
  server.handleClient();
  if (!digitalRead(2))          //If button on GPI02 pressed
  {
    val = !val;                 // switch status
    digitalWrite(0, val);
    while (!digitalRead(2))server.handleClient(); // wait until button released
  }
}
