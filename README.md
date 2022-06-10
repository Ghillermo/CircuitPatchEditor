# CircuitPatchEditor
A virtual midi CC controller for Novation Circuit running in ESP8266.
The code can easily be adapted to other devices from a midi table using RegExp, let me know if you have any questions.

Right now, the program allows you to edit all synth parameters in real time, even if they are normally hidden from the UI.

This is not a library, this is just an experiment. Needs testing. If you want to use it in your project please contact me for help.

I'd love to know your opinion and ideas, it's time to unlock the features we always wanted within the device. Contact me and pull requests! I'm doing this for fun and I'll listen to everybody, even if you don't know programming.

To install: 
install libraries, open code and write your desired wifi and IP, upload it. Connect TRS jack with the tip to 3.3V, ring to ESP tx, and sleeve to ground. Do this at your risk, use optocouplers or some other method if unsure.

To use: 
Plug jack into MIDI IN, connect to http://192.168.1.4 (or the IP you've chosen). Set Circuit to listen to channel one if it isn't (press shift+sessions while turning on, make sure top left pad is lit). Touch the sliders and they'll immediately affect your synth.

To do:
Testing! Switches, select MIDI channel, better UI. Mixer, Master fx, drums. CV/knob input that doesn't kill wifi.

Let me know if you try it or want to, what features you'd want to see next or what trouble did you have and I'll give you support. Cheers and thanks for coming.
