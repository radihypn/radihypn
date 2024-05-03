VERSION 0.7

build-ubuntu20.04:
    FROM ubuntu:20.04
    WORKDIR /workdir
    RUN apt update
    RUN DEBIAN_FRONTEND=noninteractive apt install -y build-essential libsqlite3-dev libgstreamer1.0-dev libcurlpp-dev libgtk-3-dev libcurl4-openssl-dev pkg-config libgtkmm-3.0-dev meson cmake software-properties-common valac gobject-introspection
    RUN add-apt-repository -y ppa:xmake-io/xmake
    RUN apt update
    RUN DEBIAN_FRONTEND=noninteractive apt install -y xmake unzip git
    RUN DEBIAN_FRONTEND=noninteractive apt install -y python3-pip wget curl
    RUN DEBIAN_FRONTEND=noninteractive apt install -y libgirepository1.0-dev python3-gi python3-gi-cairo ninja-build
    RUN DEBIAN_FRONTEND=noninteractive apt install -y python-gi-dev
    COPY . .
    RUN xmake config -yvD --root --mode=release
    RUN xmake build -yvD --root radihypn
    RUN DESTDIR=/destdir xmake install -y --root
    RUN tar -C /destdir -czvf /radihypn.tar.gz .
    SAVE ARTIFACT /radihypn.tar.gz AS LOCAL build/ubuntu20.04/radihypn-ubuntu20.04.tar.gz
