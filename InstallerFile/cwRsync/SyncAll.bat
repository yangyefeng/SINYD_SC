
cd .\\cwRsync

del rsync.log

setlocal enabledelayedexpansion

set /p serverPath=<RemoteServerPath.txt


::�������������ļ�
rsync -auv --timeout=300 %serverPath%ui/config/* ../ConfigFile/
rsync -auv --timeout=300 %serverPath%ui/log/config_version ../RemoteLog/
set /p configVer=<../RemoteLog/config_version
echo [%DATE%%TIME%][���������ļ�][%configVer%] >>../RemoteLog/config.log
echo [%DATE%%TIME%][���������ļ�][%configVer%] >../RemoteLog/configlast.log

::�ϴ����������ļ�
rsync -auv --timeout=300 ../ConfigFile/* %serverPath%ui/config/
echo [%DATE%%TIME%][�ϴ������ļ�] >>../RemoteLog/config.log

::�ϴ��궨ͼƬĿ¼
::rsync -auv --timeout=300 ../CalibrationInfo/* %serverPath%ui/data/
::tree /F ../CalibrationInfo >../RemoteLog/CalibrationInfoTree.log
::echo [%DATE%%TIME%][���ɱ궨��ͼĿ¼] >>../RemoteLog/data.log
Set /p png=<../RemoteLog/pnglist.log
set dpng=%png:/=_% 
set "dir=../CalibrationInfo/"
if exist %dir%%png% (
    rsync -uv --timeout=300 %dir%%png% %serverPath%ui/data/%dpng%
    echo [%DATE%%TIME%][�ϴ�һ�ű궨��ͼ][%png%] >>../RemoteLog/data.log
)
for /f "skip=1 tokens=*"  %%a in (../RemoteLog/pnglist.log) do (
    if exist %dir%%%a (
        echo %%a>>newlist.log
    )
)
move /y newlist.log ../RemoteLog/pnglist.log

::���ؽű��ļ�
rsync -auv --timeout=300 %serverPath%ui/script/* ../RemoteScript/

::�ϴ��ű��ļ�
rsync -auv --timeout=300 ../RemoteScript/* %serverPath%ui/script/


::�ϴ����ݿ⼰���ñ����ļ�
rsync -auv --timeout=300 ../DB %serverPath%ui/data/
rsync -auv --timeout=300 ../ConfigFile_RunTime %serverPath%ui/data/
echo [%DATE%%TIME%][�ϴ����ݿ⼰���ñ����ļ��ļ�] >>../RemoteLog/data.log

cd ..
if exist .\\RemoteLog\SOFTDAT (copy /Y .\\RemoteLog\SOFTDAT SOFTDAT)
if exist SOFTDAT (echo [%DATE%%TIME%][ע�����Ѵ���]>>.\\RemoteLog/log.log) else (echo [%DATE%%TIME%][ע����δ����]>>.\\RemoteLog/log.log)
cd .\\cwRsync

::�ϴ���־�ļ�
rsync -auv --timeout=300 ../RemoteLog/* %serverPath%ui/log/
echo [%DATE%%TIME%][�ϴ���־�ļ�] >>../RemoteLog/log.log

::������־�ļ�
rsync -auv --timeout=300 %serverPath%ui/log/* ../RemoteLog/
echo [%DATE%%TIME%][������־�ļ�] >>../RemoteLog/log.log
