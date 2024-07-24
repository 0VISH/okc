OKC

Pull recursively and run src/raylib.py before building
Create "gamePath.hh" under src/ and define GAME_PATH with the path to your game's main.cc 

----ANDROID----
Building for android sucks. It makes me paranoid android

1) local.properties
ndk.dir = absolute_path_to_ndk
cmake.dir = absolute_path_to_CMake(this folder should contain "bin/" which should contain cmake and ninja)

2) app/build.gradle
ndkVersion = "whatever_version_in_your_local_machine"

flush android studio cache and reload project before building