#GAME_DATA=`pwd`/GameData
GAME_DATA=`pwd`/Assets
if [ ! -d $GAME_DATA ]; then
  mkdir $GAME_DATA
fi

RC=`pwd`/Tools/rc/rc

function rc()
{
	eval "$RC $@" --verbose
}

# $1 name
function create()
{
	rc --create $1 $GAME_DATA/$1.pak --verbose
}

##############################################################
#create Data
pushd .
cd Data
for filename in Music Sounds minecraft; do
    create $filename
done

popd
##############################################################
pushd .
##############################################################
cd Engine
for filename in Scripts Textures Textures1 Textures2 Fonts; do
    create $filename
done
##############################################################
popd
