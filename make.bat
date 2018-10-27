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
.\wnd-procs.cpp ^
.\snow-particle.cpp
cd ..