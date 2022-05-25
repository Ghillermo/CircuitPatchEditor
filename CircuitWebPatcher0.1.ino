/*Real time midi parameter editor example for Novation Circuit.
 * Connect TRS jack: Tip V+, Ring Data(tx), Sleeve GND.
 * Introduce your net credentials and preferred IP below. 
 * Then just access that IP using a browser.
 * Based mostly on Websockets library examples and Circuits Programmers Reference Guide.
 */

#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <MIDI.h>
#include <ESP8266WiFiMulti.h>
#include <string.h>

#define USE_SERIAL Serial

const char * ssid = "YOUR_AP_SSID";
const char * pw = "YOUR_AP_PASSWORD";
IPAddress local_IP(192, 168, 1, 4); //DEVICE'S IP
IPAddress gateway(192, 168, 1, 1); //YOUR ROUTER
IPAddress subnet(255, 255, 255, 0);

ESP8266WiFiMulti WiFiMulti;
WebSocketsServer webSocket = WebSocketsServer(81);
ESP8266WebServer server(80); 




class CircuitCC{ //CC object
  public:
    byte CC,Min,Max;
    String label;
    unsigned int value;
    CircuitCC(){
      CC=0; //Command
      Min=0; //Minimum value
      Max=0; //Maximum value
      label="\0"; //Name
      value=0; //Value, currently unused
      };
    CircuitCC(byte cc,byte m,byte M,String lab){ 
      CC=cc;
      Min=m;
      Max=M;
      label=lab;
      value=0;
      };
     void setCC(String lab, byte cc,byte m,byte M,byte defval){
      CC=cc;
      Min=m;
      Max=M;
      label=lab;
      value=defval;
      };
      void setVal(int val){
        value=val;
        };
  };
  
MIDI_CREATE_DEFAULT_INSTANCE();
CircuitCC command[80];
static int numberofCC;

int makeCClist(){ //Create a list with all CCs: 
  //RegExP:(.+) CC ([0-9]+) ([0-9]+) – ([0-9]+)(.*) ([0-9]+)(.*) | command[i].setCC("$1",$2,$3,$4,$6); i++;\n 
  int i=0;
  command[i].setCC("Polyphony Mode",3,0,2,2); i++;
command[i].setCC("Portamento Rate",5,0,127,0); i++;
command[i].setCC("Pre-Glide",9,52,76,64); i++;
command[i].setCC("Keyboard Octave",13,58,69,68); i++;
command[i].setCC("osc 1 wave",19,0,29,2); i++;
command[i].setCC("osc 1 wave interpolate",20,0,127,0); i++;
command[i].setCC("osc 1 pulse width index",21,0,127,127); i++;
command[i].setCC("osc 1 virtual sync depth",22,0,127,0); i++;
command[i].setCC("osc 1 density",24,0,127,0); i++;
command[i].setCC("osc 1 density detune",25,0,127,0); i++;
command[i].setCC("osc 1 semitones",26,0,127,64); i++;
command[i].setCC("osc 1 cents",27,0,127,64); i++;
command[i].setCC("osc 1 pitchbend",28,52,76,76); i++;
command[i].setCC("osc 2 wave",29,0,29,2); i++;
command[i].setCC("osc 2 wave interpolate",30,0,127,0); i++;
command[i].setCC("osc 2 pulse width index",31,0,127,127); i++;
command[i].setCC("osc 2 virtual sync depth",33,0,127,0); i++;
command[i].setCC("osc 2 density",35,0,127,0); i++;
command[i].setCC("osc 2 density detune",36,0,127,0); i++;
command[i].setCC("osc 2 semitones",37,0,127,64); i++;
command[i].setCC("osc 2 cents",39,0,127,64); i++;
command[i].setCC("osc 2 pitchbend",40,52,76,76); i++;
command[i].setCC("osc 1 level",51,0,127,127); i++;
command[i].setCC("osc 2 level",52,0,127,0); i++;
command[i].setCC("ring mod level",54,0,127,0); i++;
command[i].setCC("noise level",56,0,127,0); i++;
command[i].setCC("pre FX level",58,52,82,64); i++;
command[i].setCC("post FX level",59,52,82,64); i++;
command[i].setCC("routing",60,0,2,0); i++;
command[i].setCC("drive",63,0,127,0); i++;
command[i].setCC("drive type",65,0,6,0); i++;
command[i].setCC("type",68,0,5,1); i++;
command[i].setCC("frequency",74,0,127,127); i++;
command[i].setCC("tracking",69,0,127,127); i++;
command[i].setCC("resonance",71,0,127,0); i++;
command[i].setCC("Q normalize",78,0,127,64); i++;
command[i].setCC("env 2 to frequency",79,0,127,64); i++;
command[i].setCC("env 1 velocity",108,0,127,64); i++;
command[i].setCC("env 1 attack",73,0,127,2); i++;
command[i].setCC("env 1 decay",75,0,127,90); i++;
command[i].setCC("env 1 sustain",70,0,127,127); i++;
command[i].setCC("env 1 release",72,0,127,40); i++;
command[i].setCC("distortion level",91,0,127,0); i++;
command[i].setCC("chorus level",93,0,127,0); i++;
command[i].setCC("macro knob 1 position",80,0,127,0); i++;
command[i].setCC("macro knob 2 position",81,0,127,0); i++;
command[i].setCC("macro knob 3 position",82,0,127,0); i++;
command[i].setCC("macro knob 4 position",83,0,127,0); i++;
command[i].setCC("macro knob 5 position",84,0,127,0); i++;
command[i].setCC("macro knob 6 position",85,0,127,0); i++;
command[i].setCC("macro knob 7 position",86,0,127,0); i++;
command[i].setCC("macro knob 8 position",87,0,127,0); i++;
return i;
  }

//html document parts:
//parent doc:
const char html[] PROGMEM = R"rawliteral(<html><head><script>var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);connection.onopen = function () {  connection.send('Connect ' + new Date()); }; connection.onerror = function (error) {    console.log('WebSocket Error ', error);};connection.onmessage = function (e) {  console.log('Server: ', e.data);}; )rawliteral";
//JS functions: (read a value and send to websocket
const char functioncc[] PROGMEM  = R"rawliteral(function sendcc%ID() {  var val = parseInt(document.getElementById('id%ID').value).toString(16); var cc=%ID; cc=cc.toString(16);  if(cc.length < 2)  { cc = '0' + cc; }if(val.length < 2)  { val = '0' + val; }   var command = '#'+cc+val;    console.log('com: ' + command); connection.send(command); };)rawliteral";
//Slider: (with adequate parameters)
const char inputcc[] PROGMEM = R"rawliteral(<br>%NAME: <input id="id%ID" type="range" min="%MIN" max="%MAX" step="1" oninput="sendcc%ID();" ><br>)rawliteral";
   
String funciones(int numparams){ //Write all JS CC send functions:
  String output = "";
  for (int i=0;i<numparams;i++){
    String aux = String(functioncc);
   // String aux2 = String(command[i].CC);
    aux.replace("%ID",String(command[i].CC));
    output=output+ aux;
  }
    return output;
  }

String sliders(int numparams){ //Write all CC sliders:
  String output = ""; 
  for (int i=0;i<numparams;i++){
    String aux = String(inputcc);
    aux.replace("%ID",String(command[i].CC));
    aux.replace("%NAME",command[i].label);
    aux.replace("%MIN",String(command[i].Min));
    aux.replace("%MAX",String(command[i].Max));
    output= output+ aux;
    }

    return output;
  }
String buildPage() //This puts the doc together.
{
 
 // Serial.println(var);
    Serial.print("LLamando a f con número: ");
    Serial.println(numberofCC);
   // String f = funciones(numberofCC); 
    Serial.print("LLamando a s con número: ");
    Serial.println(numberofCC);
    //String s = sliders(numberofCC);
   // return String(html) + funciones(numberofCC) + "</script></head><body>" + sliders(numberofCC) + "</body></html>";
  return  String(html) + funciones(numberofCC) + "</script></head><body>" + sliders(numberofCC) + "</body></html>";

}
  
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
 //Handle websocket
    switch(type) {
        case WStype_DISCONNECTED:
            //USE_SERIAL.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);
            //USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

            // send message to client
            webSocket.sendTXT(num, "Connected");
        }
            break;
        case WStype_TEXT:
            //USE_SERIAL.printf("[%u] get Text: %s\n", num, payload);
           byte   canal = 1;
           
            if(payload[0] == '#') { //Read payload (it's always #CCVV, where CC is 2 hex digit CC and VV is 2 hex dig value).
                
                uint32_t msg = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
                             
                byte CC = (msg >> 8) & 0xFF;
                byte val = (msg >> 0) & 0xFF;

              MIDI.sendControlChange(CC, val, canal);
              delay(10);
            }
            break;
    }
}

void handleRoot() {
  server.send(200, "text/html",  buildPage());
}

void setup() {
  //USE_SERIAL.begin(9600);
  MIDI.begin();    
  numberofCC=makeCClist();
   // //USE_SERIAL.begin(57600);
    

    for(uint8_t t = 4; t > 0; t--) {
        //USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
        //USE_SERIAL.flush();
        delay(1000);
    }
     if (!WiFi.config(local_IP, gateway, subnet)) {
    //USE_SERIAL.println("STA Failed to configure");
  }
   WiFiMulti.addAP(ssid, pw);

    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }

    if(MDNS.begin("esp8266")) {
        //USE_SERIAL.println("MDNS responder started");
    }

    // handle index

    server.on("/", HTTP_GET, handleRoot); 
   
    webSocket.begin();
     webSocket.onEvent(webSocketEvent);

    server.begin();

    // Add service to MDNS
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("ws", "tcp", 81);


}

unsigned long last_10sec = 0;
unsigned int counter = 0;

void loop() {
    unsigned long t = millis();
    webSocket.loop();
     server.handleClient();

    if((t - last_10sec) > 10 * 1000) {
        counter++;
        bool ping = (counter % 2);
        int i = webSocket.connectedClients(ping);
        //USE_SERIAL.printf("%d Connected websocket clients ping: %d\n", i, ping);
        last_10sec = millis();
    }
}
