if [ ! -d GameData ]; then
  mkdir GameData
fi
set RC="./Tools/rc/rc.exe"
echo $RC

./Tools/rc/rc.exe --create Data GameData/Data.pak --verbose
pushd .
cd Engine
./Tools/rc/rc.exe --create Scripts ../GameData/Scripts.pak --verbose

popd

./Tools/rc/rc.exe --create Engine GameData/Engine.pak --exclude ".*Shaders/" --verbose
