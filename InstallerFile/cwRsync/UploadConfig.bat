cd .\\cwRsync

set /p serverPath=<RemoteServerPath.txt

rsync  -auv --timeout=300 ../ConfigFile/* %serverPath%ui/config/ >>rsync.log
echo [%DATE%%TIME%][�ϴ������ļ�] >>../RemoteLog/config.log

