# The_Wifydra
Multi-headed 802.11 panopticon

![image](https://github.com/lozaning/The_Wifydra/assets/13127833/40c7db88-5f30-40e3-a340-c568a6b9d1a1)


# BOM
Name	Quantity	Manufacturer Part	Manufacturer

"Adafruit_GPS"	"1"	"Adafruit Ultimate GPS Breakout - 66 channel w/10 Hz updates - Version 3"	""

"MicroSD card breakout board+"	"1"	"Adafruit Ultimate GPS Breakout - 66 channel w/10 Hz updates - Version 3"	""

"ADAFRUIT FEATHER ESP32-S2 W/TFT"	"1"	"4264"	"Adafruit"

"Seeedstudio XIAO"	"16"	"102010328"	"Seeedstudio" (These can be any combination of the esp32-c3 or esp32-s3 ones. The benefits of the s3 is that it support BOTH BT and LE)

# What to flash with what

The Dom.ino file goes on the main esp32-s3 that has the tft screen. 

The Sub.ino file goes on all of the sub seed xaio radios. Each of the sub nodes only scans a single channel for networks. 
YOU MUST MANUALLY CHANGE THE BOARD ID FOR EVERY SUB

If you dont change the Board ID all your radios will all just scan channel 1. There are 14 wifi channels, so iterate through those as you flash each sub
