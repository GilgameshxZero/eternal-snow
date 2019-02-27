@echo off
setlocal enabledelayedexpansion
set filename=../rc/build.h
if exist "%filename%.tmp" del "%filename%.tmp"
for /F "tokens=*" %%R in (%filename%) do (
	if "%%R"=="" echo. >> "%filename%.tmp"
	set incflag=0
	for /f "tokens=1-3 delims= " %%A in ("%%R") do (
		if "%%B"=="VERSION_BUILD" set incflag=1
		if !incflag! equ 1 (
			set /a num=%%C+1
			echo %%A %%B            !num!>> "%filename%.tmp"
		) else (
			echo %%R>> "%filename%.tmp"
		)
	)
)
del "%filename%"
ren "%filename%.tmp" "%filename%"
