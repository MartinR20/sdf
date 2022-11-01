(vcvarsall am64 > NUL) || cl sdf.c /Zi /nologo /link /SUBSYSTEM:Windows user32.lib OpenGL32.lib Gdi32.lib || exit /b & sdf.exe 
echo %=ExitCode%
