// TestDBModule.cpp : 定义控制台应用程序的入口点。
//
#include "..\InterfaceInclude\DBModule.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "..\InterfaceInclude\SYDDataTransfer.h"

int main()
{
	CSYDDataTransfer dt;
	SourceInfo info;
	info.mix_id = 1;
	info.mix_type = 2;
	info.source_type = 1;
	dt.Init(info);
	const char* ip = "127.0.0.1";
	int port = 27015;
	dt.Start(ip, port);
	getchar();
	dt.Stop();
	time_t st = time(NULL);
	while (true)
	{
		time_t st2 = time(NULL);
		if (st2 - st > 2)
		{
			break;
		}
	}
	return 0;

	DBInfo tinfo;
	tinfo.DBNamePrefix = "raw_record";
	tinfo.configInfo.DBConfigPath = "Config\\TimeSpan";
	tinfo.configInfo.ConfigTimeStamp = "1234-12-12 01:02:03";
	char cname[5] = { 0 };
	FieldInfo cinfo;
	for (int i = 0; i < 50; i++)
	{
		sprintf(cname, "%d", i);
		cinfo.fieldName = string(cname);
		cinfo.fieldOriginalName = "orgName" + string(cname);
		cinfo.dataType = DATATYPE_REAL;
		tinfo.fields.push_back(cinfo);
	}

	IDBOperator* pDBO = NULL;
	try
	{
		pDBO = CreateDBOperator(tinfo);
		if (pDBO)
		{
			map<string, string> datas;
			for (int i = 0; i < 50; i++)
			{
				if (i%3 == 0)
				{
			 		continue;
				}
				sprintf(cname, "%d", i);
				datas[cname] = "123.45";
			}
			
			//Insert
			for (int a = 0; a < 50; a++)
			{
				int rowID = pDBO->Insert(datas);
			}

			//Query
			IDBQueryRow* pDBQR = pDBO->Query(5, 10);
			while (!pDBQR->eof())
			{
				int pk = pDBQR->RowPK();
				list<string> rawDatas;
				for (int i = 0; i < pDBQR->numDataFields(); i++)
				{
					string fieldName = pDBQR->DataFieldName(i);
					bool bNull = pDBQR->DataFieldIsNull(i);
					string fieldValue = pDBQR->GetStringDataField(i);
					
					rawDatas.push_back(fieldValue);
				}
				pDBQR->nextRow();
			}
			pDBO->QueryFinalize(pDBQR);
		}

		ReleaseDBOperator(pDBO);
		pDBO = NULL;
	}
	catch (CDBModuleException& e)
	{
		if (pDBO)
		{
			ReleaseDBOperator(pDBO);
			pDBO = NULL;
		}
	}


	// 		}
// 	IDBController* pDBC;
// 	CreateDBController(&pDBC);
// 	if (pDBC)
// 	{
// 		TableInfo tinfo;
// 		tinfo.tableName = "raw_record";
// 		char cname[5] = { 0 };
// 		ColumnInfo cinfo;
// 		for (int i = 0; i < 50; i++)
// 		{
// 			sprintf(cname, "%d", i);
// 			cinfo.columnName = string(cname);
// 			cinfo.columnOriginalName = "orgName" + string(cname);
// 			cinfo.dataType = DATATYPE_REAL;
// 			tinfo.columns.push_back(cinfo);
// 		}
// 
// 		string fileName;
// 		CreateDBFileName(DBTYPE_RAW, fileName);
// 		int iRes = pDBC->CreateDBFile(fileName);
// 		if (iRes == DBModule_SUCCESS)
// 		{
// 			IDBTable* pT;
// 			iRes = pDBC->CreateTable(tinfo, &pT);
// 			if (iRes == DBModule_SUCCESS && pT)
// 			{
// 				pT = pDBC->GetTable(tinfo.tableName);
// 				string nameOld = pT->getTableName();
// 				iRes = pT->RenameTable(nameOld, nameOld + string("_new"));
// 				string nameNew = pT->getTableName();
// 				pT = pDBC->GetTable(nameNew);
// 				TableInfo infoNew = pT->GetTableInfo();
// 
// 				map<string, string> datas;
// 				for (int i = 0; i < 50; i++)
// 				{
// 					if (i%3 == 0)
// 					{
// 						continue;
// 					}
// 					sprintf(cname, "%d", i);
// 					datas[cname] = "123.45";
// 				}
// 
// 				for (int a = 0; a < 50; a++)
// 				{
// 					int rowID = -1;
// 					pT->Insert(datas, &rowID);
// 				}
// 
// 				IDBTableQueryRow* pQ;
// 				iRes = pT->Query(5, 10, &pQ);
// 				if (iRes == DBModule_SUCCESS && pQ)
// 				{
// 					list<string> getDatas;
// 					list<int> pkas;
// 					while (!pQ->eof())
// 					{
// 						int pka = -1;
// 						pQ->RowPK(pka);
// 						pkas.push_back(pka);
// 						for (int i = 0; i < pQ->numDataFields(); i++)
// 						{
// 							bool bNull;
// 							iRes = pQ->DataFieldIsNull(i, bNull);
// 							if (!bNull)
// 							{
// 								string filedName;
// 								iRes = pQ->DataFieldName(i, filedName);
// 								string value;
// 								iRes = pQ->GetStringDataField(i, value);
// 								getDatas.push_back(value);
// 							}
// 
// 						}
// 						pQ->nextRow();
// 					}
// 					pT->QueryFinalize(pQ);
// 					pQ = NULL;
// 				}
// 			}
// 		}
// 
// 		ReleaseDBController(pDBC);
// 	}

    return 0;
}

