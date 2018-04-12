/////////////////////////////////////////////////////////////////////////////
/// @File name: SYDDataTransferDef.h
/// @Author: YaoDi
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	传输模块Define头文件
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef _SYD_Data_Transfer_Def_h_
#define _SYD_Data_Transfer_Def_h_


struct SourceInfo
{
	int mix_id;
	int mix_type;
	int source_type;
};

struct SourceInfo2
{
	char mix_project_id[100];
	char mix_section_id[100];
	char mix_machine_id[100];
	int mix_type;
	int source_type;
};

#endif	//_SYD_Data_Transfer_Def_h_
