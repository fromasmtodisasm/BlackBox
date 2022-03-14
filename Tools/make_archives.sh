GAME_DATA=`pwd`/GameData
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

create Data
create Engine --exclude ".*Shaders/|.*/Textures.*"

pushd .
cd Engine
create Scripts
create Textures
create Textures1
create Textures2
popd
