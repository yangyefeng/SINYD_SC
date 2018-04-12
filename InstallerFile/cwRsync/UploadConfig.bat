cd .\\cwRsync

set /p serverPath=<RemoteServerPath.txt

rsync  -auv --timeout=300 ../ConfigFile/* %serverPath%ui/config/ >>rsync.log
echo [%DATE%%TIME%][上传配置文件] >>../RemoteLog/config.log

