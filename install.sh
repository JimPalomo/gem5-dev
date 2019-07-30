#!/bin/bash

echo "Updating OS"
sudo apt-get update
sudo apt-get upgrade

echo "Installing gem5 packages..." 

sudo apt-get install git
sudo apt-get install build-essential
sudo apt-get install scons
sudo apt-get install python dev
sudo apt-get install libprotobuf-dev python-protobuf protobuf-compiler libgoogle-perftools-dev
git clone https://gem5.googlesource.com/public/gem5
scons build/X86/gem5.opt -j3 #3 cores + 1

cd
cd gem5/src/
mkdir tutorial
cd tutorial

echo "Done installing"