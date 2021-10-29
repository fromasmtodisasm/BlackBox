find_package(Cygwin)


macro(ExecuteCygwinCommand Command)
	set(CMD ${CYGWIN_INSTALL_PATH}/bin/bash --login -c "${Command}"
endmacro()

macro(InstallCygwin Root)
	set(INSTALL_COMMAND "./setup-x86_64.exe -s http://cygwin.mirror.constant.com --root ${Root} -q -a x86_64 -B -P bison flex wget")
endmacro()