#!/bin/bash
echo "making sixnet directory type your password (using sudo don't use root password)"
sudo mkdir -p /opt/sixnet;echo "...done"
echo "copying files..."
sudo cp sxiadk.tar.gz /opt/sixnet/;echo "sxiadk.tar.gz ...done"
sudo cp sxlib.tar.gz /opt/sixnet/;echo "sxlib.tar.gz ...done"
sudo cp sxsamples.tar.gz /opt/sixnet/;echo "sxsamples.tar.gz ...done"
echo "Moving to /opt/sixnet directory"
cd /opt/sixnet2/; echo " ...done"
echo "Uncompressing files"
sudo tar zxvf sxiadk.tar.gz
sudo tar zxvf sxlib.tar.gz
sudo tar zxvf sxsamples.tar.gz
echo "Moving to /usr/bin"
cd /usr/bin
sudo ln -s /opt/sixnet/bin/powerpc-cpp powerpc-cpp
sudo ln -s /opt/sixnet/bin/powerpc-gcjh powerpc-gcjh
sudo ln -s /opt/sixnet/bin/powerpc-jcf-dump powerpc-jcf-dump
sudo ln -s /opt/sixnet/bin/powerpc-jv-scan powerpc-jv-scan
sudo ln -s /opt/sixnet/bin/powerpc-linux-addr2line powerpc-linux-addr2line
sudo ln -s /opt/sixnet/bin/powerpc-linux-ar powerpc-linux-ar
sudo ln -s /opt/sixnet/bin/powerpc-linux-as powerpc-linux-as
sudo ln -s /opt/sixnet/bin/powerpc-linux-c++ powerpc-linux-c++
sudo ln -s /opt/sixnet/bin/powerpc-linux-c++filt powerpc-linux-c++filt
sudo ln -s /opt/sixnet/bin/powerpc-linux-chill powerpc-linux-chill
sudo ln -s /opt/sixnet/bin/powerpc-linux-g++ powerpc-linux-g++
sudo ln -s /opt/sixnet/bin/powerpc-linux-g77 powerpc-linux-g77
sudo ln -s /opt/sixnet/bin/powerpc-linux-gcc powerpc-linux-gcc
sudo ln -s /opt/sixnet/bin/powerpc-linux-gcj powerpc-linux-gcj
sudo ln -s /opt/sixnet/bin/powerpc-linux-ld powerpc-linux-ld
sudo ln -s /opt/sixnet/bin/powerpc-linux-nm powerpc-linux-nm
sudo ln -s /opt/sixnet/bin/powerpc-linux-objcopy powerpc-linux-objcopy
sudo ln -s /opt/sixnet/bin/powerpc-linux-objdump powerpc-linux-objdump
sudo ln -s /opt/sixnet/bin/powerpc-linux-protoize powerpc-linux-protoize
sudo ln -s /opt/sixnet/bin/powerpc-linux-ranlib powerpc-linux-ranlib
sudo ln -s /opt/sixnet/bin/powerpc-linux-readelf powerpc-linux-readelf
sudo ln -s /opt/sixnet/bin/powerpc-linux-size powerpc-linux-size
sudo ln -s /opt/sixnet/bin/powerpc-linux-strings powerpc-linux-strings
sudo ln -s /opt/sixnet/bin/powerpc-linux-strip powerpc-linux-strip
sudo ln -s /opt/sixnet/bin/powerpc-linux-unprotoize powerpc-linux-unprotoize
echo "...done"
