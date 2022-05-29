# arftracksat: A CLI satellite tracking software for linux
SGDP4 adaptation shamelessly copied from BatchDrake's suscan.

![image](https://user-images.githubusercontent.com/35542215/170756857-fb7d43c5-41d3-4cc6-bc0b-30bf321265bc.png)

## Building
Install dependencies
```
sudo apt update
sudo apt install nlohmann-json3-dev libcurl4-openssl-dev libcurlpp-dev
```
Build
```
git clone https://github.com/arf20/arftracksat
cd arftracksat/src
mkdir build
cd build
cmake ..
make
sudo make install
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
```
You may add the last line to .bashrc to make it persistant. It makes linux search in /usr/local/lib for libraries

## Configuring
The default config.json should work out of the box
```
Value           Description
sgp4libroot:    Location of SGP4 shared libraries (no modification encouraged)
licpath:        License of SGP4 path (space-track(NORAD)'s library requires it)
tleroot:        Location to get and load TLE files, must be writable by the user,
                note: this default will delete TLEs after reboot. Modification advised
tlefile:        TLE file to load from tleroot
tlesources:     A array of URLs to curl get into tleroot
updatePerdiod:  Screen update period in milliseconds
station:        Station data
  name:           Name
  lat:            Geodetic latitude
  lon:            East longitude
  hgt:            Altitude (height) over sea level in meters
show:           Array to show sats by name
columns:        Sat data to show in columns in order
                name, azel, dis, geo, tab, pos, vel
satidnames:     A table to convert from 5 digit International Designators to human readable common name
                You may need to add entries for new TLEs and sats, or else they will show as unknown
                The SGP4 library that reads TLEs, ignores the title line in the TLE
```

## Launching
Currently config.json must be in the current working directory, by default /usr/local/etc/arftracksat/
```
cd /usr/local/etc/arftracksat/
arftracksat
```

# Where Windows?
I just broke support lol
Old VS project in my personal repos: https://arf20.com/source/arftracksat (it sucks)

# Support
https://discord.gg/GpgrnDQqtr
