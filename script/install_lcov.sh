#!/usr/bin/env bash
echo "Installing lcov"
cd $1
wget http://ftp.de.debian.org/debian/pool/main/l/lcov/lcov_1.14.orig.tar.gz
tar xf lcov_1.14.orig.tar.gz
sudo make -C lcov-1.14/ install
gem install coveralls-lcov
rm lcov_1.14.orig.tar.gz
