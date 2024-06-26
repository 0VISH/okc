@echo off

set buildDir=bin\win\dbg
set buildFlags=/Z7 /D DBG=1
set linkerFlags=/DEBUG /PDB:%buildDir%\okc.pdb

if "%1"=="rls" (
    set buildDir=bin\win\rls
    set buildFlags=/D RLS=1
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
    cl /nologo src/Windows/rcore.c /Ivendor/raylib/src/external/glfw/include/ -c %buildFlags% /Fo:%rayDir%\rcore.obj

    lib /nologo %rayDir%\rcore.obj %rayDir%\utils.obj %rayDir%\rglfw.obj %rayDir%\raudio.obj %rayDir%\rmodels.obj %rayDir%\rshapes.obj %rayDir%\rtext.obj %rayDir%\rtextures.obj /out:%rayDir%\raylib.lib
)

cl /nologo /Ivendor/raylib/src/ src/Windows/main.cc -c %buildFlags% /Fo:%buildDir%\okc.obj
link /NOLOGO %buildDir%\okc.obj %rayDir%\raylib.lib %linkerFlags% /OUT:%buildDir%\okc.exe