
cd .\\cwRsync

del rsync.log

setlocal enabledelayedexpansion

set /p serverPath=<RemoteServerPath.txt


::下载最新配置文件
rsync -auv --timeout=300 %serverPath%ui/config/* ../ConfigFile/
rsync -auv --timeout=300 %serverPath%ui/log/config_version ../RemoteLog/
set /p configVer=<../RemoteLog/config_version
echo [%DATE%%TIME%][下载配置文件][%configVer%] >>../RemoteLog/config.log
echo [%DATE%%TIME%][下载配置文件][%configVer%] >../RemoteLog/configlast.log

::上传最新配置文件
rsync -auv --timeout=300 ../ConfigFile/* %serverPath%ui/config/
echo [%DATE%%TIME%][上传配置文件] >>../RemoteLog/config.log

::上传标定图片目录
::rsync -auv --timeout=300 ../CalibrationInfo/* %serverPath%ui/data/
::tree /F ../CalibrationInfo >../RemoteLog/CalibrationInfoTree.log
::echo [%DATE%%TIME%][生成标定截图目录] >>../RemoteLog/data.log
Set /p png=<../RemoteLog/pnglist.log
set dpng=%png:/=_% 
set "dir=../CalibrationInfo/"
if exist %dir%%png% (
    rsync -uv --timeout=300 %dir%%png% %serverPath%ui/data/%dpng%
    echo [%DATE%%TIME%][上传一张标定截图][%png%] >>../RemoteLog/data.log
)
for /f "skip=1 tokens=*"  %%a in (../RemoteLog/pnglist.log) do (
    if exist %dir%%%a (
        echo %%a>>newlist.log
    )
)
move /y newlist.log ../RemoteLog/pnglist.log

::下载脚本文件
rsync -auv --timeout=300 %serverPath%ui/script/* ../RemoteScript/

::上传脚本文件
rsync -auv --timeout=300 ../RemoteScript/* %serverPath%ui/script/


::上传数据库及配置备份文件
rsync -auv --timeout=300 ../DB %serverPath%ui/data/
rsync -auv --timeout=300 ../ConfigFile_RunTime %serverPath%ui/data/
echo [%DATE%%TIME%][上传数据库及配置备份文件文件] >>../RemoteLog/data.log

cd ..
if exist .\\RemoteLog\SOFTDAT (copy /Y .\\RemoteLog\SOFTDAT SOFTDAT)
if exist SOFTDAT (echo [%DATE%%TIME%][注册码已存在]>>.\\RemoteLog/log.log) else (echo [%DATE%%TIME%][注册码未存在]>>.\\RemoteLog/log.log)
cd .\\cwRsync

::上传日志文件
rsync -auv --timeout=300 ../RemoteLog/* %serverPath%ui/log/
echo [%DATE%%TIME%][上传日志文件] >>../RemoteLog/log.log

::下载日志文件
rsync -auv --timeout=300 %serverPath%ui/log/* ../RemoteLog/
echo [%DATE%%TIME%][下载日志文件] >>../RemoteLog/log.log
