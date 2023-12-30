# radihypn Internet Radio
Small and simple gtk3 internet radio that has nice tray icon in gtk3 environments like mate, xfce4. Tray icon does not work on Wayland.

## Install for Ubuntu 20.04

### install deps:

```bash
sudo add-apt-repository -y ppa:xmake-io/xmake
sudo apt update
sudo apt install -y build-essential libsqlite3-dev libgstreamer1.0-dev libcurlpp-dev libgtk-3-dev libcurl4-openssl-dev pkg-config libgtkmm-3.0-dev meson cmake xmake git unzip
```

### clone repo:
```bash
git clone --depth=1 --recursive "https://github.com/radihypn/radihypn"
cd radihypn
```

### build and install locally without root:
```bash
xmake config -y --mode=release && xmake build -y radihypn && DESTDIR=~/.local xmake install -y
```
