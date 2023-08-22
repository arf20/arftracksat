{
  description = "A graphical satellite tracking software for linux";

  inputs = {
    nixpkgs.url = github:nixos/nixpkgs/nixos-unstable;
    flake-utils.url = github:numtide/flake-utils;
  };

  outputs = {self, nixpkgs, flake-utils, ...}@inputs: flake-utils.lib.eachDefaultSystem (system:
    let 
      pkgs = import nixpkgs {
        inherit system;
      };
      arftracksat = (with pkgs; stdenv.mkDerivation {
          name = "arftracksat";
          src = ./.;
          nativeBuildInputs = [ 
            cmake
            mesa
            curl
            curlpp
            nlohmann_json
            freeglut
            libGL
            mesa_glu
            glm
            tree
          ];
          # Patch share location
          postUnpack = ''
            path=${placeholder "out"}
            sed -i "s,/usr/local,$path," *-source/src/main.cpp
            sed -i "s,/usr/local,$path," *-source/config.json
          '';
          buildPhase = "make -j $NIX_BUILD_CORES";
          installPhase = ''
            mkdir -p $out/bin $out/etc/arftracksat $out/share/arftracksat
            mv arftracksat $out/bin
            cd ..
            mv config.json $out/etc/arftracksat/config.json
            mv assets/earth.png assets/map.json $out/share/arftracksat
          '';
        }
      );
    in rec {
      defaultApp = flake-utils.lib.mkApp {
        drv = defaultPackage;
      };
      defaultPackage = arftracksat;
      devShell = pkgs.mkShell {
        buildInputs = [
          arftracksat
        ];
      };
    }
  );
}