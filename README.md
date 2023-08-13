# arftracksat: A graphical satellite tracking software for linux

SGDP-4 adaptation shamelessly copied from BatchDrake's suscan.

![arftracksat](https://user-images.githubusercontent.com/35542215/187993716-356f1ce6-88f8-4734-b260-4ac9b984f914.PNG)

## Building

Install dependencies:

```shell
sudo apt update
sudo apt install nlohmann-json3-dev libcurl4-openssl-dev libcurlpp-dev libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev libglm-dev
```

Build:

```shell
git clone https://github.com/arf20/arftracksat
cd arftracksat/
mkdir build
cd build
cmake ..
make
sudo make install # omit this if only building for development
```

You may add `-j <CPUs>` to the `make` command to build faster.

## Running

Without installing, for development:

```
./arftracksat
```

After installing, just run `arftracksat` anywhere.

## Configuring

The default `config.json` located at `/usr/local/etc/arftracksat/` after install should work out of the box.

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

## Troubleshooting

- If arftracksat fails with "No sats", the TLE file contains no TLEs. Likely the TLE server (i.e. celestrack) returned a redirect or something.
- If no sats are found in the TLE, celestrack (or similar) probably returned no TLEs but rather a redirect or something like that instead.

## Assets

The cylindrical earth texture is `world.topo.bathy.200412.3x5400x2700` from NASA's <https://visibleearth.nasa.gov/images/73909/december-blue-marble-next-generation-w-topography-and-bathymetry>.

## Further development

Working on converting this to modern GL (GLFW/OpenGL 3.3) (branch feature/moderngl), and making a nice Qt GUI for it (branch qt), and maybe even at some point I could make a SigDigger and/or SDR++ plugin (branch plugin).

## Where Windows?

I just broke support lol.

Old VS project in my personal repos: <https://arf20.com/source/arftracksat> (it sucks).

## Support & help

<https://discord.gg/GpgrnDQqtr>
