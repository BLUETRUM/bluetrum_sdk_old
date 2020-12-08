@echo off
cd /d %~dp0
set proj_name=app
echo 1 > ..\obj\ram.o
@echo on
riscv32-elf-objdump -h -d -t %proj_name%.rv32 > %proj_name%.lst || goto err
riscv32-elf-objcopy -O binary %proj_name%.rv32 %proj_name%.bin || goto err
riscv32-elf-xmaker -b %proj_name%.xm || goto err
if exist C:\upload\upload.bat       (call C:\upload\upload.bat %proj_name%.dcf)
REM .\riscv32-elf-xmaker -b download.xm || goto err
exit
:err
@echo off
if "%1"=="" pause
exit /b 1