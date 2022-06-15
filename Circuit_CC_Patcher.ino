/*Real time MIDI parameter editor example for Novation Circuit.
   Connect TRS jack: Tip 3.3V, Ring Data(tx), Sleeve GND.
   Introduce your net credentials and preferred IP below.
   Then just access the IP 192.168.1.4 using any device and browser.
   Based Websockets library examples
   and Circuits Programmers Reference Guide.

*/

#include <Arduino.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <MIDI.h>
#include <ESP8266WiFiMulti.h>
#include <string.h>

#define MY_SSID  "albaricoque"
#define MY_PW  "platanoyfresa"

#define USE_SERIAL Serial

static int CANAL = 1; //Change this for midi channel (synth1 on ch1 and synth2 on ch2 by defalt



IPAddress local_IP(192, 168, 1, 101); //ESP's IP
IPAddress gateway(192, 168, 1, 1); //YOUR ROUTER's IP
IPAddress subnet(255, 255, 255, 0);
const char global_html[] PROGMEM = R"rawliteral(<html><head><meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body {
  background-image: linear-gradient(to bottom right,  #101010, #606060);
  color: powderblue;
}
select {
  font-size:14px;
  border-style: ridge;
  padding: 2px;
  border-radius: 5px;
   margin-top: 1%; 
  }
  
.slidercontainer{
   margin-top: 1%; 
 margin-bottom: 1%;
}

.slider {
  -webkit-appearance: none;
  height: 10px;
  background: Black;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s;
  border-style: inset;
  border-color: DarkGray;
 margin-top: 1%; 
 margin-bottom: 1%;
 border-radius: 6px;

}

.slider:hover {
  opacity: 1;
  background:#00ffca
}
.slider:checked {
  opacity: 1;
  background:#ca00ff
}


.slider::-webkit-slider-thumb {
   border-style: outset;
  border-style: outset;
  border-color: DarkGray;
  -webkit-appearance: none;
  appearance: none;
  width: 4px;
  height: 16px;
  background: #ffca00;
  cursor: pointer;
   border-radius: 6px;
}

.slider::-moz-range-thumb {
  border-style: outset;
  width: 4px;
  height: 16px;
  background: #ffca00;
  cursor: pointer;
  border-radius: 6px;
}
</style>
<script>var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);connection.onopen = function () {  connection.send('Connect ' + new Date()); }; connection.onerror = function (error) {    console.log('WebSocket Error ', error);};connection.onmessage = function (e) {  console.log('Server: ', e.data);};
function sendcc(cc,val) { hexV = parseInt(val).toString(16); 
hexC = cc.toString(); 
if(hexV.length < 2) { hexV = '0' + hexV; }  
if(hexC.length < 2) { hexC = '0' + hexC; }  
var command = 'G'+hexC+hexV; connection.send(command); var output = document.getElementById("id"+cc); output.innerHTML = val; }; 
function sendNPRN(cc,val) {hexV = parseInt(val).toString(16); 
hexC = cc.toString(); 
if(hexV.length < 2) { hexV = '0' + hexV; }  
while (hexC.length < 4) { hexC = '0' + hexC; }  
var command = 'g'+hexC+hexV.toString(16); connection.send(command); var output = document.getElementById("id"+cc); output.innerHTML = val; }; 
</script></head>
<body>
<div class=slidercontainer><a href="/assign">Asignar Knobs</a></div>
<div class=slidercontainer><a href="/modmatrix">Modulation matrix</a></div>
<div class=slidercontainer><a href="/drums">Drums</a></div>
<div class=slidercontainer><a href="/">CC & NRPN</a></div>
<div class=slidercontainer><a href="/global">Global</a></div>

<div class=slidercontainer><h2>Reverb:</h2></div>
<div class=slidercontainer>
synth 1 send level:<br><input class="slider" id="58" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id58"></span><br>
</div>

<div class=slidercontainer>
synth 2 send level:<br><input class="slider" id="59" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id59"></span><br>
</div>

<div class=slidercontainer>
drum 1 send level:<br><input class="slider" id="5a" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id5a"></span><br>
</div>

<div class=slidercontainer>
drum 2 send level:<br><input class="slider" id="6a" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id6a"></span><br>
</div>

<div class=slidercontainer>
drum 3 send level:<br><input class="slider" id="6d" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id6d"></span><br>
</div>

<div class=slidercontainer>
drum 4 send level:<br><input class="slider" id="6e" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id6e"></span><br>
</div>
<div class=slidercontainer>
type:<br><input class="slider" id="0112" type="range" min="0" max="5" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0112"></span><br>
</div>

<div class=slidercontainer>
decay:<br><input class="slider" id="0113" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0113"></span><br>
</div>

<div class=slidercontainer>
damping:<br><input class="slider" id="0114" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0114"></span><br>
</div>

<div class=slidercontainer><h2>Delay:</h2></div>
<div class=slidercontainer>
synth 1 send level:<br><input class="slider" id="6f" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id6f"></span><br>
</div>

<div class=slidercontainer>
synth 2 send level:<br><input class="slider" id="70" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id70"></span><br>
</div>

<div class=slidercontainer>
drum 1 send level:<br><input class="slider" id="71" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id71"></span><br>
</div>

<div class=slidercontainer>
drum 2 send level:<br><input class="slider" id="72" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id72"></span><br>
</div>

<div class=slidercontainer>
drum 3 send level:<br><input class="slider" id="73" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id73"></span><br>
</div>

<div class=slidercontainer>
drum 4 send level:<br><input class="slider" id="74" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id74"></span><br>
</div>

<div class=slidercontainer>
time:<br><input class="slider" id="0106" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0106"></span><br>
</div>

<div class=slidercontainer>
time sync:<br><input class="slider" id="0107" type="range" min="0" max="35" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0107"></span><br>
</div>

<div class=slidercontainer>
feedback:<br><input class="slider" id="0108" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0108"></span><br>
</div>

<div class=slidercontainer>
width:<br><input class="slider" id="0109" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0109"></span><br>
</div>

<div class=slidercontainer>
left-right ratio:<br><input class="slider" id="010a" type="range" min="0" max="12" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id010a"></span><br>
</div>

<div class=slidercontainer>
slew rate:<br><input class="slider" id="010b" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id010b"></span><br>
</div>

<div class=slidercontainer><h2>Filter:</h2></div>
<div class=slidercontainer>
frequency:<br><input class="slider" id="4a" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id4a"></span><br>
</div>

<div class=slidercontainer>
resonance:<br><input class="slider" id="47" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id47"></span><br>
</div>
<div class=slidercontainer><h2>Mixer:</h2></div>
<div class=slidercontainer>
synth 1 level:<br><input class="slider" id="c" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="idc"></span><br>
</div>

<div class=slidercontainer>
synth 2 level:<br><input class="slider" id="e" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="ide"></span><br>
</div>

<div class=slidercontainer>
synth 1 pan:<br><input class="slider" id="75" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id75"></span><br>
</div>

<div class=slidercontainer>
synth 2 pan:<br><input class="slider" id="76" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id76"></span><br>
</div>
<div class=slidercontainer><h2>Sidechain:</h2></div>
<div class=slidercontainer>
synth 1 source:<br><input class="slider" id="0237" type="range" min="0" max="4" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0237"></span><br>
</div>

<div class=slidercontainer>
synth 1 attack:<br><input class="slider" id="0238" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0238"></span><br>
</div>

<div class=slidercontainer>
synth 1 hold:<br><input class="slider" id="0239" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0239"></span><br>
</div>

<div class=slidercontainer>
synth 1 decay:<br><input class="slider" id="023a" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id023a"></span><br>
</div>

<div class=slidercontainer>
synth 1 depth:<br><input class="slider" id="023b" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id023b"></span><br>
</div>

<div class=slidercontainer>
synth 2 source:<br><input class="slider" id="0241" type="range" min="0" max="4" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0241"></span><br>
</div>

<div class=slidercontainer>
synth 2 attack:<br><input class="slider" id="0242" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0242"></span><br>
</div>

<div class=slidercontainer>
synth 2 hold:<br><input class="slider" id="0243" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0243"></span><br>
</div>

<div class=slidercontainer>
synth 2 decay:<br><input class="slider" id="0244" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0244"></span><br>
</div>

<div class=slidercontainer>
synth 2 depth:<br><input class="slider" id="0145" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0145"></span><br>
</div>

<div class=slidercontainer>
FX Bypass:<br><input class="slider" id="0115" type="range" min="0" max="1" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0115"></span><br>
</div>
</body></html>
)rawliteral";

const char drums_html[] PROGMEM =R"rawliteral(<html><head><meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body {
  background-image: linear-gradient(to bottom right,  #101010, #606060);
  color: powderblue;
}
select {
  font-size:14px;
  border-style: ridge;
  padding: 2px;
  border-radius: 5px;
   margin-top: 1%; 
  }

.slidercontainer{
   margin-top: 1%; 
 margin-bottom: 1%;
}

.slider {
  -webkit-appearance: none;
  height: 10px;
  background: Black;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s;
  border-style: inset;
  border-color: DarkGray;
 margin-top: 1%; 
 margin-bottom: 1%;
 border-radius: 6px;

}

.slider:hover {
  opacity: 1;
  background:#00ffca
}
.slider:checked {
  opacity: 1;
  background:#ca00ff
}


.slider::-webkit-slider-thumb {
   border-style: outset;
  border-style: outset;
  border-color: DarkGray;
  -webkit-appearance: none;
  appearance: none;
  width: 4px;
  height: 16px;
  background: #ffca00;
  cursor: pointer;
   border-radius: 6px;
}

.slider::-moz-range-thumb {
  border-style: outset;
  width: 4px;
  height: 16px;
  background: #ffca00;
  cursor: pointer;
  border-radius: 6px;
}
</style>
<script>var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);connection.onopen = function () {  connection.send('Connect ' + new Date()); }; connection.onerror = function (error) {    console.log('WebSocket Error ', error);};connection.onmessage = function (e) {  console.log('Server: ', e.data);};
function sendcc(cc,val) { hexV = parseInt(val).toString(16); 
hexC = cc.toString(); 
if(hexV.length < 2) { hexV = '0' + hexV; }  
if(hexC.length < 2) { hexC = '0' + hexC; }  
var command = 'D'+hexC+hexV; connection.send(command); var output = document.getElementById("id"+cc); output.innerHTML = val; }; 
</script></head>
<body>
<div class=slidercontainer><a href="/assign">Asignar Knobs</a></div>
<div class=slidercontainer><a href="/modmatrix">Modulation matrix</a></div>
<div class=slidercontainer><a href="/drums">Drums</a></div>
<div class=slidercontainer><a href="/">CC & NRPN</a></div>
<div class=slidercontainer><a href="/global">Global</a></div>
<div class=slidercontainer>
drum 1 patch select:<br><input class="slider" id="8" type="range" min="0" max="63" step="1" oninput="sendcc(this.id,this.value);" ><span id="id8"></span><br>
</div>

<div class=slidercontainer>
drum 1 level:<br><input class="slider" id="c" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="idc"></span><br>
</div>

<div class=slidercontainer>
drum 1 pitch:<br><input class="slider" id="e" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="ide"></span><br>
</div>

<div class=slidercontainer>
drum 1 decay:<br><input class="slider" id="f" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="idf"></span><br>
</div>

<div class=slidercontainer>
drum 1 distortion:<br><input class="slider" id="10" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id10"></span><br>
</div>

<div class=slidercontainer>
drum 1 EQ:<br><input class="slider" id="11" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id11"></span><br>
</div>

<div class=slidercontainer>
drum 1 pan:<br><input class="slider" id="4d" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id4d"></span><br>
</div>

<div class=slidercontainer>
drum 2 patch select:<br><input class="slider" id="12" type="range" min="0" max="63" step="1" oninput="sendcc(this.id,this.value);" ><span id="id12"></span><br>
</div>

<div class=slidercontainer>
drum 2 level:<br><input class="slider" id="17" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id17"></span><br>
</div>

<div class=slidercontainer>
drum 2 pitch:<br><input class="slider" id="22" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id22"></span><br>
</div>

<div class=slidercontainer>
drum 2 decay:<br><input class="slider" id="28" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id28"></span><br>
</div>

<div class=slidercontainer>
drum 2 distortion:<br><input class="slider" id="2a" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id2a"></span><br>
</div>

<div class=slidercontainer>
drum 2 EQ:<br><input class="slider" id="2b" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id2b"></span><br>
</div>

<div class=slidercontainer>
drum 2 pan:<br><input class="slider" id="4e" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id4e"></span><br>
</div>

<div class=slidercontainer>
drum 3 patch select:<br><input class="slider" id="2c" type="range" min="0" max="63" step="1" oninput="sendcc(this.id,this.value);" ><span id="id2c"></span><br>
</div>

<div class=slidercontainer>
drum 3 level:<br><input class="slider" id="2d" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id2d"></span><br>
</div>

<div class=slidercontainer>
drum 3 pitch:<br><input class="slider" id="2e" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id2e"></span><br>
</div>

<div class=slidercontainer>
drum 3 decay:<br><input class="slider" id="2f" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id2f"></span><br>
</div>

<div class=slidercontainer>
drum 3 distortion:<br><input class="slider" id="30" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id30"></span><br>
</div>

<div class=slidercontainer>
drum 3 EQ:<br><input class="slider" id="31" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id31"></span><br>
</div>

<div class=slidercontainer>
drum 3 pan:<br><input class="slider" id="4f" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id4f"></span><br>
</div>

<div class=slidercontainer>
drum 4 patch select:<br><input class="slider" id="32" type="range" min="0" max="63" step="1" oninput="sendcc(this.id,this.value);" ><span id="id32"></span><br>
</div>

<div class=slidercontainer>
drum 4 level:<br><input class="slider" id="35" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id35"></span><br>
</div>

<div class=slidercontainer>
drum 4 pitch:<br><input class="slider" id="37" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id37"></span><br>
</div>

<div class=slidercontainer>
drum 4 decay:<br><input class="slider" id="39" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id39"></span><br>
</div>

<div class=slidercontainer>
drum 4 distortion:<br><input class="slider" id="3d" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id3d"></span><br>
</div>

<div class=slidercontainer>
drum 4 EQ:<br><input class="slider" id="4c" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id4c"></span><br>
</div>

<div class=slidercontainer>
drum 4 pan:<br><input class="slider" id="50" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id50"></span><br>
</div>
</body></html>
)rawliteral";
const char full_html[] PROGMEM = R"rawliteral(
<html><head><meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body {
  background-image: linear-gradient(to bottom right,  #101010, #606060);
  color: powderblue;
}
select {
  font-size:14px;
  border-style: ridge;
  padding: 2px;
  border-radius: 5px;
   margin-top: 1%; 
  }
.button {
  border: none;
  color: #808080;
  padding: 6px 12px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 16px;
  margin: 2px 1px;
  cursor: pointer;
    border-radius: 12px;
      background:#caff00;
}
.slidercontainer{
   margin-top: 1%; 
 margin-bottom: 1%;
}

.slider {
  -webkit-appearance: none;
  height: 10px;
  background: Black;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s;
  border-style: inset;
  border-color: DarkGray;
 margin-top: 1%; 
 margin-bottom: 1%;
 border-radius: 6px;

}

.slider:hover {
  opacity: 1;
  background:#00ffca
}
.slider:checked {
  opacity: 1;
  background:#ca00ff
}


.slider::-webkit-slider-thumb {
   border-style: outset;
  border-style: outset;
  border-color: DarkGray;
  -webkit-appearance: none;
  appearance: none;
  width: 4px;
  height: 16px;
  background: #ffca00;
  cursor: pointer;
   border-radius: 6px;
}

.slider::-moz-range-thumb {
  border-style: outset;
  width: 4px;
  height: 16px;
  background: #ffca00;
  cursor: pointer;
  border-radius: 6px;
}
</style>
<script>var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);connection.onopen = function () {  connection.send('Connect ' + new Date()); }; connection.onerror = function (error) {    console.log('WebSocket Error ', error);};connection.onmessage = function (e) {  console.log('Server: ', e.data);};
function sendcc(cc,val) { hexV = parseInt(val).toString(16); 
hexC = cc.toString(); 
if(hexV.length < 2) { hexV = '0' + hexV; }  
if(hexC.length < 2) { hexC = '0' + hexC; }  
var command = '#'+hexC+hexV; connection.send(command); var output = document.getElementById("id"+cc); output.innerHTML = val; }; 
function sendNPRN(cc,val) {hexV = parseInt(val).toString(16); 
hexC = cc.toString(); 
if(hexV.length < 2) { hexV = '0' + hexV; }  
while (hexC.length < 4) { hexC = '0' + hexC; }  
var command = 'N'+hexC+hexV.toString(16); connection.send(command); var output = document.getElementById("id"+cc); output.innerHTML = val; }; 
function canal() {
  connection.send('C');
  }
</script></head>
<body>
<div class=slidercontainer><a href="/assign">Asignar Knobs</a></div>
<div class=slidercontainer><a href="/modmatrix">Modulation matrix</a></div>
<div class=slidercontainer><a href="/drums">Drums</a></div>
<div class=slidercontainer><a href="/">CC & NRPN</a></div> 
<div class=slidercontainer><a href="/global">Global</a></div> 

<div class=slidercontainer><input type="button" class="button" value="Channel Switch" onclick="canal();"></div> 

<div class=slidercontainer>Polyphony Mode: <select id="3" oninput="sendcc(this.id,this.value)" ><option value="0">Mono</option><option value="1">Mono AG</option><option value="2">Poly</option></select></div>

<div class=slidercontainer>
Portamento Rate:<br><input class="slider" id="5" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id5"></span><br>
</div>

<div class=slidercontainer>
Pre-Glide:<br><input class="slider" id="9" type="range" min="52" max="76" step="1" oninput="sendcc(this.id,this.value);" ><span id="id9"></span><br>
</div>

<div class=slidercontainer>
Keyboard Octave:<br><input class="slider" id="d" type="range" min="58" max="69" step="1" oninput="sendcc(this.id,this.value);" ><span id="idd"></span><br>
</div>

<div class=slidercontainer>
osc 1 wave:<select id="13"oninput="sendcc(this.id,this.value)" ><option value="0">sine</option><option value="1">triangle</option><option value="2">sawtooth</option><option value="3">saw 9:1 PW</option><option value="4">saw 8:2 PW</option><option value="5">saw 7:3 PW</option><option value="6">saw 6:4 PW</option><option value="7">saw 5:5 PW</option><option value="8">saw 4:6 PW</option><option value="9">saw 3:7 PW</option><option value="10">saw 2:8 PW</option><option value="11">saw 1:9 PW</option><option value="12">pulse width</option><option value="13">square</option><option value="14">sine table</option><option value="15">analogue pulse</option><option value="16">analogue sync</option><option value="17">triangle saw blend</option><option value="18">digital nasty 1</option><option value="19">digital nasty 2</option><option value="20">digital saw square</option><option value="21">digital vocal 1</option><option value="22">digital vocal 2</option><option value="23">digital vocal 3</option><option value="24">digital vocal 4</option><option value="25">digital vocal 5</option><option value="26">digital vocal 6</option><option value="27">random collection 1</option><option value="28">random collection 2</option><option value="29">random collection 3</option></select></div>
</div>
<div class=slidercontainer>osc 1 wave interpolate:
<br><input class="slider" id="20" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id20 "></span><br>
 </div>
<div class=slidercontainer>
osc 1 pulse width index:<br><input class="slider" id="15" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id15"></span><br>
</div>

<div class=slidercontainer>
osc 1 virtual sync depth:<br><input class="slider" id="16" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id16"></span><br>
</div>

<div class=slidercontainer>
osc 1 density:<br><input class="slider" id="18" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id18"></span><br>
</div>

<div class=slidercontainer>
osc 1 density detune:<br><input class="slider" id="19" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id19"></span><br>
</div>

<div class=slidercontainer>
osc 1 semitones:<br><input class="slider" id="1a" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id1a"></span><br>
</div>

<div class=slidercontainer>
osc 1 cents:<br><input class="slider" id="1b" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id1b"></span><br>
</div>

<div class=slidercontainer>
osc 1 pitchbend:<br><input class="slider" id="1c" type="range" min="52" max="76" step="1" oninput="sendcc(this.id,this.value);" ><span id="id1c"></span><br>
</div>
<div class=slidercontainer>osc 2 wave: <select id="29"oninput="sendcc(this.id,this.value)" ><option value="0">sine</option><option value="1">triangle</option><option value="2">sawtooth</option><option value="3">saw 9:1 PW</option><option value="4">saw 8:2 PW</option><option value="5">saw 7:3 PW</option><option value="6">saw 6:4 PW</option><option value="7">saw 5:5 PW</option><option value="8">saw 4:6 PW</option><option value="9">saw 3:7 PW</option><option value="10">saw 2:8 PW</option><option value="11">saw 1:9 PW</option><option value="12">pulse width</option><option value="13">square</option><option value="14">sine table</option><option value="15">analogue pulse</option><option value="16">analogue sync</option><option value="17">triangle saw blend</option><option value="18">digital nasty 1</option><option value="19">digital nasty 2</option><option value="20">digital saw square</option><option value="21">digital vocal 1</option><option value="22">digital vocal 2</option><option value="23">digital vocal 3</option><option value="24">digital vocal 4</option><option value="25">digital vocal 5</option><option value="26">digital vocal 6</option><option value="27">random collection 1</option><option value="28">random collection 2</option><option value="29">random collection 3</option></select></div>
</div>
<div class=slidercontainer>
osc 2 wave interpolate:<br><input class="slider" id="1e" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id1e"></span><br>
</div>

<div class=slidercontainer>
osc 2 pulse width index:<br><input class="slider" id="1f" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id1f"></span><br>
</div>

<div class=slidercontainer>
osc 2 virtual sync depth:<br><input class="slider" id="21" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id21"></span><br>
</div>

<div class=slidercontainer>
osc 2 density:<br><input class="slider" id="23" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id23"></span><br>
</div>

<div class=slidercontainer>
osc 2 density detune:<br><input class="slider" id="24" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id24"></span><br>
</div>

<div class=slidercontainer>
osc 2 semitones:<br><input class="slider" id="25" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id25"></span><br>
</div>

<div class=slidercontainer>
osc 2 cents:<br><input class="slider" id="27" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id27"></span><br>
</div>

<div class=slidercontainer>
osc 2 pitchbend:<br><input class="slider" id="28" type="range" min="52" max="76" step="1" oninput="sendcc(this.id,this.value);" ><span id="id28"></span><br>
</div>

<div class=slidercontainer>
osc 1 level:<br><input class="slider" id="33" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id33"></span><br>
</div>

<div class=slidercontainer>
osc 2 level:<br><input class="slider" id="34" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id34"></span><br>
</div>

<div class=slidercontainer>
ring mod level:<br><input class="slider" id="36" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id36"></span><br>
</div>

<div class=slidercontainer>
noise level:<br><input class="slider" id="38" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id38"></span><br>
</div>

<div class=slidercontainer>
pre FX level:<br><input class="slider" id="3a" type="range" min="52" max="82" step="1" oninput="sendcc(this.id,this.value);" ><span id="id3a"></span><br>
</div>

<div class=slidercontainer>
post FX level:<br><input class="slider" id="3b" type="range" min="52" max="82" step="1" oninput="sendcc(this.id,this.value);" ><span id="id3b"></span><br>
</div>
<div class=slidercontainer>
routing: <select id="60"oninput="sendcc(this.id,this.value)" ><option value="0">sine</option><option value="1">triangle</option><option value="2">sawtooth</option><option value="3">saw 9:1 PW</option><option value="4">saw 8:2 PW</option><option value="5">saw 7:3 PW</option><option value="6">saw 6:4 PW</option><option value="7">saw 5:5 PW</option><option value="8">saw 4:6 PW</option><option value="9">saw 3:7 PW</option><option value="10">saw 2:8 PW</option><option value="11">saw 1:9 PW</option><option value="12">pulse width</option><option value="13">square</option><option value="14">sine table</option><option value="15">analogue pulse</option><option value="16">analogue sync</option><option value="17">triangle saw blend</option><option value="18">digital nasty 1</option><option value="19">digital nasty 2</option><option value="20">digital saw square</option><option value="21">digital vocal 1</option><option value="22">digital vocal 2</option><option value="23">digital vocal 3</option><option value="24">digital vocal 4</option><option value="25">digital vocal 5</option><option value="26">digital vocal 6</option><option value="27">random collection 1</option><option value="28">random collection 2</option><option value="29">random collection 3</option></select></div>
</div>
<div class=slidercontainer>
drive:<br><input class="slider" id="3f" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id3f"></span><br>
</div>
<div class=slidercontainer>
drive type: <select id="65"oninput="sendcc(this.id,this.value)" ><option value="0">diode</option><option value="1">valve</option><option value="2">clipper</option><option value="3">crossover</option><option value="4">rectifier</option><option value="5">bit reducer</option><option value="6">rate reducer</option></select></div>
</div>
<div class=slidercontainer>
filter type: <select id="68"oninput="sendcc(this.id,this.value)" ><option value="0">low pass 12 db</option><option value="1">low pass 24 db</option><option value="2">band pass 6 db</option><option value="3">band pass 12 db</option><option value="4">High pass 12 db</option><option value="5">High pass 24 db</option></select></div>
</div>
<div class=slidercontainer>
frequency:<br><input class="slider" id="4a" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id4a"></span><br>
</div>

<div class=slidercontainer>
tracking:<br><input class="slider" id="45" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id45"></span><br>
</div>

<div class=slidercontainer>
resonance:<br><input class="slider" id="47" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id47"></span><br>
</div>

<div class=slidercontainer>
Q normalize:<br><input class="slider" id="4e" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id4e"></span><br>
</div>

<div class=slidercontainer>
env 2 to frequency:<br><input class="slider" id="4f" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id4f"></span><br>
</div>

<div class=slidercontainer>
env 1 velocity:<br><input class="slider" id="6c" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id6c"></span><br>
</div>

<div class=slidercontainer>
env 1 attack:<br><input class="slider" id="49" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id49"></span><br>
</div>

<div class=slidercontainer>
env 1 decay:<br><input class="slider" id="4b" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id4b"></span><br>
</div>

<div class=slidercontainer>
env 1 sustain:<br><input class="slider" id="46" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id46"></span><br>
</div>

<div class=slidercontainer>
env 1 release:<br><input class="slider" id="48" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id48"></span><br>
</div>

<div class=slidercontainer>
distortion level:<br><input class="slider" id="5b" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id5b"></span><br>
</div>

<div class=slidercontainer>
chorus level:<br><input class="slider" id="5d" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id5d"></span><br>
</div>

<div class=slidercontainer>
macro knob 1 position:<br><input class="slider" id="50" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id50"></span><br>
</div>

<div class=slidercontainer>
macro knob 2 position:<br><input class="slider" id="51" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id51"></span><br>
</div>

<div class=slidercontainer>
macro knob 3 position:<br><input class="slider" id="52" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id52"></span><br>
</div>

<div class=slidercontainer>
macro knob 4 position:<br><input class="slider" id="53" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id53"></span><br>
</div>

<div class=slidercontainer>
macro knob 5 position:<br><input class="slider" id="54" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id54"></span><br>
</div>

<div class=slidercontainer>
macro knob 6 position:<br><input class="slider" id="55" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id55"></span><br>
</div>

<div class=slidercontainer>
macro knob 7 position:<br><input class="slider" id="56" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id56"></span><br>
</div>

<div class=slidercontainer>
macro knob 8 position:<br><input class="slider" id="57" type="range" min="0" max="127" step="1" oninput="sendcc(this.id,this.value);" ><span id="id57"></span><br>
</div>
<br><h2>NRPN:</h2><br>

<div class=slidercontainer>
env 2 velocity:<br><input class="slider" id="0000" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0000"></span><br>
</div>
<div class=slidercontainer>
env 2 attack:<br><input class="slider" id="0001" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0001"></span><br>
</div>
<div class=slidercontainer>
env 2 decay:<br><input class="slider" id="0002" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0002"></span><br>
</div>
<div class=slidercontainer>
env 2 sustain:<br><input class="slider" id="0003" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0003"></span><br>
</div>
<div class=slidercontainer>
env 2 release:<br><input class="slider" id="0004" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0004"></span><br>
</div>
<div class=slidercontainer>
env 3 delay:<br><input class="slider" id="000e" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id000e"></span><br>
</div>
<div class=slidercontainer>
env 3 attack:<br><input class="slider" id="000f" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id000f"></span><br>
</div>
<div class=slidercontainer>
env 3 decay:<br><input class="slider" id="0010" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0010"></span><br>
</div>
<div class=slidercontainer>
env 3 sustain:<br><input class="slider" id="0011" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0011"></span><br>
</div>
<div class=slidercontainer>
env 3 release:<br><input class="slider" id="0012" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0012"></span><br>
</div>
<div class=slidercontainer>
lfo 1 waveform: <select id="0046" >
<option value="0">sine</option>
<option value="1">triangle</option>
<option value="2">sawtooth</option>
<option value="3">square</option>
<option value="4">random S/H</option>
<option value="5">time S/H</option>
<option value="6">piano envelope</option>
<option value="7">sequence 1</option>
<option value="8">sequence 2</option>
<option value="9">sequence 3</option>
<option value="10">sequence 4</option>
<option value="11">sequence 5</option>
<option value="12">sequence 6</option>
<option value="13">sequence 7</option>
<option value="14">alternative 1</option>
<option value="15">alternative 2</option>
<option value="16">alternative 3</option>
<option value="17">alternative 4</option>
<option value="18">alternative 5</option>
<option value="19">alternative 6</option>
<option value="20">alternative 7</option>
<option value="21">alternative 8</option>
<option value="22">chromatic</option>
<option value="23">chromatic 16</option>
<option value="24">major</option>
<option value="25">major 7</option>
<option value="26">minor 7</option>
<option value="27">min arp 1</option>
<option value="28">min arp 2</option>
<option value="29">diminished</option>
<option value="30">dec minor</option>
<option value="31">minor 3rd</option>
<option value="32">pedal</option>
<option value="33">4ths</option>
<option value="34">4ths x12</option>
<option value="35">1625 maj</option>
<option value="36">1625 Min</option>
<option value="37">251</option>
<option value="0">osc 1 pitch</option>
</select></div>
<div class=slidercontainer>
lfo 1 phase offset:<br><input class="slider" id="0047" type="range" min="0" max="119" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0047"></span><br>
</div>
<div class=slidercontainer>
lfo 1 slew rate:<br><input class="slider" id="0048" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0048"></span><br>
</div>
<div class=slidercontainer>
lfo 1 delay:<br><input class="slider" id="004a" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id004a"></span><br>
</div>
<div class=slidercontainer>
lfo 1 delay sync:<br><input class="slider" id="004b" type="range" min="0" max="35" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id004b"></span><br>
</div>
<div class=slidercontainer>
lfo 1 rate:<br><input class="slider" id="004c" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id004c"></span><br>
</div>
<div class=slidercontainer>
lfo 1 rate sync:<br><input class="slider" id="004d" type="range" min="0" max="35" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id004d"></span><br>
</div>
<div class=slidercontainer>
lfo 1 one shot:<br><input class="slider" id="007a" type="range" min="12" max="13" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id007a"></span><br>
</div>
<div class=slidercontainer>
lfo 1 key sync:<br><input class="slider" id="007a" type="range" min="14" max="15" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id007a"></span><br>
</div>
<div class=slidercontainer>
lfo 1 common sync:<br><input class="slider" id="007a" type="range" min="16" max="17" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id007a"></span><br>
</div>
<div class=slidercontainer>
lfo 1 delay trigger:<br><input class="slider" id="007a" type="range" min="18" max="19" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id007a"></span><br>
</div>
<div class=slidercontainer>
lfo 1 fade mode:<br><input class="slider" id="007b" type="range" min="0" max="3" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id007b"></span><br>
</div>
<div class=slidercontainer>
lfo 2 waveform: <select id="004f" >
<option value="0">sine</option>
<option value="1">triangle</option>
<option value="2">sawtooth</option>
<option value="3">square</option>
<option value="4">random S/H</option>
<option value="5">time S/H</option>
<option value="6">piano envelope</option>
<option value="7">sequence 1</option>
<option value="8">sequence 2</option>
<option value="9">sequence 3</option>
<option value="10">sequence 4</option>
<option value="11">sequence 5</option>
<option value="12">sequence 6</option>
<option value="13">sequence 7</option>
<option value="14">alternative 1</option>
<option value="15">alternative 2</option>
<option value="16">alternative 3</option>
<option value="17">alternative 4</option>
<option value="18">alternative 5</option>
<option value="19">alternative 6</option>
<option value="20">alternative 7</option>
<option value="21">alternative 8</option>
<option value="22">chromatic</option>
<option value="23">chromatic 16</option>
<option value="24">major</option>
<option value="25">major 7</option>
<option value="26">minor 7</option>
<option value="27">min arp 1</option>
<option value="28">min arp 2</option>
<option value="29">diminished</option>
<option value="30">dec minor</option>
<option value="31">minor 3rd</option>
<option value="32">pedal</option>
<option value="33">4ths</option>
<option value="34">4ths x12</option>
<option value="35">1625 maj</option>
<option value="36">1625 Min</option>
<option value="37">251</option>
<option value="0">osc 1 pitch</option>
</select></div>
<div class=slidercontainer>
lfo 2 phase offset:<br><input class="slider" id="0050" type="range" min="0" max="119" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0050"></span><br>
</div>
<div class=slidercontainer>
lfo 2 slew rate:<br><input class="slider" id="0051" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0051"></span><br>
</div>
<div class=slidercontainer>
lfo 2 delay:<br><input class="slider" id="0053" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0053"></span><br>
</div>
<div class=slidercontainer>
lfo 2 delay sync:<br><input class="slider" id="0054" type="range" min="0" max="35" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0054"></span><br>
</div>
<div class=slidercontainer>
lfo 2 rate:<br><input class="slider" id="0055" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0055"></span><br>
</div>
<div class=slidercontainer>
lfo 2 rate sync:<br><input class="slider" id="0056" type="range" min="0" max="35" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0056"></span><br>
</div>
<div class=slidercontainer>
lfo 2 one shot:<br><input class="slider" id="007a" type="range" min="22" max="23" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id007a"></span><br>
</div>
<div class=slidercontainer>
lfo 2 key sync:<br><input class="slider" id="007a" type="range" min="24" max="25" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id007a"></span><br>
</div>
<div class=slidercontainer>
lfo 2 common sync:<br><input class="slider" id="007a" type="range" min="26" max="27" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id007a"></span><br>
</div>
<div class=slidercontainer>
lfo 2 delay trigger:<br><input class="slider" id="007a" type="range" min="28" max="29" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id007a"></span><br>
</div>
<div class=slidercontainer>
lfo 2 fade mode:<br><input class="slider" id="007b" type="range" min="4" max="7" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id007b"></span><br>
</div>
<div class=slidercontainer>
EQ bass frequency:<br><input class="slider" id="0068" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0068"></span><br>
</div>
<div class=slidercontainer>
EQ bass level:<br><input class="slider" id="0069" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0069"></span><br>
</div>
<div class=slidercontainer>
EQ mid frequency:<br><input class="slider" id="006a" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id006a"></span><br>
</div>
<div class=slidercontainer>
EQ mid level:<br><input class="slider" id="006b" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id006b"></span><br>
</div>
<div class=slidercontainer>
EQ treble frequency:<br><input class="slider" id="006c" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id006c"></span><br>
</div>
<div class=slidercontainer>
EQ treble level:<br><input class="slider" id="006d" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id006d"></span><br>
</div>
<div class=slidercontainer>
distortion type:<br><input class="slider" id="0100" type="range" min="0" max="6" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0100"></span><br>
</div>
<div class=slidercontainer>
distortion compensation:<br><input class="slider" id="0101" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0101"></span><br>
</div>
<div class=slidercontainer>
chorus type:<br><input class="slider" id="0118" type="range" min="0" max="1" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0118"></span><br>
</div>
<div class=slidercontainer>
chorus rate:<br><input class="slider" id="0119" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0119"></span><br>
</div>
<div class=slidercontainer>
chorus rate sync:<br><input class="slider" id="011a" type="range" min="0" max="35" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id011a"></span><br>
</div>
<div class=slidercontainer>
chorus feedback:<br><input class="slider" id="011b" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id011b"></span><br>
</div>
<div class=slidercontainer>
chorus mod depth:<br><input class="slider" id="011c" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id011c"></span><br>
</div>
<div class=slidercontainer>
chorus delay:<br><input class="slider" id="011d" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id011d"></span><br>
</div></div>
</body></html>
)rawliteral";

const char * assign PROGMEM = R"rawliteral(
<html><head><meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body {
  background-image: linear-gradient(to bottom right,  #101010, #606060);
  color: powderblue;
}
select {
  font-size:14px;
  border-style: ridge;
  padding: 2px;
  border-radius: 5px;
   margin-top: 1%; 
  }
.slidercontainer{
   margin-top: 1%; 
 margin-bottom: 1%;
}

.slider {
  -webkit-appearance: none;
  height: 10px;
  background: Black;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s;
  border-style: inset;
  border-color: DarkGray;
 margin-top: 1%; 
 margin-bottom: 1%;
 border-radius: 6px;

}

.slider:hover {
  opacity: 1;
  background:#00ffca
}
.slider:checked {
  opacity: 1;
  background:#ca00ff
}


.slider::-webkit-slider-thumb {
   border-style: outset;
  border-style: outset;
  border-color: DarkGray;
  -webkit-appearance: none;
  appearance: none;
  width: 4px;
  height: 16px;
  background: #ffca00;
  cursor: pointer;
   border-radius: 6px;
}

.slider::-moz-range-thumb {
  border-style: outset;
  width: 4px;
  height: 16px;
  background: #ffca00;
  cursor: pointer;
  border-radius: 6px;
}
</style><script>var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);connection.onopen = function () {  connection.send('Connect ' + new Date()); }; connection.onerror = function (error) {    console.log('WebSocket Error ', error);};connection.onmessage = function (e) {  console.log('Server: ', e.data);};
function sendcc(cc,val) { hexV = parseInt(val).toString(16); 
hexC = cc.toString(); 
if(hexV.length < 2) { hexV = '0' + hexV; }  
if(hexC.length < 2) { hexC = '0' + hexC; }  
var command = '#'+hexC+hexV; connection.send(command); var output = document.getElementById("id"+cc); output.innerHTML = val; }; 
function sendNPRN(cc,val) {hexV = parseInt(val).toString(16); 
hexC = cc.toString(); 
if(hexV.length < 2) { hexV = '0' + hexV; }  
while (hexC.length < 4) { hexC = '0' + hexC; }  
var command = 'N'+hexC+hexV.toString(16); connection.send(command); var output = document.getElementById("id"+cc); output.innerHTML = val; }; 
</script></head>
<body>
<div class=slidercontainer><a href="/assign">Asignar Knobs</a></div>
<div class=slidercontainer><a href="/modmatrix">Modulation matrix</a></div>
<div class=slidercontainer><a href="/drums">Drums</a></div>
<div class=slidercontainer><a href="/">CC & NRPN</a></div> 
<div class=slidercontainer><a href="/global">Global</a></div> 
<div class=slidercontainer><select id="Knob">
<option value="0300"> Knob 1 destination A:</option>
<option value="0304"> Knob 1 destination B:</option>
<option value="0308"> Knob 1 destination C:</option>
<option value="030C"> Knob 1 destination D:</option>
<option value="0310"> Knob 2 destination A:</option>
<option value="0314"> Knob 2 destination B:</option>
<option value="0318"> Knob 2 destination C:</option>
<option value="031C"> Knob 2 destination D:</option>
<option value="0320"> Knob 3 destination A:</option>
<option value="0324"> Knob 3 destination B:</option>
<option value="0328"> Knob 3 destination C:</option>
<option value="032C"> Knob 3 destination D:</option>
<option value="0330"> Knob 4 destination A:</option>
<option value="0334"> Knob 4 destination B:</option>
<option value="0338"> Knob 4 destination C:</option>
<option value="033C"> Knob 4 destination D:</option>
<option value="0340"> Knob 5 destination A:</option>
<option value="0344"> Knob 5 destination B:</option>
<option value="0348"> Knob 5 destination C:</option>
<option value="034C"> Knob 5 destination D:</option>
<option value="0350"> Knob 6 destination A:</option>
<option value="0354"> Knob 6 destination B:</option>
<option value="0358"> Knob 6 destination C:</option>
<option value="035C"> Knob 6 destination D:</option>
<option value="0360"> Knob 7 destination A:</option>
<option value="0364"> Knob 7 destination B:</option>
<option value="0368"> Knob 7 destination C:</option>
<option value="036C"> Knob 7 destination D:</option>
<option value="0370"> Knob 8 destination A:</option>
<option value="0374"> Knob 8 destination B:</option>
<option value="0378"> Knob 8 destination C:</option>
<option value="037C"> Knob 8 destination D:</option>
</select></div>
<div class=slidercontainer>
<select id="assign" oninput="sendNPRN(document.getElementById('Knob').value,this.value);" >
<option value="0">osc 1 wave interpolate</option>
<option value="1">osc 1 pulse width index</option>
<option value="2">osc 1 virtual sync depth</option>
<option value="3">osc 1 density</option>
<option value="4">osc 1 density detune</option>
<option value="5">osc 1 semitones</option>
<option value="6">osc 1 cents</option>
<option value="7">osc 2 wave interpolate</option>
<option value="8">osc 2 pulse width index</option>
<option value="9">osc 2 virtual sync depth</option>
<option value="10">osc 2 density</option>
<option value="11">osc 2 density detune</option>
<option value="12">osc 2 semitones</option>
<option value="13">osc 2 cents</option>
<option value="14">Post FX level</option>
<option value="15">osc 1 level</option>
<option value="16">osc 2 level</option>
<option value="17">ring mod level</option>
<option value="18">noise level</option>
<option value="19">frequency</option>
<option value="20">resonance</option>
<option value="21">drive</option>
<option value="22">tracking</option>
<option value="23">env 2 to frequency</option>
<option value="24">env 1 attack</option>
<option value="25">env 1 decay</option>
<option value="26">env 1 sustain</option>
<option value="27">env 1 release</option>
<option value="28">env 2 attack</option>
<option value="29">env 2 decay</option>
<option value="30">env 2 sustain</option>
<option value="31">env 2 release</option>
<option value="32">env 3 delay</option>
<option value="33">env 3 attack</option>
<option value="34">env 3 decay</option>
<option value="35">env 3 sustain</option>
<option value="36">env 3 release</option>
<option value="37">lfo 1 rate</option>
<option value="38">lfo 1 delay</option>
<option value="39">lfo 1 slew rate</option>
<option value="40">lfo 2 rate</option>
<option value="41">lfo 2 delay</option>
<option value="42">lfo 2 slew rate</option>
<option value="43">distortion level</option>
<option value="44">chorus level</option>
<option value="45">chorus rate</option>
<option value="46">chorus feedback</option>
<option value="47">chorus mod depth</option>
<option value="48">chorus delay</option>
<option value="49">Portamento Rate</option>
<option value="50">mod matrix 1 depth</option>
<option value="51">mod matrix 2 depth</option>
<option value="52">mod matrix 3 depth</option>
<option value="53">mod matrix 4 depth</option>
<option value="54">mod matrix 5 depth</option>
<option value="55">mod matrix 6 depth</option>
<option value="56">mod matrix 7 depth</option>
<option value="57">mod matrix 8 depth</option>
<option value="58">mod matrix 9 depth</option>
<option value="59">mod matrix 10 depth</option>
<option value="60">mod matrix 11 depth</option>
<option value="61">mod matrix 12 depth</option>
<option value="62">mod matrix 13 depth</option>
<option value="63">mod matrix 14 depth</option>
<option value="64">mod matrix 15 depth</option>
<option value="65">mod matrix 16 depth</option>
<option value="66">mod matrix 17 depth</option>
<option value="67">mod matrix 18 depth</option>
<option value="68">mod matrix 19 depth</option>
<option value="69">mod matrix 20 depth</option>
</select></div>
<div class=slidercontainer>
macro knob 1 start position A:<br><input class="slider" id="0301" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0301"></span><br>
</div>
<div class=slidercontainer>
macro knob 1 end position A:<br><input class="slider" id="0302" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0302"></span><br>
</div>
<div class=slidercontainer>
macro knob 1 depth A:<br><input class="slider" id="0303" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0303"></span><br>
</div>

<div class=slidercontainer>
macro knob 1 start position B:<br><input class="slider" id="0305" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0305"></span><br>
</div>
<div class=slidercontainer>
macro knob 1 end position B:<br><input class="slider" id="0306" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0306"></span><br>
</div>
<div class=slidercontainer>
macro knob 1 depth B:<br><input class="slider" id="0307" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0307"></span><br>
</div>

<div class=slidercontainer>
macro knob 1 start position C:<br><input class="slider" id="0309" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0309"></span><br>
</div>
<div class=slidercontainer>
macro knob 1 end position C:<br><input class="slider" id="030a" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id030a"></span><br>
</div>
<div class=slidercontainer>
macro knob 1 depth C:<br><input class="slider" id="030b" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id030b"></span><br>
</div>

<div class=slidercontainer>
macro knob 1 start position D:<br><input class="slider" id="030d" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id030d"></span><br>
</div>
<div class=slidercontainer>
macro knob 1 end position D:<br><input class="slider" id="030e" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id030e"></span><br>
</div>
<div class=slidercontainer>
macro knob 1 depth D:<br><input class="slider" id="030f" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id030f"></span><br>
</div>

<div class=slidercontainer>
macro knob 2 start position A:<br><input class="slider" id="0311" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0311"></span><br>
</div>
<div class=slidercontainer>
macro knob 2 end position A:<br><input class="slider" id="0312" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0312"></span><br>
</div>
<div class=slidercontainer>
macro knob 2 depth A:<br><input class="slider" id="0313" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0313"></span><br>
</div>
<div class=slidercontainer>
macro knob 2 start position B:<br><input class="slider" id="0315" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0315"></span><br>
</div>
<div class=slidercontainer>
macro knob 2 end position B:<br><input class="slider" id="0316" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0316"></span><br>
</div>
<div class=slidercontainer>
macro knob 2 depth B:<br><input class="slider" id="0317" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0317"></span><br>
</div>

<div class=slidercontainer>
macro knob 2 start position C:<br><input class="slider" id="0319" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0319"></span><br>
</div>
<div class=slidercontainer>
macro knob 2 end position C:<br><input class="slider" id="031a" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id031a"></span><br>
</div>
<div class=slidercontainer>
macro knob 2 depth C:<br><input class="slider" id="031b" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id031b"></span><br>
</div>

<div class=slidercontainer>
macro knob 2 start position D:<br><input class="slider" id="031d" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id031d"></span><br>
</div>
<div class=slidercontainer>
macro knob 2 end position D:<br><input class="slider" id="031e" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id031e"></span><br>
</div>
<div class=slidercontainer>
macro knob 2 depth D:<br><input class="slider" id="031f" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id031f"></span><br>
</div>

<div class=slidercontainer>
macro knob 3 start position A:<br><input class="slider" id="0321" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0321"></span><br>
</div>
<div class=slidercontainer>
macro knob 3 end position A:<br><input class="slider" id="0322" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0322"></span><br>
</div>
<div class=slidercontainer>
macro knob 3 depth A:<br><input class="slider" id="0323" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0323"></span><br>
</div>

<div class=slidercontainer>
macro knob 3 start position B:<br><input class="slider" id="0325" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0325"></span><br>
</div>
<div class=slidercontainer>
macro knob 3 end position B:<br><input class="slider" id="0326" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0326"></span><br>
</div>
<div class=slidercontainer>
macro knob 3 depth B:<br><input class="slider" id="0327" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0327"></span><br>
</div>

<div class=slidercontainer>
macro knob 3 start position C:<br><input class="slider" id="0329" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0329"></span><br>
</div>
<div class=slidercontainer>
macro knob 3 end position C:<br><input class="slider" id="032a" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id032a"></span><br>
</div>
<div class=slidercontainer>
macro knob 3 depth C:<br><input class="slider" id="032b" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id032b"></span><br>
</div>

<div class=slidercontainer>
macro knob 3 start position D:<br><input class="slider" id="032d" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id032d"></span><br>
</div>
<div class=slidercontainer>
macro knob 3 end position D:<br><input class="slider" id="032e" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id032e"></span><br>
</div>
<div class=slidercontainer>
macro knob 3 depth D:<br><input class="slider" id="032f" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id032f"></span><br>
</div>

<div class=slidercontainer>
macro knob 4 start position A:<br><input class="slider" id="0331" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0331"></span><br>
</div>
<div class=slidercontainer>
macro knob 4 end position A:<br><input class="slider" id="0332" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0332"></span><br>
</div>
<div class=slidercontainer>
macro knob 4 depth A:<br><input class="slider" id="0333" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0333"></span><br>

<div class=slidercontainer>
macro knob 4 start position B:<br><input class="slider" id="0335" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0335"></span><br>
</div>
<div class=slidercontainer>
macro knob 4 end position B:<br><input class="slider" id="0336" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0336"></span><br>
</div>
<div class=slidercontainer>
macro knob 4 depth B:<br><input class="slider" id="0337" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0337"></span><br>
</div>

<div class=slidercontainer>
macro knob 4 start position C:<br><input class="slider" id="0339" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0339"></span><br>
</div>
<div class=slidercontainer>
macro knob 4 end position C:<br><input class="slider" id="033a" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id033a"></span><br>
</div>
<div class=slidercontainer>
macro knob 4 depth C:<br><input class="slider" id="033b" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id033b"></span><br>
</div>

<div class=slidercontainer>
macro knob 4 start position D:<br><input class="slider" id="033d" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id033d"></span><br>
</div>
<div class=slidercontainer>
macro knob 4 end position D:<br><input class="slider" id="033e" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id033e"></span><br>
</div>
<div class=slidercontainer>
macro knob 4 depth D:<br><input class="slider" id="033f" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id033f"></span><br>
</div>

<div class=slidercontainer>
macro knob 5 start position A:<br><input class="slider" id="0341" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0341"></span><br>
</div>
<div class=slidercontainer>
macro knob 5 end position A:<br><input class="slider" id="0342" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0342"></span><br>
</div>
<div class=slidercontainer>
macro knob 5 depth A:<br><input class="slider" id="0343" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0343"></span><br>
</div>

<div class=slidercontainer>
macro knob 5 start position B:<br><input class="slider" id="0345" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0345"></span><br>
</div>
<div class=slidercontainer>
macro knob 5 end position B:<br><input class="slider" id="0346" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0346"></span><br>
</div>
<div class=slidercontainer>
macro knob 5 depth B:<br><input class="slider" id="0347" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0347"></span><br>
</div>
<div class=slidercontainer>
macro knob 5 destination C:<br><input class="slider" id="0348" type="range" min="0" max="70" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0348"></span><br>
</div>

<div class=slidercontainer>
macro knob 5 end position C:<br><input class="slider" id="034a" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id034a"></span><br>
</div>
<div class=slidercontainer>
macro knob 5 depth C:<br><input class="slider" id="034b" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id034b"></span><br>
</div>

<div class=slidercontainer>
macro knob 5 start position D:<br><input class="slider" id="034d" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id034d"></span><br>
</div>
<div class=slidercontainer>
macro knob 5 end position D:<br><input class="slider" id="034e" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id034e"></span><br>
</div>
<div class=slidercontainer>
macro knob 5 depth D:<br><input class="slider" id="034f" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id034f"></span><br>
</div>

<div class=slidercontainer>
macro knob 6 start position A:<br><input class="slider" id="0351" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0351"></span><br>
</div>
<div class=slidercontainer>
macro knob 6 end position A:<br><input class="slider" id="0352" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0352"></span><br>
</div>
<div class=slidercontainer>
macro knob 6 depth A:<br><input class="slider" id="0353" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0353"></span><br>
</div>

<div class=slidercontainer>
macro knob 6 start position B:<br><input class="slider" id="0355" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0355"></span><br>
</div>
<div class=slidercontainer>
macro knob 6 end position B:<br><input class="slider" id="0356" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0356"></span><br>
</div>
<div class=slidercontainer>
macro knob 6 depth B:<br><input class="slider" id="0357" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0357"></span><br>
</div>

<div class=slidercontainer>
macro knob 6 start position C:<br><input class="slider" id="0359" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0359"></span><br>
</div>
<div class=slidercontainer>
macro knob 6 end position C:<br><input class="slider" id="035a" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id035a"></span><br>
</div>
<div class=slidercontainer>
macro knob 6 depth C:<br><input class="slider" id="035b" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id035b"></span><br>
</div>

<div class=slidercontainer>
macro knob 6 start position D:<br><input class="slider" id="035d" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id035d"></span><br>
</div>
<div class=slidercontainer>
macro knob 6 end position D:<br><input class="slider" id="035e" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id035e"></span><br>
</div>
<div class=slidercontainer>
macro knob 6 depth D:<br><input class="slider" id="035f" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id035f"></span><br>
</div>

<div class=slidercontainer>
macro knob 7 start position A:<br><input class="slider" id="0361" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0361"></span><br>
</div>
<div class=slidercontainer>
macro knob 7 end position A:<br><input class="slider" id="0362" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0362"></span><br>
</div>
<div class=slidercontainer>
macro knob 7 depth A:<br><input class="slider" id="0363" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0363"></span><br>
</div>

<div class=slidercontainer>
macro knob 7 start position B:<br><input class="slider" id="0365" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0365"></span><br>
</div>
<div class=slidercontainer>
macro knob 7 end position B:<br><input class="slider" id="0366" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0366"></span><br>
</div>
<div class=slidercontainer>
macro knob 7 depth B:<br><input class="slider" id="0367" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0367"></span><br>
</div>

<div class=slidercontainer>
macro knob 7 start position C:<br><input class="slider" id="0369" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0369"></span><br>
</div>
<div class=slidercontainer>
macro knob 7 end position C:<br><input class="slider" id="036a" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id036a"></span><br>
</div>
<div class=slidercontainer>
macro knob 7 depth C:<br><input class="slider" id="036b" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id036b"></span><br>
</div>

<div class=slidercontainer>
macro knob 7 start position D:<br><input class="slider" id="036d" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id036d"></span><br>
</div>
<div class=slidercontainer>
macro knob 7 end position D:<br><input class="slider" id="036e" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id036e"></span><br>
</div>
<div class=slidercontainer>
macro knob 7 depth D:<br><input class="slider" id="036f" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id036f"></span><br>
</div>

<div class=slidercontainer>
macro knob 8 start position A:<br><input class="slider" id="0371" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0371"></span><br>
</div>
<div class=slidercontainer>
macro knob 8 end position A:<br><input class="slider" id="0372" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0372"></span><br>
</div>
<div class=slidercontainer>
macro knob 8 depth A:<br><input class="slider" id="0373" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0373"></span><br>
</div>

<div class=slidercontainer>
macro knob 8 start position B:<br><input class="slider" id="0375" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0375"></span><br>
</div>
<div class=slidercontainer>
macro knob 8 end position B:<br><input class="slider" id="0376" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0376"></span><br>
</div>
<div class=slidercontainer>
macro knob 8 depth B:<br><input class="slider" id="0377" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0377"></span><br>
</div>

<div class=slidercontainer>
macro knob 8 start position C:<br><input class="slider" id="0379" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0379"></span><br>
</div>
<div class=slidercontainer>
macro knob 8 end position C:<br><input class="slider" id="037a" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id037a"></span><br>
</div>
<div class=slidercontainer>
macro knob 8 depth C:<br><input class="slider" id="037b" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id037b"></span><br>
</div>
<div class=slidercontainer>
macro knob 8 start position D:<br><input class="slider" id="037d" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id037d"></span><br>
</div>
<div class=slidercontainer>
macro knob 8 end position D:<br><input class="slider" id="037e" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id037e"></span><br>
</div>
<div class=slidercontainer>
macro knob 8 depth D:<br><input class="slider" id="037f" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id037f"></span><br>
</div></body></html>)rawliteral";
const char modmatrix[] PROGMEM = R"rawliteral(<head>
<style>
body {
  background-image: linear-gradient(to bottom right,  #101010, #606060);
  color: powderblue;
}
select {
  font-size:14px;
  border-style: ridge;
  padding: 2px;
  border-radius: 5px;
   margin-top: 1%; 
  }
.slidercontainer{
   margin-top: 1%; 
 margin-bottom: 1%;
}

.slider {
  -webkit-appearance: none;
  height: 10px;
  background: Black;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s;
  border-style: inset;
  border-color: DarkGray;
 margin-top: 1%; 
 margin-bottom: 1%;
 border-radius: 6px;

}

.slider:hover {
  opacity: 1;
  background:#00ffca
}
.slider:checked {
  opacity: 1;
  background:#ca00ff
}


.slider::-webkit-slider-thumb {
   border-style: outset;
  border-style: outset;
  border-color: DarkGray;
  -webkit-appearance: none;
  appearance: none;
  width: 4px;
  height: 16px;
  background: #ffca00;
  cursor: pointer;
   border-radius: 6px;
}

.slider::-moz-range-thumb {
  border-style: outset;
  width: 4px;
  height: 16px;
  background: #ffca00;
  cursor: pointer;
  border-radius: 6px;
}
</style><script>var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);connection.onopen = function () {  connection.send('Connect ' + new Date()); }; connection.onerror = function (error) {    console.log('WebSocket Error ', error);};connection.onmessage = function (e) {  console.log('Server: ', e.data);};
function sendcc(cc,val) { hexV = parseInt(val).toString(16); 
hexC = cc.toString(); 
if(hexV.length < 2) { hexV = '0' + hexV; }  
if(hexC.length < 2) { hexC = '0' + hexC; }  
var command = '#'+hexC+hexV; connection.send(command); var output = document.getElementById("id"+cc); output.innerHTML = val; }; 
function sendNPRN(cc,val) {hexV = parseInt(val).toString(16); 
hexC = cc.toString(); 
if(hexV.length < 2) { hexV = '0' + hexV; }  
while (hexC.length < 4) { hexC = '0' + hexC; }  
var command = 'N'+hexC+hexV.toString(16); connection.send(command); var output = document.getElementById("id"+cc); output.innerHTML = val; }; 
</script></head>
<body>
<div class=slidercontainer><a href="/assign">Asignar Knobs</a></div>
<div class=slidercontainer><a href="/modmatrix">Modulation matrix</a></div>
<div class=slidercontainer><a href="/drums">Drums</a></div>
<div class=slidercontainer><a href="/">CC & NRPN</a></div> 
<div class=slidercontainer><a href="/global">Global</a></div> 
<div class=slidercontainer>
mod matrix 1 source 1:<select id="0153" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select></div>
<div class=slidercontainer>
mod matrix 1 source 2:<select id="0154" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select></div>
<div class=slidercontainer>
mod matrix 1 depth:<br><input class="slider" id="0156" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0156"></span><br>
</div>
<div class=slidercontainer>
mod matrix 1 destination:<select oninput="sendNPRN(this.id,this.value);" id="0157" >
<option value="0">osc 1 pitch</option>
<option value="1">osc 1 pitch</option>
<option value="2">osc 2 pitch</option>
<option value="3">osc 1 v-sync</option>
<option value="4">osc 2 v-sync</option>
<option value="5">osc 1 pulse width</option>
<option value="6">osc 2 pulse width</option>
<option value="7">osc 1 level</option>
<option value="8">osc 2 level</option>
<option value="9">noise level</option>
<option value="10">ring modulation 1*2 level</option>
<option value="11">drive amount</option>
<option value="12">frequency</option>
<option value="13">resonance</option>
<option value="14">LFO 1 rate</option>
<option value="15">LFO 2 rate</option>
<option value="16">amp envelope decay</option>
<option value="17">mod envelope decay</option></select></div>
<div class=slidercontainer>
mod matrix 2 source 1:<select id="0158" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select></div>
<div class=slidercontainer>
mod matrix 2 source 2:<select id="0159" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 2 depth:<br><input class="slider" id="015b" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id015b"></span><br>
</div>
<div class=slidercontainer>
mod matrix 2 destination:<select oninput="sendNPRN(this.id,this.value);" id="015c" >
<option value="0">osc 1 pitch</option>
<option value="1">osc 1 pitch</option>
<option value="2">osc 2 pitch</option>
<option value="3">osc 1 v-sync</option>
<option value="4">osc 2 v-sync</option>
<option value="5">osc 1 pulse width</option>
<option value="6">osc 2 pulse width</option>
<option value="7">osc 1 level</option>
<option value="8">osc 2 level</option>
<option value="9">noise level</option>
<option value="10">ring modulation 1*2 level</option>
<option value="11">drive amount</option>
<option value="12">frequency</option>
<option value="13">resonance</option>
<option value="14">LFO 1 rate</option>
<option value="15">LFO 2 rate</option>
<option value="16">amp envelope decay</option>
<option value="17">mod envelope decay</option></select></div>
<div class=slidercontainer>
mod matrix 3 source 1:<select id="015d" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 3 source 2:<select id="015e" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select></div>
<div class=slidercontainer>
mod matrix 3 depth:<br><input class="slider" id="0160" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0160"></span><br>
</div>
<div class=slidercontainer>
mod matrix 3 destination:<select oninput="sendNPRN(this.id,this.value);" id="0161" >
<option value="0">osc 1 pitch</option>
<option value="1">osc 1 pitch</option>
<option value="2">osc 2 pitch</option>
<option value="3">osc 1 v-sync</option>
<option value="4">osc 2 v-sync</option>
<option value="5">osc 1 pulse width</option>
<option value="6">osc 2 pulse width</option>
<option value="7">osc 1 level</option>
<option value="8">osc 2 level</option>
<option value="9">noise level</option>
<option value="10">ring modulation 1*2 level</option>
<option value="11">drive amount</option>
<option value="12">frequency</option>
<option value="13">resonance</option>
<option value="14">LFO 1 rate</option>
<option value="15">LFO 2 rate</option>
<option value="16">amp envelope decay</option>
<option value="17">mod envelope decay</option></select></div>
<div class=slidercontainer>
mod matrix 4 source 1:<select id="0162" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 4 source 2:<select id="0163" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 4 depth:<br><input class="slider" id="0165" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0165"></span><br>
</div>
<div class=slidercontainer>
mod matrix 4 destination:<select oninput="sendNPRN(this.id,this.value);" id="0166" >
<option value="0">osc 1 pitch</option>
<option value="1">osc 1 pitch</option>
<option value="2">osc 2 pitch</option>
<option value="3">osc 1 v-sync</option>
<option value="4">osc 2 v-sync</option>
<option value="5">osc 1 pulse width</option>
<option value="6">osc 2 pulse width</option>
<option value="7">osc 1 level</option>
<option value="8">osc 2 level</option>
<option value="9">noise level</option>
<option value="10">ring modulation 1*2 level</option>
<option value="11">drive amount</option>
<option value="12">frequency</option>
<option value="13">resonance</option>
<option value="14">LFO 1 rate</option>
<option value="15">LFO 2 rate</option>
<option value="16">amp envelope decay</option>
<option value="17">mod envelope decay</option></select></div>
<div class=slidercontainer>
mod matrix 5 source 1:<select id="0164" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 5 source 2:<select id="0168" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 5 depth:<br><input class="slider" id="016a" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id016a"></span><br>
</div>
<div class=slidercontainer>
mod matrix 5 destination:<select oninput="sendNPRN(this.id,this.value);" id="016b" >
<option value="0">osc 1 pitch</option>
<option value="1">osc 1 pitch</option>
<option value="2">osc 2 pitch</option>
<option value="3">osc 1 v-sync</option>
<option value="4">osc 2 v-sync</option>
<option value="5">osc 1 pulse width</option>
<option value="6">osc 2 pulse width</option>
<option value="7">osc 1 level</option>
<option value="8">osc 2 level</option>
<option value="9">noise level</option>
<option value="10">ring modulation 1*2 level</option>
<option value="11">drive amount</option>
<option value="12">frequency</option>
<option value="13">resonance</option>
<option value="14">LFO 1 rate</option>
<option value="15">LFO 2 rate</option>
<option value="16">amp envelope decay</option>
<option value="17">mod envelope decay</option></select></div>
<div class=slidercontainer>
mod matrix 6 source 1:<select id="016c" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select></div>
<div class=slidercontainer>
mod matrix 6 source 2:<select id="016d" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 6 depth:<br><input class="slider" id="016f" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id016f"></span><br>
</div>
<div class=slidercontainer>
mod matrix 6 destination:<select oninput="sendNPRN(this.id,this.value);" id="0170" >
<option value="0">osc 1 pitch</option>
<option value="1">osc 1 pitch</option>
<option value="2">osc 2 pitch</option>
<option value="3">osc 1 v-sync</option>
<option value="4">osc 2 v-sync</option>
<option value="5">osc 1 pulse width</option>
<option value="6">osc 2 pulse width</option>
<option value="7">osc 1 level</option>
<option value="8">osc 2 level</option>
<option value="9">noise level</option>
<option value="10">ring modulation 1*2 level</option>
<option value="11">drive amount</option>
<option value="12">frequency</option>
<option value="13">resonance</option>
<option value="14">LFO 1 rate</option>
<option value="15">LFO 2 rate</option>
<option value="16">amp envelope decay</option>
<option value="17">mod envelope decay</option></select></div>
<div class=slidercontainer>
mod matrix 7 source 1:<select id="0171" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 7 source 2:<select id="0172" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 7 depth:<br><input class="slider" id="0174" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0174"></span><br>
</div>
<div class=slidercontainer>
mod matrix 7 destination:<select oninput="sendNPRN(this.id,this.value);" id="0175" >
<option value="0">osc 1 pitch</option>
<option value="1">osc 1 pitch</option>
<option value="2">osc 2 pitch</option>
<option value="3">osc 1 v-sync</option>
<option value="4">osc 2 v-sync</option>
<option value="5">osc 1 pulse width</option>
<option value="6">osc 2 pulse width</option>
<option value="7">osc 1 level</option>
<option value="8">osc 2 level</option>
<option value="9">noise level</option>
<option value="10">ring modulation 1*2 level</option>
<option value="11">drive amount</option>
<option value="12">frequency</option>
<option value="13">resonance</option>
<option value="14">LFO 1 rate</option>
<option value="15">LFO 2 rate</option>
<option value="16">amp envelope decay</option>
<option value="17">mod envelope decay</option></select></div>
<div class=slidercontainer>
mod matrix 8 source 1:<select id="0176" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 8 source 2:<select id="0177" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 8 depth:<br><input class="slider" id="0179" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0179"></span><br>
</div>
<div class=slidercontainer>
mod matrix 8 destination:<select oninput="sendNPRN(this.id,this.value);" id="017a" >
<option value="0">osc 1 pitch</option>
<option value="1">osc 1 pitch</option>
<option value="2">osc 2 pitch</option>
<option value="3">osc 1 v-sync</option>
<option value="4">osc 2 v-sync</option>
<option value="5">osc 1 pulse width</option>
<option value="6">osc 2 pulse width</option>
<option value="7">osc 1 level</option>
<option value="8">osc 2 level</option>
<option value="9">noise level</option>
<option value="10">ring modulation 1*2 level</option>
<option value="11">drive amount</option>
<option value="12">frequency</option>
<option value="13">resonance</option>
<option value="14">LFO 1 rate</option>
<option value="15">LFO 2 rate</option>
<option value="16">amp envelope decay</option>
<option value="17">mod envelope decay</option></select></div>
<div class=slidercontainer>
mod matrix 9 source 1:<select id="017b" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 9 source 2:<select id="017c" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 9 depth:<br><input class="slider" id="017e" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id017e"></span><br>
</div>
<div class=slidercontainer>
mod matrix 9 destination:<select oninput="sendNPRN(this.id,this.value);" id="017f" >
<option value="0">osc 1 pitch</option>
<option value="1">osc 1 pitch</option>
<option value="2">osc 2 pitch</option>
<option value="3">osc 1 v-sync</option>
<option value="4">osc 2 v-sync</option>
<option value="5">osc 1 pulse width</option>
<option value="6">osc 2 pulse width</option>
<option value="7">osc 1 level</option>
<option value="8">osc 2 level</option>
<option value="9">noise level</option>
<option value="10">ring modulation 1*2 level</option>
<option value="11">drive amount</option>
<option value="12">frequency</option>
<option value="13">resonance</option>
<option value="14">LFO 1 rate</option>
<option value="15">LFO 2 rate</option>
<option value="16">amp envelope decay</option>
<option value="17">mod envelope decay</option></select></div>
<div class=slidercontainer>
mod matrix 10 source 1:<select id="0200" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 10 source 2:<select id="0201" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 10 depth:<br><input class="slider" id="0203" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0203"></span><br>
</div>
<div class=slidercontainer>
mod matrix 10 destination:<select oninput="sendNPRN(this.id,this.value);" id="0204" >
<option value="0">osc 1 pitch</option>
<option value="1">osc 1 pitch</option>
<option value="2">osc 2 pitch</option>
<option value="3">osc 1 v-sync</option>
<option value="4">osc 2 v-sync</option>
<option value="5">osc 1 pulse width</option>
<option value="6">osc 2 pulse width</option>
<option value="7">osc 1 level</option>
<option value="8">osc 2 level</option>
<option value="9">noise level</option>
<option value="10">ring modulation 1*2 level</option>
<option value="11">drive amount</option>
<option value="12">frequency</option>
<option value="13">resonance</option>
<option value="14">LFO 1 rate</option>
<option value="15">LFO 2 rate</option>
<option value="16">amp envelope decay</option>
<option value="17">mod envelope decay</option></select></div>
<div class=slidercontainer>
mod matrix 11 source 1:<select id="0205" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 11 source 2:<select id="0206" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 11 depth:<br><input class="slider" id="0208" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0208"></span><br>
</div>
<div class=slidercontainer>
mod matrix 11 destination:<select oninput="sendNPRN(this.id,this.value);" id="0209" >
<option value="0">osc 1 pitch</option>
<option value="1">osc 1 pitch</option>
<option value="2">osc 2 pitch</option>
<option value="3">osc 1 v-sync</option>
<option value="4">osc 2 v-sync</option>
<option value="5">osc 1 pulse width</option>
<option value="6">osc 2 pulse width</option>
<option value="7">osc 1 level</option>
<option value="8">osc 2 level</option>
<option value="9">noise level</option>
<option value="10">ring modulation 1*2 level</option>
<option value="11">drive amount</option>
<option value="12">frequency</option>
<option value="13">resonance</option>
<option value="14">LFO 1 rate</option>
<option value="15">LFO 2 rate</option>
<option value="16">amp envelope decay</option>
<option value="17">mod envelope decay</option></select></div>
<div class=slidercontainer>
mod matrix 12 source 1:<select id="020a" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 12 source 2:<select id="020b" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 12 depth:<br><input class="slider" id="020d" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id020d"></span><br>
</div>
<div class=slidercontainer>
mod matrix 12 destination:<select oninput="sendNPRN(this.id,this.value);" id="020e" >
<option value="0">osc 1 pitch</option>
<option value="1">osc 1 pitch</option>
<option value="2">osc 2 pitch</option>
<option value="3">osc 1 v-sync</option>
<option value="4">osc 2 v-sync</option>
<option value="5">osc 1 pulse width</option>
<option value="6">osc 2 pulse width</option>
<option value="7">osc 1 level</option>
<option value="8">osc 2 level</option>
<option value="9">noise level</option>
<option value="10">ring modulation 1*2 level</option>
<option value="11">drive amount</option>
<option value="12">frequency</option>
<option value="13">resonance</option>
<option value="14">LFO 1 rate</option>
<option value="15">LFO 2 rate</option>
<option value="16">amp envelope decay</option>
<option value="17">mod envelope decay</option></select></div>
<div class=slidercontainer>
mod matrix 13 source 1:<select id="020f" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 13 source 2:<select id="0210" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 13 depth:<br><input class="slider" id="0212" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0212"></span><br>
</div>
<div class=slidercontainer>
mod matrix 13 destination:<select oninput="sendNPRN(this.id,this.value);" id="0213" >
<option value="0">osc 1 pitch</option>
<option value="1">osc 1 pitch</option>
<option value="2">osc 2 pitch</option>
<option value="3">osc 1 v-sync</option>
<option value="4">osc 2 v-sync</option>
<option value="5">osc 1 pulse width</option>
<option value="6">osc 2 pulse width</option>
<option value="7">osc 1 level</option>
<option value="8">osc 2 level</option>
<option value="9">noise level</option>
<option value="10">ring modulation 1*2 level</option>
<option value="11">drive amount</option>
<option value="12">frequency</option>
<option value="13">resonance</option>
<option value="14">LFO 1 rate</option>
<option value="15">LFO 2 rate</option>
<option value="16">amp envelope decay</option>
<option value="17">mod envelope decay</option></select></div>
<div class=slidercontainer>
mod matrix 14 source 1:<select id="0214" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 14 source 2:<select id="0215" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 14 depth:<br><input class="slider" id="0217" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0217"></span><br>
</div>
<div class=slidercontainer>
mod matrix 14 destination:<select oninput="sendNPRN(this.id,this.value);" id="0218" >
<option value="0">osc 1 pitch</option>
<option value="1">osc 1 pitch</option>
<option value="2">osc 2 pitch</option>
<option value="3">osc 1 v-sync</option>
<option value="4">osc 2 v-sync</option>
<option value="5">osc 1 pulse width</option>
<option value="6">osc 2 pulse width</option>
<option value="7">osc 1 level</option>
<option value="8">osc 2 level</option>
<option value="9">noise level</option>
<option value="10">ring modulation 1*2 level</option>
<option value="11">drive amount</option>
<option value="12">frequency</option>
<option value="13">resonance</option>
<option value="14">LFO 1 rate</option>
<option value="15">LFO 2 rate</option>
<option value="16">amp envelope decay</option>
<option value="17">mod envelope decay</option></select></div>
<div class=slidercontainer>
mod matrix 15 source 1:<select id="0219" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 15 source 2:<select id="021b" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 15 depth:<br><input class="slider" id="021c" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id021c"></span><br>
</div>
<div class=slidercontainer>
mod matrix 15 destination:<select oninput="sendNPRN(this.id,this.value);" id="021d" >
<option value="0">osc 1 pitch</option>
<option value="1">osc 1 pitch</option>
<option value="2">osc 2 pitch</option>
<option value="3">osc 1 v-sync</option>
<option value="4">osc 2 v-sync</option>
<option value="5">osc 1 pulse width</option>
<option value="6">osc 2 pulse width</option>
<option value="7">osc 1 level</option>
<option value="8">osc 2 level</option>
<option value="9">noise level</option>
<option value="10">ring modulation 1*2 level</option>
<option value="11">drive amount</option>
<option value="12">frequency</option>
<option value="13">resonance</option>
<option value="14">LFO 1 rate</option>
<option value="15">LFO 2 rate</option>
<option value="16">amp envelope decay</option>
<option value="17">mod envelope decay</option></select></div>
<div class=slidercontainer>
mod matrix 16 source 1:<select id="021e" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 16 source 2:<select id="0220" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 16 depth:<br><input class="slider" id="0221" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0221"></span><br>
</div>
<div class=slidercontainer>
mod matrix 16 destination:<select oninput="sendNPRN(this.id,this.value);" id="0222" >
<option value="0">osc 1 pitch</option>
<option value="1">osc 1 pitch</option>
<option value="2">osc 2 pitch</option>
<option value="3">osc 1 v-sync</option>
<option value="4">osc 2 v-sync</option>
<option value="5">osc 1 pulse width</option>
<option value="6">osc 2 pulse width</option>
<option value="7">osc 1 level</option>
<option value="8">osc 2 level</option>
<option value="9">noise level</option>
<option value="10">ring modulation 1*2 level</option>
<option value="11">drive amount</option>
<option value="12">frequency</option>
<option value="13">resonance</option>
<option value="14">LFO 1 rate</option>
<option value="15">LFO 2 rate</option>
<option value="16">amp envelope decay</option>
<option value="17">mod envelope decay</option></select></div>
<div class=slidercontainer>
mod matrix 17 source 1:<select id="0223" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 17 source 2:<select id="0225" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 17 depth:<br><input class="slider" id="0226" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0226"></span><br>
</div>
<div class=slidercontainer>
mod matrix 17 destination:<select oninput="sendNPRN(this.id,this.value);" id="0227" >
<option value="0">osc 1 pitch</option>
<option value="1">osc 1 pitch</option>
<option value="2">osc 2 pitch</option>
<option value="3">osc 1 v-sync</option>
<option value="4">osc 2 v-sync</option>
<option value="5">osc 1 pulse width</option>
<option value="6">osc 2 pulse width</option>
<option value="7">osc 1 level</option>
<option value="8">osc 2 level</option>
<option value="9">noise level</option>
<option value="10">ring modulation 1*2 level</option>
<option value="11">drive amount</option>
<option value="12">frequency</option>
<option value="13">resonance</option>
<option value="14">LFO 1 rate</option>
<option value="15">LFO 2 rate</option>
<option value="16">amp envelope decay</option>
<option value="17">mod envelope decay</option></select></div>
<div class=slidercontainer>
mod matrix 18 source 1:<select id="0228" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 18 source 2:<select id="022a" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 18 depth:<br><input class="slider" id="022b" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id022b"></span><br>
</div>
<div class=slidercontainer>
mod matrix 18 destination:<select oninput="sendNPRN(this.id,this.value);" id="022c" >
<option value="0">osc 1 pitch</option>
<option value="1">osc 1 pitch</option>
<option value="2">osc 2 pitch</option>
<option value="3">osc 1 v-sync</option>
<option value="4">osc 2 v-sync</option>
<option value="5">osc 1 pulse width</option>
<option value="6">osc 2 pulse width</option>
<option value="7">osc 1 level</option>
<option value="8">osc 2 level</option>
<option value="9">noise level</option>
<option value="10">ring modulation 1*2 level</option>
<option value="11">drive amount</option>
<option value="12">frequency</option>
<option value="13">resonance</option>
<option value="14">LFO 1 rate</option>
<option value="15">LFO 2 rate</option>
<option value="16">amp envelope decay</option>
<option value="17">mod envelope decay</option></select></div>
<div class=slidercontainer>
mod matrix 19 source 1:<select id="022d" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 19 source 2:<select id="022f" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 19 depth:<br><input class="slider" id="0230" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0230"></span><br>
</div>
<div class=slidercontainer>
mod matrix 19 destination:<select oninput="sendNPRN(this.id,this.value);" id="0231" >
<option value="0">osc 1 pitch</option>
<option value="1">osc 1 pitch</option>
<option value="2">osc 2 pitch</option>
<option value="3">osc 1 v-sync</option>
<option value="4">osc 2 v-sync</option>
<option value="5">osc 1 pulse width</option>
<option value="6">osc 2 pulse width</option>
<option value="7">osc 1 level</option>
<option value="8">osc 2 level</option>
<option value="9">noise level</option>
<option value="10">ring modulation 1*2 level</option>
<option value="11">drive amount</option>
<option value="12">frequency</option>
<option value="13">resonance</option>
<option value="14">LFO 1 rate</option>
<option value="15">LFO 2 rate</option>
<option value="16">amp envelope decay</option>
<option value="17">mod envelope decay</option></select></div>
<div class=slidercontainer>
mod matrix 20 source 1:<select id="0232" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select>
<div class=slidercontainer>
mod matrix 20 source 2:<select id="0234" oninput="sendNPRN(this.id,this.value);" ><option value="0">direct</option>
 
 
 
<option value="4">velocity</option>
<option value="5">keyboard</option>
<option value="6">LFO 1 +</option>
<option value="7">LFO 1 +/-</option>
<option value="8">LFO 2 +</option>
<option value="9">LFO 2 +/-</option>
<option value="10">env amp</option>
<option value="11">env filter</option>
<option value="12">env 3</option></select></div>
<div class=slidercontainer>
mod matrix 20 depth:<br><input class="slider" id="0235" type="range" min="0" max="127" step="1" oninput="sendNPRN(this.id,this.value);" ><span id="id0235"></span><br>
</div>
<div class=slidercontainer>
mod matrix 20 destination:<select oninput="sendNPRN(this.id,this.value);" id="0236" >
<option value="0">osc 1 pitch</option>
<option value="1">osc 1 pitch</option>
<option value="2">osc 2 pitch</option>
<option value="3">osc 1 v-sync</option>
<option value="4">osc 2 v-sync</option>
<option value="5">osc 1 pulse width</option>
<option value="6">osc 2 pulse width</option>
<option value="7">osc 1 level</option>
<option value="8">osc 2 level</option>
<option value="9">noise level</option>
<option value="10">ring modulation 1*2 level</option>
<option value="11">drive amount</option>
<option value="12">frequency</option>
<option value="13">resonance</option>
<option value="14">LFO 1 rate</option>
<option value="15">LFO 2 rate</option>
<option value="16">amp envelope decay</option>
<option value="17">mod envelope decay</option></select></div>
</body></html>)rawliteral";


ESP8266WiFiMulti WiFiMulti;
WebSocketsServer webSocket = WebSocketsServer(81);
ESP8266WebServer server(80);

MIDI_CREATE_DEFAULT_INSTANCE();

void sendNRPN(int MSB, int LSB, int valor, int canal) {
  MIDI.sendControlChange( 99, MSB, canal);

  MIDI.sendControlChange( 98, LSB, canal);

  MIDI.sendControlChange( 6 , valor, canal);

}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  //Handle websocket
  switch (type) {
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
      static byte canal = 1 ; //Midi channel 1=synth1, 2=synth2.
     if (payload[0] == 'C'){
      if (canal==1){
          canal=2;
        }
      else if (canal==2){
          canal=1;
        }
      }
      else if (payload[0] == '#') { //Read CC payload (it's always #CCVV, where CC is 2 hex digit CC and VV is 2 hex dig value).

        uint32_t msg = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
        int M3 = (msg >> 16) & 0xFF;
        int CC = (msg >> 8) & 0xFF;
        int val = (msg >> 0) & 0xFF;
        //USE_SERIAL.print("CC: "); //USE_SERIAL.print(CC);
        //USE_SERIAL.print(" - val: "); //USE_SERIAL.print(val);
        //USE_SERIAL.print(" - canal: "); //USE_SERIAL.print(canal);
        //USE_SERIAL.print(" M3 : "); //USE_SERIAL.println(M3);
        MIDI.sendControlChange(CC, val, canal);
 
      }else if (payload[0] == 'D'){
        
        uint32_t msg = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
        int M3 = (msg >> 16) & 0xFF;
        int CC = (msg >> 8) & 0xFF;
        int val = (msg >> 0) & 0xFF;
        //USE_SERIAL.print("CC: "); //USE_SERIAL.print(CC);
        //USE_SERIAL.print(" - val: "); //USE_SERIAL.print(val);
        //USE_SERIAL.print(" - canal: "); //USE_SERIAL.print(canal);
        //USE_SERIAL.print(" M3 : "); //USE_SERIAL.println(M3);
        MIDI.sendControlChange(CC, val, 10); //10 is fixed channel for drums. 
        }
      else if (payload[0] == 'N') { //Read NPRN payload. It's always NMMLLVV, where MM msb, LL lsb VV value
        uint32_t msg = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
        int MSB = (msg >> 16) & 0xFF;
        int LSB = (msg >> 8) & 0xFF;
        int val = (msg >> 0) & 0xFF;
        sendNRPN(MSB, LSB, val, canal);

        //USE_SERIAL.print("MSB: "); //USE_SERIAL.print(MSB);
        //USE_SERIAL.print(" - LSB: "); //USE_SERIAL.print(LSB);
        //USE_SERIAL.print(" - val: "); //USE_SERIAL.print(val);
        //USE_SERIAL.print(" - canal: "); //USE_SERIAL.println(canal);

      } else if (payload[0]='G'){
          uint32_t msg = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
        int M3 = (msg >> 16) & 0xFF;
        int CC = (msg >> 8) & 0xFF;
        int val = (msg >> 0) & 0xFF;
        //USE_SERIAL.print("CC: "); //USE_SERIAL.print(CC);
        //USE_SERIAL.print(" - val: "); //USE_SERIAL.print(val);
        //USE_SERIAL.print(" - canal: "); //USE_SERIAL.print(canal);
        //USE_SERIAL.print(" M3 : "); //USE_SERIAL.println(M3);
        MIDI.sendControlChange(CC, val, 16);
        }
        else if (payload[0]='g'){
         uint32_t msg = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
        int MSB = (msg >> 16) & 0xFF;
        int LSB = (msg >> 8) & 0xFF;
        int val = (msg >> 0) & 0xFF;
        sendNRPN(MSB, LSB, val, 16);

        //USE_SERIAL.print("MSB: "); //USE_SERIAL.print(MSB);
        //USE_SERIAL.print(" - LSB: "); //USE_SERIAL.print(LSB);
        //USE_SERIAL.print(" - val: "); //USE_SERIAL.print(val);
        //USE_SERIAL.print(" - canal: "); //USE_SERIAL.println(canal);

        }
      break;
  }
}
void handleRoot() { //CC and NRPN
  server.send(200, "text/html", full_html);
}
void handleAssign() { //Knob assign
  server.send(200, "text/html", assign);
} 
void handleMatrix() { //Modulation Matrix
  server.send(200, "text/html", modmatrix);
}
void handleDrums() { //Modulation Matrix
  server.send(200, "text/html", drums_html);
}
void handleGlobal() { //Modulation Matrix
  server.send(200, "text/html", global_html);
}

void setup() {
  //USE_SERIAL.begin(115200);
  MIDI.begin();
  if (!WiFi.config(local_IP, gateway, subnet)) {
    //USE_SERIAL.println("STA Failed to configure");
  }
  WiFiMulti.addAP(MY_SSID, MY_PW);
  while (WiFiMulti.run() != WL_CONNECTED) { //Wait and retry reconnect
    delay(100);
  }
  if (MDNS.begin("esp8266")) {
    //USE_SERIAL.println("MDNS responder started");
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/assign", HTTP_GET, handleAssign);
  server.on("/modmatrix", HTTP_GET, handleMatrix);
  server.on("/drums", HTTP_GET, handleDrums);
  server.on("/global", HTTP_GET, handleGlobal);
  
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  server.begin();
  // Add service to MDNS
  MDNS.addService("http", "tcp", 80);
  MDNS.addService("ws", "tcp", 81);
}
void loop() {
  webSocket.loop();
  server.handleClient();
}
