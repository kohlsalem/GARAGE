#Openhab Integration to Hoermann Garage door Opener#

I'm a proud owner of a Gararage with a electric Door powered by a [ProMatic](http://www.hoermann.de/fileadmin/_country/dok/ProMatic_3.pdf) opener from hoermann. The device can be used with external switch, but only as a open-->stop-->close-->stop toggle button. My first try to integrate this to my home automation was a [Wemo Maker](http://www.belkin.com/us/p/F7C043/) and an additional reed contact to check if the door is closed or opened.

With the Wemos app this solution is fairly good. For integration into a home automation i do have one additional requirement: I want to have specific buttons for opening and closing. 

In theory i could acchive that with some logic for the Maker; it reality i do have a stupid issue: the sensor needs to be polled.  A better solution is needed. 
![picture alt](http://i.imgur.com/XiI62Mq.jpg "Wemos with Relais")

## Hardware

## Usage
#open/close garage door with Hoermann and Reed sensor via wemos + Relais shield

* /open
* /close

if you want to copy this - have fun :-)
