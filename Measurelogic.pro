TEMPLATE = app
CONFIG += console
CONFIG -= qt

INCLUDEPATH += ./InterfaceInclude/
INCLUDEPATH += ./CppSqlite3/
INCLUDEPATH += ./InterfaceInclude/CppSqlite3/
INCLUDEPATH += ./Common/
#INCLUDEPATH += /opt/log4cplus/include/
SOURCES += main.cpp \
    SINYD_DBModule/DBTableQueryRow.cpp \
    SINYD_DBModule/DBTableConfigTool.cpp \
    SINYD_DBModule/DBTable.cpp \
    SINYD_DBModule/DBController.cpp \
    SINYD_Measure/Data/RWData/RWData.cpp \
    SINYD_Measure/Tool/syd_time.cpp \
    SINYD_Measure/Tool/syd_record_file.cpp \
    SINYD_Measure/main/machine_loader.cpp \
    SINYD_Measure/main/data_provider.cpp \
    SINYD_Measure/main/data_node.cpp \
    SINYD_Measure/main/data_collector.cpp \
    SINYD_Measure/main/data_node/valued_hopper_node.cpp \
    SINYD_Measure/main/data_node/raw_recorder.cpp \
    SINYD_Measure/main/data_node/multivalued_hopper_node.cpp \
    SINYD_Measure/main/data_node/mixer_node.cpp \
    SINYD_Measure/main/data_node/hopper_product.cpp \
    SINYD_Measure/main/data_node/hopper_node.cpp \
    SINYD_Measure/main/data_provider/can_data_provider.cpp \
    SINYD_Measure/main/data_provider/analog_calibrator.cpp \
    SINYD_Measure/main/machine_loader/cement_machine_loader.cpp \
    CppSqlite3/sqlite3.c \
    CppSqlite3/CppSQLite3.cpp \
    SINYD_Measure/main/data_provider/rawfile_playback.cpp \
    SINYD_Measure/main/data_node/rawdb_recorder.cpp \
    SINYD_DBModule/DBOperator.cpp \
    SINYD_DBModule/DBModule.cpp \
    lib_Json/json_writer.cpp \
    lib_Json/json_value.cpp \
    lib_Json/json_reader.cpp \
    TinyXML2/tinyxml2.cpp \
    SINYD_DataTransfer/TransDataReader.cpp \
    SINYD_DataTransfer/SYDDataTransferImpl.cpp \
    SINYD_DataTransfer/SYDDataTransfer.cpp \
    SINYD_DataTransfer/SocketWrapper.cpp \
    SINYD_DataTransfer/SocketAPI.cpp \
    SINYD_DataTransfer/ReturnCodeParser.cpp \
    SINYD_DataTransfer/JsonRW.cpp \
    SINYD_DataTransfer/DBFileGetter.cpp \
    SINYD_DataTransfer/DataBreakPointStorage.cpp \
    SINYD_DataTransfer/TRWData.cpp \
    SINYD_DBModule/DBHandleManager.cpp \
    SINYD_Measure/main/data_provider/rawdb_playback.cpp \
    SINYD_Measure/main/data_algorithm/data_algorithm.cpp \
    SINYD_Measure/main/abstract_algorithm.cpp \
    SINYD_Measure/Data/data_node_valuemgr.cpp \
    SINYD_Measure/main/data_node/valuedbytap_hopper_node.cpp \
    SINYD_DataTransfer/TransDataReader_Flume.cpp \
    SINYD_DataTransfer/FlumeDataRW.cpp \
    SINYD_DataTransfer/ConfigGetter.cpp \
    SINYD_DataTransfer/TransConfig.cpp

HEADERS += \
    SINYD_DBModule/DBTableQueryRow.h \
    SINYD_DBModule/DBTableConfigTool.h \
    SINYD_DBModule/DBTable.h \
    SINYD_DBModule/DBController.h \
    SINYD_Measure/Data/Include/Operation_conf.h \
    SINYD_Measure/Data/RWData/RWData.h \
    SINYD_Measure/Include/SwDscuMmdef.h \
    SINYD_Measure/Include/EmsasCommon.h \
    SINYD_Measure/Include/DebugLogger.h \
    SINYD_Measure/Tool/syd_time.h \
    SINYD_Measure/Tool/syd_record_file.h \
    SINYD_Measure/main/machine_loader.h \
    SINYD_Measure/main/data_provider.h \
    SINYD_Measure/main/data_node.h \
    SINYD_Measure/main/data_collector.h \
    SINYD_Measure/main/data_node/valued_hopper_node.h \
    SINYD_Measure/main/data_node/raw_recorder.h \
    SINYD_Measure/main/data_node/multivalued_hopper_node.h \
    SINYD_Measure/main/data_node/mixer_node.h \
    SINYD_Measure/main/data_node/hopper_product.h \
    SINYD_Measure/main/data_node/hopper_node.h \
    SINYD_Measure/main/data_provider/can_data_provider.h \
    SINYD_Measure/main/data_provider/analog_calibrator.h \
    SINYD_Measure/main/machine_loader/cement_machine_loader.h \
    InterfaceInclude/DBModuleDef.h \
    InterfaceInclude/DBModule.h \
    InterfaceInclude/CppSqlite3/sqlite3ext.h \
    InterfaceInclude/CppSqlite3/sqlite3.h \
    InterfaceInclude/CppSqlite3/CppSQLite3.h \
    InterfaceInclude/sqlite3/SydSqliteWrapper.h \
    InterfaceInclude/sqlite3/sydsqlitedef.h \
    InterfaceInclude/sqlite3/Sydsqlite.h \
    InterfaceInclude/sqlite3/sqlite3ext.h \
    InterfaceInclude/sqlite3/sqlite3.h \
    SINYD_Measure/main/data_provider/rawfile_playback.h \
    SINYD_Measure/main/data_node/rawdb_recorder.h \
    SINYD_DBModule/DBOperator.h \
    SINYD_DBModule/DBModulePrivate.h \
    InterfaceInclude/CopyFile.h \
    InterfaceInclude/MeasureDef.h \
    lib_Json/json_tool.h \
    TinyXML2/tinyxml2.h \
    SINYD_DataTransfer/TransDataReader.h \
    SINYD_DataTransfer/SYDDataTransferImpl.h \
    InterfaceInclude/SYDDataTransfer.h \
    SINYD_DataTransfer/SocketWrapper.h \
    SINYD_DataTransfer/SocketThread.h \
    SINYD_DataTransfer/SocketAPI.h \
    SINYD_DataTransfer/ReturnCodeParser.h \
    SINYD_DataTransfer/ProtocolDef.h \
    SINYD_DataTransfer/platform.h \
    SINYD_DataTransfer/JsonRW.h \
    SINYD_DataTransfer/DBFileGetter.h \
    SINYD_DataTransfer/DataBreakPointStorage.h \
    SINYD_DataTransfer/TRWData.h \
    TinyXML2/tinyxml2.h \
    SINYD_DBModule/DBHandleManager.h \
    InterfaceInclude/ThreadTemplateCrossSystem/Singleton.h \
    SINYD_Measure/main/data_provider/rawdb_playback.h \
    SINYD_Measure/main/data_algorithm/data_algorithm.h \
    SINYD_Measure/main/abstract_algorithm.h \
    SINYD_Measure/Data/data_node_valuemgr.h \
    Common/log.h \
    SINYD_Measure/main/data_node/valuedbytap_hopper_node.h \
    SINYD_DataTransfer/TransDataReader_Flume.h \
    SINYD_DataTransfer/FlumeDataRW.h \
    SINYD_DataTransfer/ConfigGetter.h \
    SINYD_DataTransfer/TransConfig.h \
    InterfaceInclude/SYDDataTransferDef.h

OTHER_FILES += \
    CppSqlite3/sqlite3.def \
    SINYD_DataTransfer/ReadMe.txt

#LIBS += -L/opt/log4cplus/lib/ -llog4cplus
LIBS += -lpthread
LIBS += -ldl

