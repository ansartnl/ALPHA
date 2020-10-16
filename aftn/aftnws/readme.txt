BUILDING

UNIX
  RELEASE
    mkdir qtcreator-build
    cd qtcreator-build
    cmake .. -DCMAKE_BUILD_TYPE=Release
    ####### install to system using make
    sudo make install
    ####### uninstall to system using mingw32-make
    sudo make uninstall
    ####### Create doxygen html documentation for sources
    make doxygen
    ####### Create installator (win32 - nsis executable, unix - zsh, tar.gz, tar.Z)
    make cpack
    ####### Create installator (win32 - nsis executable, unix - zsh, tar.gz, tar.Z)
    mingw32-make cpack

  DEBUG
    mkdir qtcreator-build-debug
    cd qtcreator-build-debug
    cmake .. -DCMAKE_BUILD_TYPE=Debug
    make


WINDOWS
  RELEASE
    mkdir qtcreator-build
    cd qtcreator-build
    cmake .. -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles"
    mingw32-make
    ####### install to system from sources using mingw32-make
    mingw32-make install
    ####### uninstall to system from sources using mingw32-make
    mingw32-make uninstall
    ####### Create doxygen html documentation for sources
    mingw32-make doxygen
    ####### Create installator (win32 - nsis executable, unix - zsh, tar.gz, tar.Z)
    mingw32-make cpack

  DEBUG
    mkdir qtcreator-build-debug
    cd qtcreator-build-debug
    cmake .. -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles"
    mingw32-make
