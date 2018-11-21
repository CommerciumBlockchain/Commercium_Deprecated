FROM ubuntu:18.04

LABEL maintainer="Commercium Dev Team <info@commercium.net>"
LABEL description="Binary wallet builder for linux or windows commercium nodes. \
With both Dockerfile and docker-compose.yml use 'docker-compose up' to build. \
Read last few lines of final output for extraction command."

##Directory settings for final build
ENV LOS_TYPE qt-linux64
ENV WOS_TYPE win64
ENV CMM_VER v0.1-17

##We need a specific version of the Berkeley database
ENV BERKELEY_DB_URL http://download.oracle.com/berkeley-db
ENV BERKELEY_DB_PACKAGE db-4.8.30.NC.tar.gz

ENV CMM_GIT_URL https://github.com/CommerciumBlockchain/Commercium.git

RUN \
  apt-get update && \
  DEBIAN_FRONTEND=noninteractive apt-get -y upgrade && \
##Root Dependencies   
  DEBIAN_FRONTEND=noninteractive apt-get install -y git build-essential libtool autotools-dev automake autoconf m4 pkg-config libboost-all-dev libssl-dev libevent-dev bsdmainutils cmake wget curl gzip make libzmq3-dev openssl libjpeg-dev libminiupnpc-dev \
##GUI Front end Dependencies
  libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler qt4-linguist-tools qt5-default libqrencode-dev \
##Ubuntu boost Dependencies
  libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-program-options-dev libboost-test-dev libboost-thread-dev \
##Win64 build Dependencies
  g++-mingw-w64-x86-64 mingw-w64-x86-64-dev zip \
&& \
##Ubuntu 16 Run Dependencies
#  DEBIAN_FRONTEND=noninteractive apt-get install -y libevent-pthreads-2.0-5 libboost-chrono1.54-dev libboost-program-options1.54-dev libboost-filesystem1.54-dev && \
  update-alternatives --set x86_64-w64-mingw32-g++ /usr/bin/x86_64-w64-mingw32-g++-posix && \
  mkdir /build

##Install Berkley DB
WORKDIR /build
RUN \
  wget $BERKELEY_DB_URL/$BERKELEY_DB_PACKAGE && \
  tar -xzf $BERKELEY_DB_PACKAGE && \
  cd ./db-4.8.30.NC/build_unix/ && \
  ../dist/configure --prefix=/usr/local --with-db4 --disable-shared --enable-cxx --with-pic && \
  make && \
  make install

##Install Wallet source code and dependencies
RUN \
  git clone $CMM_GIT_URL --depth=1 && \
  cd ./Commercium && \
  git submodule init && \
  git submodule update

##Build moar dependencies
WORKDIR /build/Commercium/depends/libsodium
RUN \
  ./autogen.sh && \
  ./configure && \
  make && make check && \
  make install && \
  cp -r src/libsodium/include/sodium src/libsodium/include/sodium.h /usr/local/include && \
  cp ./src/libsodium/.libs/libsodium.a /usr/local/lib

##Comment an entier section to choose the binary you want to build
WORKDIR /build/Commercium
#~~~~~ START LINUX ~~~~~ 
RUN \
  ./autogen.sh && \
  ./configure --disable-tests --enable-hardening && \
  make && \
  make install

#Prepare the Linux files for export
WORKDIR /build
RUN \
  mkdir commercium-$CMM_VER-$LOS_TYPE && \
  cp Commercium/src/commerciumd Commercium/src/commercium-cli Commercium/src/qt/commercium-qt Commercium/src/commercium-tx Commercium/src/commercium-seeder commercium-$CMM_VER-$LOS_TYPE/ && \
  tar -czf commercium-$CMM_VER-$LOS_TYPE.tar.gz commercium-$CMM_VER-$LOS_TYPE/

##Run the linux node in docker if you want. But the wallet wont be saved in docker, so I dont recommendtrying that. 
#WORKDIR /build/Commercium/src
#RUN \
#  commerciumd &
#
#~~~~~ END LINUX ~~~~~

#~~~~~ START WIN64 ~~~~~ 
WORKDIR /build/Commercium
RUN \
  make clean && \
  sed -e 's/soln.size(), SolutionWidth);/\/\/soln.size(), SolutionWidth);/' -e '/\else$/,/LogPrint/c\ \ \ \ \ \ \ \ }' -e 's/LogPrint/\/\/LogPrint/g' -e 's/crypto\/equihash.h"/crypto\/equihash.h"\n#include "compat\/endian.h"/' -i src/crypto/equihash.cpp && \
  cd depends && \
  make HOST=x86_64-w64-mingw32
RUN \
  ./autogen.sh && \
  CONFIG_SITE=$PWD/depends/x86_64-w64-mingw32/share/config.site ./configure -with-gui --with-seeder=false --disable-shared --disable-tests --disable-bench --prefix=/ && \
  make LDFLAGS=-no-undefined
#Prepare the Windows files for export
WORKDIR /build
RUN mkdir commercium-$CMM_VER-$WOS_TYPE && \
  cp Commercium/src/commercium*.exe Commercium/src/qt/commercium-qt.exe commercium-$CMM_VER-$WOS_TYPE/ && \
  zip -r commercium-$CMM_VER-$WOS_TYPE.zip commercium-$CMM_VER-$WOS_TYPE
#~~~~~ END WIN64 ~~~~~
RUN \
  echo "Now run 'docker cp docker-cmm:/build/commercium-$CMM_VER-$LOS_TYPE.tar.gz .' to save the Linux files you compiled." && \
  echo "Now run 'docker cp docker-cmm:/build/commercium-$CMM_VER-$WOS_TYPE.zip .' to save the Windows files you compiled."
