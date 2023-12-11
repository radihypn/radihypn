VERSION 0.7
FROM ubuntu:20.04
WORKDIR /workdir

ubuntu2004-prepare:
    RUN apt update
    RUN DEBIAN_FRONTEND=noninteractive apt install -y build-essential libsqlite3-dev libgstreamer1.0-dev libcurlpp-dev libgtk-3-dev
    RUN DEBIAN_FRONTEND=noninteractive apt install -y libcurl4-openssl-dev
    RUN DEBIAN_FRONTEND=noninteractive apt install -y pkg-config libgtkmm-3.0-dev

build-ubuntu2004:
    FROM +ubuntu2004-prepare
    COPY * .
    RUN make DESTDIR=/workdir
    SAVE ARTIFACT /workdir/radihypn AS LOCAL build/ubuntu2004/
