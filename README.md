# CircuitPatchEditor
A virtual midi CC controller for Novation Circuit running in ESP8266.
The code can easily be adapted to other devices from a midi table using RegExp, let me know if you have any questions.

Right now, the program allows you to edit all synth parameters in real time, even if they are normally hidden from the UI.
>Added functional prototype menus for macro knob edit/modulation Matrix.
>Added synth 2, drums and global controls. Demo: https://www.reddit.com/r/novationcircuit/comments/vdcduq/comment/icjqw4u/?context=3

What you can do with it:
Sound design: tweak and hear any parameter in real time. Save session on your Circuit to keep the patch (won't be stored in your library, just in your current session).
Noodling: response is good enough to perform and jam.
Reassign macro knobs and reroute the mod matrix.

This is not a library, this is just an experiment. Needs testing. If you want to use it in your project please contact me for help.

I'd love to know your opinion and ideas, it's time to unlock the features we always wanted for the device. Contact me and pull requests!
 I'm doing this for fun and I'll listen to everybody, even if you don't know programming you could still have good ideas.
 Specially interested in hearing about desired features from the non-programmer crowd.

To install: 
install dependencies libraries , open code and write your desired wifi and IP, upload it. Connect TRS jack with the tip to 3.3V, ring to ESP tx, and sleeve to ground. Do this at your risk, use optocouplers or some other method if unsure.
>If you want to skip using a router or need a more user friendly way to do this, I can implement it. Just reach me!

To use: 
Plug jack into MIDI IN, connect to http://192.168.1.101 (or the IP you've chosen). Set Circuit to listen to channel one if it isn't already (press shift+sessions while turning on, make sure top left pad is lit). Touch the sliders and they'll immediately affect your synth.

>What's left: to do and limitations (ideas are welcome):

>The UI is nor pretty or tidy, needs some shaping. Right now it's basically an unsorted list of controls. But at least it goes smooth.

>Analog Reads kill the wifi. "Successfully" implemented 8 CV inputs through a mux but it killed the wifi. Some pots can be added, but there's a compromise between samplerate and wifi stability. I've decided to have a second esp unit work as a "port expander" or maybe upgrade to esp32. If you want to try I can write some code but otherwise I'll just keep analogRead a different project (request code for it if interested).
>Session control: select/queue sessionstart,stop and clock are not implemented, but they're trivial to do so ask for it if you need it. Possibly a "song mode" can be added.

>Fetch data from the Circuit/bulk upload/download/store data in the device. This means we can't read the current parameter values.  Those are doable things, including writing samples from an SD card. It seems harder to implement but if anyone finds use for those features I can work it out.

I'll tackle those things in this order, but I can change priorities if someone is interested in something else. Any contribution/guidance/idea on this subjects will be thankfully welcome.



Let me know if you try it or want to, what features you'd want to see next or what trouble did you have and I'll give you support. Cheers and thanks for coming.
