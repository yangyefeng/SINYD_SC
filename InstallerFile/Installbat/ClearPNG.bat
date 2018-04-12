if exist CalibrationInfo (
    rd /s /q CalibrationInfo
    cd ./RemoteLog
    cd.>pnglist.log

    echo [%DATE%%TIME%][清空标定截图][已清空] >>../RemoteLog/script.log
) else (
    cd ./RemoteLog
    cd.>pnglist.log
    echo [%DATE%%TIME%][清空标定截图][保存截图的文件夹不存在] >>../RemoteLog/script.log
)