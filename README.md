![HaveClip](haveclip-desktop/gfx/logo-text-512.png)

HaveClip - clipboard synchronization tool
=========================================

HaveClip is a peer-to-peer clipboard synchronization tool.
Ctrl+C on one computer, Ctrl+V on another.

Clipboard is instantly synchronized across all your devices. It can handle all
forms of text, images and even application-specific data.

HaveClip is multiplatform. Copy something in one OS, paste in another.
Supports Windows, Linux, Mac OS X, Sailfish OS and Android.

Home page: http://www.havefun.cz/projects/haveclip/

Build requirements
------------------
 - Qt >= 4.7
   - modules core, gui and network
 - Linux version has additional dependencies
   - X11 header files
 - OpenSSL

### Debian based distributions

    # apt-get install g++ libqt4-dev libqt4-dev-bin libX11-dev libssl-dev make pkg-config

### Fedora and other RPM based distributions

    # yum install gcc-c++ qt-devel xorg-x11-proto-devel openssl-devel

Build
-----
    $ tar -xzf haveclip-desktop-0.15.0.tar.gz
    $ cd haveclip-desktop-0.15.0
    $ qmake (or qmake-qt4)
    $ make

You should now have an executable at bin/haveclip.

Installation
------------
    $ sudo make install

Usage
-----
HaveClip runs in the background. There is no window when launched, only tray
icon will show up.

Left-click on the tray icon will pop up a menu with clipboard history.
Right-click triggers a menu with options. These two menus are merged
in one in OS X.

First thing to do is to setup devices where the clipboard will be
synced. Right-click on the tray icon and select Settings. Go to tab *Devices*,
click on *Add*" button and follow the verification process.

If the verification process is successful, the clipboard will be synchronized
between the two computers.

Please remember, that for HaveClip to work properly, the same kind of  encryption has to be used
and all computers should have unique private key and certificate.

Security
--------
It is strongly recommended to use the encryption and have unique private key
and certificate on every computer. HaveClip then ensures, that the clipboard
will be sent and received only to/from authenticated peers (those listed in *Devices*
with appropriate certificates) and that the transfer will be encrypted.

If one does not use the encryption, literally everyone can change the content
of the clipboard or receive it without user's knowledge.

License
-------
HaveClip is released under GNU/GPL.
