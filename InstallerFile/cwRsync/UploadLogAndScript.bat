cd .\\cwRsync

set /p serverPath=<RemoteServerPath.txt

::�ϴ��ű��ļ�
rsync  -auv --timeout=300 ../RemoteScript/* %serverPath%ui/script/ >>rsync.log
echo [%DATE%%TIME%][�ϴ��ű��ļ�] >>../RemoteLog/log.log

::�ϴ�Log�ļ�
rsync  -auv --timeout=300 ../RemoteLog/* %serverPath%ui/log/ >>rsync.log
echo [%DATE%%TIME%][�ϴ���־�ļ�] >>../RemoteLog/log.log

