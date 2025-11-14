@echo off
REM build.bat - Build script untuk Windows
REM Compile socket demo programs menggunakan MinGW g++

setlocal enabledelayedexpansion

echo ========================================
echo Socket Asinkron Demo - Build Script
echo Target Platform: Windows
echo Compiler: MinGW g++
echo ========================================
echo.

REM Check if g++ is available
where g++ >nul 2>&1
if errorlevel 1 (
    echo ERROR: g++ not found!
    echo Please install MinGW and add to PATH.
    echo See docs/windows-setup.md for installation guide.
    pause
    exit /b 1
)

REM Show compiler version
echo Compiler version:
g++ --version | findstr g++
echo.

REM Create directories
if not exist bin mkdir bin
echo Created bin/ directory
echo.

REM Compiler flags
set CXXFLAGS=-std=c++11 -Wall -Wextra -Iinclude
set LDFLAGS=-lws2_32

REM Build counter
set /a COUNT=0
set /a TOTAL=8

REM TCP Examples
set /a COUNT+=1
echo [%COUNT%/%TOTAL%] Building TCP Server...
g++ %CXXFLAGS% src\tcp\tcp_server.cpp -o bin\tcp_server.exe %LDFLAGS%
if errorlevel 1 goto :error
echo   [OK] bin\tcp_server.exe

set /a COUNT+=1
echo [%COUNT%/%TOTAL%] Building TCP Client...
g++ %CXXFLAGS% src\tcp\tcp_client.cpp -o bin\tcp_client.exe %LDFLAGS%
if errorlevel 1 goto :error
echo   [OK] bin\tcp_client.exe

REM UDP Examples
set /a COUNT+=1
echo [%COUNT%/%TOTAL%] Building UDP Server...
g++ %CXXFLAGS% src\udp\udp_server.cpp -o bin\udp_server.exe %LDFLAGS%
if errorlevel 1 goto :error
echo   [OK] bin\udp_server.exe

set /a COUNT+=1
echo [%COUNT%/%TOTAL%] Building UDP Client...
g++ %CXXFLAGS% src\udp\udp_client.cpp -o bin\udp_client.exe %LDFLAGS%
if errorlevel 1 goto :error
echo   [OK] bin\udp_client.exe

REM Async Examples
set /a COUNT+=1
echo [%COUNT%/%TOTAL%] Building Async Server...
g++ %CXXFLAGS% src\async\async_tcp_server.cpp -o bin\async_server.exe %LDFLAGS%
if errorlevel 1 goto :error
echo   [OK] bin\async_server.exe

set /a COUNT+=1
echo [%COUNT%/%TOTAL%] Building Async Client...
g++ %CXXFLAGS% src\async\async_tcp_client.cpp -o bin\async_client.exe %LDFLAGS%
if errorlevel 1 goto :error
echo   [OK] bin\async_client.exe

REM Chat Demo
set /a COUNT+=1
echo [%COUNT%/%TOTAL%] Building Chat Server...
g++ %CXXFLAGS% src\examples\chat_demo\chat_server.cpp -o bin\chat_server.exe %LDFLAGS%
if errorlevel 1 goto :error
echo   [OK] bin\chat_server.exe

set /a COUNT+=1
echo [%COUNT%/%TOTAL%] Building Chat Client...
g++ %CXXFLAGS% src\examples\chat_demo\chat_client.cpp -o bin\chat_client.exe %LDFLAGS%
if errorlevel 1 goto :error
echo   [OK] bin\chat_client.exe

echo.
echo ========================================
echo Build completed successfully!
echo ========================================
echo.
echo Binaries are in bin\ directory:
dir /b bin\*.exe
echo.
echo To run examples:
echo   TCP:   bin\tcp_server.exe  (in one terminal)
echo          bin\tcp_client.exe  (in another terminal)
echo.
echo   UDP:   bin\udp_server.exe
echo          bin\udp_client.exe
echo.
echo   Async: bin\async_server.exe
echo          bin\async_client.exe
echo.
echo   Chat:  bin\chat_server.exe (run first)
echo          bin\chat_client.exe (run multiple instances!)
echo.
goto :end

:error
echo.
echo ========================================
echo Build failed!
echo ========================================
echo Check the error messages above.
echo.
exit /b 1

:end
pause
