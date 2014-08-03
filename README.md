![HaveClip](haveclip-desktop/gfx/logo-text-512.png)

HaveClip - clipboard synchronization tool
=========================================

HaveClip is a peer-to-peer clipboard synchronization tool.
Ctrl+C on one computer, Ctrl+V on another.

Clipboard is instantly synchronized across all your devices. It can handle all
forms of text, images and even application-specific data.

HaveClip is multiplatform. Copy something in one OS, paste in another.
Supports Windows, Linux, Mac OS X and Sailfish OS.

Home page: http://www.havefun.cz/projects/haveclip/

Build requirements
------------------
 - Qt >= 4.7
   - modules core, gui and network
 - QCA (Qt Cryptographic Architecture)
 - Linux version has additional dependencies
   - X11 header files

### Debian based distributions

    # apt-get install g++ libqt4-dev libqt4-dev-bin libX11-dev make pkg-config libqca2-dev libqca2-plugin-ossl

### Fedora and other RPM based distributions

    # yum install gcc-c++ qt-devel xorg-x11-proto-devel qca2-devel qca-ossl

Download
--------
[https://sourceforge.net/projects/haveclip/files/](https://sourceforge.net/projects/haveclip/files/)

Build
-----
    $ tar -xzf haveclip-desktop-0.13.0-src.tar.gz
    $ cd haveclip-desktop-0.13.0-src
    $ qmake (or qmake-qt4)
    $ make

You should now have executable at bin/haveclip and a library at haveclip-core/bin/libhaveclipcore.so.1.

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
synced. Right-click on the tray icon and select Settings. Go to tab Pool,
click on "Add" button and follow the verification process.

If the verification process is successful, the clipboard will be synchronized
between the two computers.

Please remember, that for HaveClip to work properly, the same kind of  encryption has to be used
and all computers should have unique private key and certificate.

Security
--------
It is strongly recommended to use the encryption and have unique private key
and certificate on every computer. HaveClip then ensures, that the clipboard
will be sent and received only to/from authenticated peers (those listed in Pool
with appropriate certificates) and that the transfer will be encrypted.

If one does not use the encryption, literally everyone can change the content
of the clipboard or receive it without user's knowledge.

License
-------
HaveClip is released under GNU/GPL.
