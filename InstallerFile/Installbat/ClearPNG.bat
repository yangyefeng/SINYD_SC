if exist CalibrationInfo (
    rd /s /q CalibrationInfo
    cd ./RemoteLog
    cd.>pnglist.log

    echo [%DATE%%TIME%][��ձ궨��ͼ][�����] >>../RemoteLog/script.log
) else (
    cd ./RemoteLog
    cd.>pnglist.log
    echo [%DATE%%TIME%][��ձ궨��ͼ][�����ͼ���ļ��в�����] >>../RemoteLog/script.log
)