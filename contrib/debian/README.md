
Debian
====================
This directory contains files used to package commerciumd/commercium-qt
for Debian-based Linux systems. If you compile commerciumd/commercium-qt yourself, there are some useful files here.

## commerciumcash: URI support ##


commercium-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install commercium-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your commercium-qt binary to `/usr/bin`
and the `../../share/pixmaps/commercium128.png` to `/usr/share/pixmaps`

commercium-qt.protocol (KDE)

