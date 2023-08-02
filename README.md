# The_Wifydra
Multi-headed 802.11 panopticon

The Wifyrda is a tool for the simulanious monitoring all 2.4Ghz for wifi networks, as well as the ability to geotag the location of found networks and write all this data to an SD card for uploading to Wigle.net

![image](https://github.com/lozaning/The_Wifydra/assets/13127833/40c7db88-5f30-40e3-a340-c568a6b9d1a1)

![image](https://github.com/lozaning/The_Wifydra/assets/13127833/05d1311d-2f22-4797-af1c-c54453f760cb)


# Build Note 1
Two bodge cables are needed with the current revision of this PCB.
The First goes from one of the two holes in the VCC holes above the ESP32-S3
The Second goes from one of the +3v out from the ESP32-S3 and into the VIN of either the SD Card or the GPS. (I sent to the sd card)
This is due to using +VCC for all the VIN net on the sub nodes and mistakenly using +5v for the VIN net on the ESP-32S3 and the peripherals connected to it. It should have all been the same net. 

# Build Note 2
To ensure that home built feel for every wifydra, I made sure to not check the footprint of the seed xaio esp32-C3 that I used in this, resulting in the board having a larger footprint for these modules than should have been used. The chips themselves have castilated edges, and both power and ground are on the same side, so just scooch 'em over a bit and make sure you get those pins connected up to the board. 

In the event I can make the swap over from ESP-NOW for Sub to DOM communications to i2c, I'll ensure to assign the pins to the ones on the same side as 5v and ground, so you'd only have to solder up more pins on the side already lined up.


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

# Powering the Wifydra
The four holes above the Wifydra are labled ground and VCC are where the power is designed to be put into the board.
The board expects to get 5v from its power source. There are a variety of options available to accomplish this. I've got an 18650 battery pack that outputs over usb c, but also has 5v and 3.3v outlets. I connect the 5v and ground from that to the VCC and ground on the board and it works great. You could also solder a USB C decoy trigger set to out put 5 volts and use a standard power bank. I've also had luck with power the entire thing by plugging in the USB C powert of the Dom ESP32-S3 feather TFT, but im not sure this is advisable as Im unclear on how much power draw this puts on the TFT feather to power all the Subs.  
