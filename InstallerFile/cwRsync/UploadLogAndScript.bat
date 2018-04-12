cd .\\cwRsync

set /p serverPath=<RemoteServerPath.txt

::上传脚本文件
rsync  -auv --timeout=300 ../RemoteScript/* %serverPath%ui/script/ >>rsync.log
echo [%DATE%%TIME%][上传脚本文件] >>../RemoteLog/log.log

::上传Log文件
rsync  -auv --timeout=300 ../RemoteLog/* %serverPath%ui/log/ >>rsync.log
echo [%DATE%%TIME%][上传日志文件] >>../RemoteLog/log.log

