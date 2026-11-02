{
  description = "Indecs dev env";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      utils,
    }:

    utils.lib.eachDefaultSystem (
      system:

      let
        pkgs = import nixpkgs { inherit system; };
        llvm = pkgs.llvmPackages_22;
      in
      {
        devShells.default = pkgs.mkShell.override { stdenv = llvm.libcxxStdenv; } {
          nativeBuildInputs = with pkgs; [
            xmake

            cppcheck

            llvm.llvm
            llvm.lldb
            llvm.clang-tools

            valgrind
          ];

          hardeningDisable = [ "fortify" ];
        };
      }
    );
}
