@echo off
setlocal enabledelayedexpansion

cd .\\ConfigFile_RunTime

(for /f "delims=" %%i in ('type "AppManager.conf"') do (
    set "str=%%i" 
    if "!str:Command=0!" neq "%%i" (
        echo Command=5
    ) else (
        echo,%%i
    )

)) > AppManager_tmp.conf
move /y AppManager_tmp.conf AppManager.conf

ping -n 3 127.0.0.1

set res = 0;
for /f "delims=" %%j in ('type "AppManager.conf"') do (
    set "stat=%%j"
    if "!stat:PNGCaptureState=a!" neq "%%j" (
        if "%%j" == "PNGCaptureState=0" (
            set /a res = 1
        )
    )
    if "!stat:CollectionAppState=a!" neq "%%j" (
        if "%%j" == "CollectionAppState=0" (
            set /a res= 2
        )
    )
)

if %res% == 1 (
    echo [%DATE%%TIME%][ֹͣ�궨��ͼ][ֹͣ�ɹ�] >>../RemoteLog/script.log
) else (
    if %res% == 2 (
        echo [%DATE%%TIME%][ֹͣ�궨��ͼ][ֹͣʧ�ܣ��ɼ�����δ����] >>../RemoteLog/script.log
    ) else (
        echo [%DATE%%TIME%][ֹͣ�궨��ͼ][ֹͣʧ��] >>../RemoteLog/script.log
    )
)