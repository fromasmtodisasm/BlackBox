#!/bin/sh

pushd `pwd`
cd build
cmake --build .
popd
