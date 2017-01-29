#Openhab Integration to Hoermann Garage Door Opener#

I'm a proud owner of a gararage with a electric door powered by a [ProMatic](http://www.hoermann.de/fileadmin/_country/dok/ProMatic_3.pdf) opener from hoermann. The device can be used with external switch, but only as a open-->stop-->close-->stop toggle button. My first try to integrate this to my home automation was a [Wemo Maker](http://www.belkin.com/us/p/F7C043/) and an additional reed contact to check if the door is closed or opened.

With the Wemos app this solution is fairly good. For integration into a home automation i do have one additional requirement: I want to have specific buttons for opening and closing. 

In theory i could acchive that with some logic for the Maker; it reality i do have a stupid issue: [the sensor needs to be polled](https://github.com/openhab/openhab1-addons/issues/4967).  A better solution was needed. 
![picture alt](http://i.imgur.com/XiI62Mq.jpg "Wemos with Relais")

## Hardware##
* [Wemos D1 mini esp8266 dev board](https://de.aliexpress.com/item/ESP8266-ESP-12-USB-WeMos-D1-Mini-WIFI-Entwicklungsboard-D1-Mini-NodeMCU-Lua-IOT-Basis-Auf/32674463823.html)
* [Relais Shield](https://de.aliexpress.com/item/NEW-Relay-Shield-WeMos-D1-Mini-ESP8266-Development-Board/32703527015.html)
* IP54 Housing form Hardware Store
* 3d printed holder

The Hoermann Switch is nonnected to the closing contact of the relais, the reed contact between D3 and GND.

## OpenHAB config ##

Item (http binding)
```
String Hoermann "Garagentor [MAP(garage.map):%s]" <garagedoor> {http=">[DOOPEN:GET:http://192.168.178.47/open] >[DOCLOSE:GET:http://192.168.178.47/close]"}
```

Garage.map
```
OPEN=Tor Offen
DOOPEN=Sende Öffnen
CLOSING=Tor Schliesst
RETRY=Neuer Schliessversuch
ERROR=Fehler Torsteuerung
CLOSED=Tor Geschlossen
OPENING=Tor Öffnet
DOCLOSE=Sende Schliessen
```

Sitemap
```
  Switch item=Hoermann  mappings=[DOOPEN="/\\", DOCLOSE="\\/"]
  Text item=Hoermann
```

## Re-Usage ##

if you want to copy this - have fun :-). You would need to adapt the url for updating the item and the ip of the wemos...
