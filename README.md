Ten Kingdoms: Ancient Adversaries
-----------------------------------
Ten Kingdoms is a public fork of the Seven Kingdoms project for 7kfans.

Forked from Seven Kingdoms 2.15.7
Seven Kingdoms website: www.7kfans.com
Seven Kingdoms source repo: sourceforge.net/projects/skfans

Ten Kingdoms is a community continuation of the Seven Kingdoms project, maintained for the 7kfans community. This fork is based on Seven Kingdoms version 2.15.7 and continues development under the GPL.

## Acknowledgments & Trademarks

- **Seven Kingdoms** is a trademark of Enlight Software Ltd., used here with permission for non-commercial, community development projects.
- The source code for Seven Kingdoms was released under the GPL‑2.0‑or‑later by Enlight Software in 2009. See COPYING for full details.
- **Ten Kingdoms** is a community continuation of the Seven Kingdoms project (via 7kfans), now licensed under GPL‑3.0.

This project does not imply any transfer of trademark ownership or official endorsement by Enlight. For trademark questions, please contact Enlight Software Ltd.

System Requirements
---------

Supported operating systems
* Windows
* Linux
* MacOS

Supported architectures for multiplayer
* x86 compatible processor


Building the game
---------

The build process is based on autotools, as this easily runs on all typical
Windows, Linux and Unix-like systems with minimal requirements. Below are the
additional things that you are expected to provide in your build environment.

For building for Windows, the environment we use is mingw-w64 with a cross-
compiler under Linux. The same mingw-w64 is available from MSYS native to
Windows. There are other similar environments, but we cannot help people
configure their system -- you are expected to know how to use your tools.

Required dependencies
* GCC 4.6+ C++11 compliant compiler
* SDL 2.24.0+
* enet 1.3.xx Reliable UDP networking library
* OpenAL-soft or equivalent driver

Optional dependencies
* libcurl for full 7kfans multiplayer integration (website requires TLS1.2)
* gettext 0.19
* Game music: 10kingdoms-music-2.15
  -> Packagers: Copy music into program data folder or PACKAGE_DATA_DIR
  -> Make sure the file names are in upper-case
* NSIS for building a Windows installer
* TeX Live with latexmk, fontaxes, accanthis, universalis, and yfonts for
  generating the game manual in pdf

Autoconf 2.65+ and autoconf-archive is required when generating a configure
script. You do not need to generate a configure script when you are building
an officially released version of the game as it is already provided in the
source archive. Compiling and using git snapshots for general use is dis-
couraged and instead use the latest officially released version of the game.


Running locally from source build
--------

To run the game from the build directory, you need to point to the game data
folder. The game data folder is set by the environment variable TKDATA. In
a bash shell, this can be accomplished by
bash autogen.sh
./configure
TKDATA=data src/10kingdoms

