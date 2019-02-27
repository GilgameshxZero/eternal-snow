if not defined DevEnvDir (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
)

call "..\build\build-inc.bat"
call rc /Fo"..\obj\rc.res" "..\rc\rc.rc"
call cl ^
/EHsc ^
/Fo"..\obj\\" ^
/Fe"..\bin\eternal-snow.exe" ^
/O2 ^
/Ot ^
/Ox ^
/MT ^
/MP ^
/incremental ^
..\obj\rc.res ^
..\src\*.cpp ^
..\src\rain-aeternum\*.cpp