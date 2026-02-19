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

        arftracksat = with pkgs; stdenv.mkDerivation {
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
          postPatch = ''
            substituteInPlace src/main.cpp --replace-fail '/usr/local' "$out"
            substituteInPlace config.json --replace-fail '/usr/local' "$out"
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
      in {
        packages.default = arftracksat;

        apps.default = flake-utils.lib.mkApp {
          drv = arftracksat;
        };

        devShells.default = pkgs.mkShell {
          inputsFrom = [ arftracksat ];
        };
      }
    );
}
