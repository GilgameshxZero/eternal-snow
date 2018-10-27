call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cd src
call ".\build-id-inc.bat"
REM call rc /Fo".\..\obj\rc.res" rc.rc
call cl ^
/EHsc ^
/Fo".\..\obj\\" ^
/Fe".\..\bin\eternal-snow.exe" ^
/O2 ^
/Ot ^
/Ox ^
/MT ^
/MP ^
/incremental ^
.\main.cpp ^
.\main-wnd-proc.cpp ^
.\snow-particle.cpp ^
.\timer-proc.cpp
cd ..