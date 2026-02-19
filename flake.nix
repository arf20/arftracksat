{
  description = "A graphical satellite tracking software";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-25.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
        };
      in
      rec {
        packages.default = with pkgs; stdenv.mkDerivation {
          pname = "arftracksat";
          version = "unstable";
          src = ./.;

          nativeBuildInputs = [
            cmake
          ];

          buildInputs = [
            curl
            curlpp
            nlohmann_json
            freeglut
            glm
          ]
          ++ lib.optionals stdenv.hostPlatform.isLinux [
            libGL
            libGLU
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
        };

        apps.default = flake-utils.lib.mkApp {
          drv = packages.default;
        };

        devShells.default = pkgs.mkShell {
          inputsFrom = [ packages.default ];
        };
      }
    );
}
