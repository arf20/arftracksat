#!/bin/bash

ver=$1

# create structure
mkdir -p arftracksat_$ver/usr/local/bin/
mkdir -p arftracksat_$ver/usr/local/share/doc/arftracksat/
mkdir -p arftracksat_$ver/usr/local/etc/arftracksat/
mkdir -p arftracksat_$ver/DEBIAN/

# create debian thing
rm arftracksat_$ver/DEBIAN/control
touch arftracksat_$ver/DEBIAN/control
echo 'Package: arftracksat' >> arftracksat_$ver/DEBIAN/control
echo 'Version: '$ver >> arftracksat_$ver/DEBIAN/control
echo 'Section: base' >> arftracksat_$ver/DEBIAN/control
echo 'Priority: optional' >> arftracksat_$ver/DEBIAN/control
echo 'Architecture: amd64' >> arftracksat_$ver/DEBIAN/control
echo 'Depends: nlohmann-json3-dev (>= 3.7.3-1), libcurl4-openssl-dev (>= 7.68.0-1ubuntu2.6), libcurlpp-dev (>= 0.8.1-2build2)' >> arftracksat_$ver/DEBIAN/control
echo 'Maintainer: arf20 <aruizfernandez05@gmail.com>' >> arftracksat_$ver/DEBIAN/control
echo 'Description: CLI satellite tracking software' >> arftracksat_$ver/DEBIAN/control

# copy files
cp build/arftracksat arftracksat_$ver/usr/local/bin/
cp config.json arftracksat_$ver/usr/local/etc/arftracksat/
cp assets/* arftracksat_$ver/usr/local/share/arftracksat/

# set permissions
chmod 755 -R arftracksat_$ver/

# build deb
dpkg-deb --build arftracksat_$ver
