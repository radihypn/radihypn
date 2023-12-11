# used libraries:
# gtkmm-3.0
# libcurl
# utest (copied in source)
# https://github.com/nlohmann/json
# https://github.com/geekyMrK/SQLiteCpp

CXX ?= c++
PKGCONF ?= pkg-config
CXXFLAGS ?= -O2
CXXFLAGS += ${TRAY_FLAGS}

EXECUTABLE_NAME ?= radihypn

DESTDIR ?= .
PREFIX ?= .
	
PKGCONF_DEPS ?= gstreamer-1.0 sqlite3 curlpp libcurl gtkmm-3.0

build:
	mkdir -p ${DESTDIR}/${PREFIX}
	${CXX} ${CXXFLAGS} main.cpp $$(${PKGCONF} --libs --cflags ${PKGCONF_DEPS}) -o ${DESTDIR}/${PREFIX}/${EXECUTABLE_NAME}


.PHONY: check entr


entr:
	find *.c *.h *.cpp *.hpp | entr -s "make CXXFLAGS='-O0' && env TESTING=1 ./${EXECUTABLE_NAME} --filter=w.*"
