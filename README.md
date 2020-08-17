# ukui-notification-daemon
 ukui-notification-daemon displays passive popup notifications on the Linux desktop without interrupting user's work.

## build & install
```
mkdir build
cd build
qmake ..
make
sudo make install INSTALL_ROOT=/usr
```

## test
`notify-send -a foo -i "/usr/share/icons/Adwaita/scalable/apps/web-browser-symbolic.svg" Hello UKUI!`

## How to report bugs
Bugs should be reported to the UKUI bug tracking system:

https://github.com/ukui/ukui-notification-daemon/issues
