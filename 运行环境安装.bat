
if "%PROCESSOR_ARCHITECTURE%"=="x86" goto x86
if "%PROCESSOR_ARCHITECTURE%"=="AMD64" goto x64
exit

:x64
copy %~dp0\mscomm32.ocx C:\Windows\SysWOW64
copy %~dp0\TeeChart5.ocx C:\Windows\SysWOW64
regsvr32.exe C:\Windows\SysWOW64\mscomm32.ocx
regsvr32.exe C:\Windows\SysWOW64\TeeChart5.ocx
exit
 
:x86
copy %~dp0\mscomm32.ocx C:\Windows\System32
copy %~dp0\TeeChart5.ocx C:\Windows\System32
regsvr32.exe C:\Windows\System32\mscomm32.ocx
regsvr32.exe C:\Windows\System32\TeeChart5.ocx