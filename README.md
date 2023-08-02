# The_Wifydra
Multi-headed 802.11 panopticon

![image](https://github.com/lozaning/The_Wifydra/assets/13127833/40c7db88-5f30-40e3-a340-c568a6b9d1a1)

![image](https://github.com/lozaning/The_Wifydra/assets/13127833/05d1311d-2f22-4797-af1c-c54453f760cb)

Two bodge cables are needed with the current revision of this PCB.
The First goes from one of the two holes in the VCC holes above the ESP32-S3
The Second goes from one of the +3v out from the ESP32-S3 and into the VIN of either the SD Card or the GPS. (I sent to the sd card)
This is due to using +VCC for all the VIN net on the sub nodes and mistakenly using +5v for the VIN net on the ESP-32S3 and the peripherals connected to it. It should have all been the same net. 


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
