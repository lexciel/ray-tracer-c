{
  description = "Neovim development environment — LSPs managed by Nix, plugins by lazy.nvim";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
        isDarwin = pkgs.stdenv.isDarwin;
        isLinux = pkgs.stdenv.isLinux;

        lspServers = with pkgs; [

          clang-tools # clangd + clang-format + clang-tidy
          cmake-language-server

          nil # Nix LSP

        ];

        devTools =
          with pkgs;
          [
            neovim
            git
            ripgrep # required by Telescope live_grep
            fd # required by Telescope find_files
            gnumake # telescope-fzf-native needs make
            nodejs # some LSPs need node (jdtls, etc.)
            tmux
            lazygit
          ]
          ++ (if isDarwin then [ pkgs.llvmPackages.clang ] else [ pkgs.gcc ])
          ++ pkgs.lib.optionals isLinux [
            pkgs.wl-clipboard
            pkgs.xclip
          ];

      in
      {
        devShells.default = pkgs.mkShell {
          buildInputs = lspServers ++ devTools;

          shellHook = ''
            export NVIM_APPNAME="nvim"
          '';
        };

        packages.default = pkgs.symlinkJoin {
          name = "nvim-wrapped";
          paths = [ pkgs.neovim ] ++ lspServers;
        };
      }
    );
}
