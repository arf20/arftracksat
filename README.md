# arftracksat: A (no longer CLI only) satellite tracking software for linux
SGDP-4 adaptation shamelessly copied from BatchDrake's suscan.

![image](https://user-images.githubusercontent.com/35542215/170757273-e3393979-2f9a-4c68-ab59-9ab2bfe9298c.png)

![image](https://user-images.githubusercontent.com/35542215/170756857-fb7d43c5-41d3-4cc6-bc0b-30bf321265bc.png)

## Building
Install dependencies
```
sudo apt update
sudo apt install nlohmann-json3-dev libcurl4-openssl-dev libcurlpp-dev libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev
```
Build
```
git clone https://github.com/arf20/arftracksat
cd arftracksat/
mkdir build
cd build
cmake ..
make
sudo make install
```
You may add -j<CPUs> to the make command to build faster.

## Configuring
The default config.json located at /usr/local/etc/arftracksat/ after install should work out of the box
```
Value           Description
tleroot:        Location to get and load TLE files, must be writable by the user,
                note: this default will delete TLEs after reboot. Modification advised
tlefile:        TLE filename to load from tleroot
tlesources:     A array of URLs to curl get into tleroot
updatePerdiod:  Screen update period in milliseconds
station:        Station data
  name:           Name
  lat:            Geodetic latitude
  lon:            East longitude
  hgt:            Altitude (height) over sea level in meters
show:           Array to only show sats by name. Leave empty to show all (possibly not good performing)
columns:        Sat data to show in columns in order
                name, azel, dis, geo, tab, pos, vel
```
  
## Controls
```
General
  1-9:    Select satelite by number
  Z:      Change view to 2D (mercator projection)
  X:      Change view to 3D (perspective earth)
  C:      Exit
  
3D view
  A-D:    Rotate earth along poles
  W-S:    Rotate earth along an axis through the equator, parallel to the camera
  Q-E:    Scale up/down
```

# Where Windows?
I just broke support lol

Old VS project in my personal repos: https://arf20.com/source/arftracksat (it sucks)

# Support & help
https://discord.gg/GpgrnDQqtr
