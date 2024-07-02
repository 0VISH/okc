@echo off

set buildDir=bin\win\dbg
set buildFlags=/Z7 /D DBG=1
set linkerFlags=/DEBUG /PDB:%buildDir%\okc.pdb

if "%1"=="rls" (
    set buildDir=bin\win\rls
    set buildFlags=/D RLS=1 /Isrc/Game/
    set linkerFlags=
)

set rayDir=%buildDir%\raylib

if not exist %buildDir% mkdir %buildDir%
if not exist %rayDir% (
    mkdir %rayDir%
    cl /nologo vendor/raylib/src/rglfw.c -c %buildFlags% /Fo:%rayDir%\rglfw.obj
    cl /nologo vendor/raylib/src/raudio.c -c %buildFlags% /Fo:%rayDir%\raudio.obj
    cl /nologo vendor/raylib/src/rmodels.c -c %buildFlags% /Fo:%rayDir%\rmodels.obj
    cl /nologo vendor/raylib/src/rshapes.c -c %buildFlags% /Fo:%rayDir%\rshapes.obj
    cl /nologo vendor/raylib/src/rtext.c -c %buildFlags% /Fo:%rayDir%\rtext.obj
    cl /nologo vendor/raylib/src/rtextures.c -c %buildFlags% /Fo:%rayDir%\rtextures.obj
    cl /nologo vendor/raylib/src/utils.c -c %buildFlags% /Fo:%rayDir%\utils.obj
    cl /nologo /Ivendor\raylib\src\external\glfw\include\ src/Windows/win32Raylib.c -c %buildFlags% /Fo:%rayDir%\win32Raylib.obj

    lib /nologo %rayDir%\win32Raylib.obj %rayDir%\utils.obj %rayDir%\rglfw.obj %rayDir%\raudio.obj %rayDir%\rmodels.obj %rayDir%\rshapes.obj %rayDir%\rtext.obj %rayDir%\rtextures.obj /out:%rayDir%\raylib.lib
)
if not exist %buildDir%\win32.obj (
    cl /nologo src/Windows/win32.cc -c %buildFlags% /Fo:%buildDir%\win32.obj
)
if not exist %buildDir%\box2d.obj (
    cl /nologo /Ivendor/box2d/include/ /Ivendor/box2d/src/ src/box2d.cc -c %buildFlags% /Fo:%buildDir%\box2d.obj
)
set buildSand=F
if not exist %buildDir%\sandbox.obj set buildSand=T
if "%1"=="sand" set buildSand=T
if "%1"=="rls" set buildSand=F
if "%buildSand%"== "T" (
    cl /nologo /Isrc/Game/ /Ivendor/raylib/src/ /Ivendor/vendor/box2d/include/ sandbox/game.cc -c %buildFlags% /Fo:%buildDir%\sandbox.obj
    link /NOLOGO %buildDir%\sandbox.obj %buildDir%\box2d.obj /DLL /OUT:%buildDir%\game.dll /DEBUG /PDB:%buildDir%\game.pdb
)

cl /nologo /Ivendor/raylib/src/ src/main.cc -c %buildFlags% /Fo:%buildDir%\okc.obj
link /NOLOGO %buildDir%\okc.obj %buildDir%\win32.obj %rayDir%\raylib.lib %linkerFlags% /OUT:%buildDir%\okc.exe
