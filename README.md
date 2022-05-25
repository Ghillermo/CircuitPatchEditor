# CircuitPatchEditor
A real time web based patch editor for Novation Circuit running in ESP8266

This is an experiment to make ESP8266 "talk" with Novation Circuit.

Right now, the program allows you to edit all synth parameters in real time, even if they are normally hidden from the UI.

This is a proof of concept, it works only with CCs and the input is sliders. But it will support NRPNs and proper input methods in the future.

I'd love to know your opinion and ideas, it's time to unlock the features we always wanted within the device. Contact me and pull requests! I'm doing this for fun and I'll listen everybody, even if you don't know programming.

To install: 
install libraries, open code and write your desired wifi and IP, upload it. Connect TRS jack with the tip to +V,Ring to ESP tx, and Sleeve to ground. An optocoupler is recommended for extra device safety.

To use: 
Plug jack into MIDI IN, connect to http://192.168.1.4 (or the IP you've chosen). Touch the sliders and they'll update instantly.
