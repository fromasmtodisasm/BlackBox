#!/bin/sh

pushd `pwd`
cd build
cmake -j8 --build . 
popd
