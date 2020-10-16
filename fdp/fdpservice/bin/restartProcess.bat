taskkill /f /IM fdpservice.exe
rem Wait for 5 seconds
rem ping -n 6 127.0.0.1 > nul
rem Start new fdp process
fdpservice.exe -e