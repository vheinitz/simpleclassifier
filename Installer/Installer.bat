set PATH=C:\Qt\6.1\6.1.3\mingw81_64\bin
mkdir ..\deploy

del /Q ..\deploy\*.*

copy C:\Development\eval\build-Arco-Desktop_Qt_6_1_3_MinGW_64_bit-Release\release\*.exe ..\deploy
copy c:\Qt\6.1\6.1.3\mingw81_64\bin\lib*.dll ..\deploy

copy ..\3rdparty\opencv\bin\.* ..\deploy
windeployqt --dir ../deploy ../deploy/Arco.exe
.\bin\InnoSetupPE\InnoSetupPE.exe Arco.iss

