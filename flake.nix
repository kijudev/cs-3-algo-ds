{
  description = "C/C++ dev shell (LLVM/Clang)";

  inputs.nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";

  outputs =
    { nixpkgs, ... }:
    let
      forAllSystems =
        f: nixpkgs.lib.genAttrs [ "x86_64-linux" "aarch64-linux" ] (s: f nixpkgs.legacyPackages.${s});
    in
    {
      devShells = forAllSystems (
        pkgs:
        let
          llvm = pkgs.llvmPackages_21;
        in
        {
          default = pkgs.mkShell.override { stdenv = llvm.stdenv; } {
            packages = [
              llvm.clang-tools
              llvm.lldb

              pkgs.cmake
              pkgs.ninja
              pkgs.gnumake
              pkgs.pkg-config
              pkgs.bear
              pkgs.neocmakelsp
              pkgs.gersemi

              pkgs.valgrind
              pkgs.cppcheck
            ];

            buildInputs = [ ];
            hardeningDisable = [ "fortify" ];
            CMAKE_EXPORT_COMPILE_COMMANDS = "ON";
          };

          shellHook = ''
            alias build-debug='cmake --preset debug; cmake --build --preset debug'
            alias build-msan='cmake --preset msan; cmake --build --preset msan'
            alias build-release='cmake --preset release; cmake --build preset release'
          '';
        }
      );
    };
}
