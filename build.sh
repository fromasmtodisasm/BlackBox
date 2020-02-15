#!/bin/sh

pushd `pwd`
cd build
cmake --build . -- -j8
popd
