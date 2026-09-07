@echo off
cd /d "%~dp0"
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
echo Compilando...
cl /EHsc /nologo pila.cpp lexico.cpp principal.cpp /Fe:gramatica.exe
if errorlevel 1 (
    echo.
    echo Hubo un error al compilar.
    pause
    exit /b 1
)
echo.
echo ===== Ejecutando =====
echo.
"%~dp0gramatica.exe"
echo.
echo (Presiona una tecla para cerrar esta ventana)
pause >nul
