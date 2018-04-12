/////////////////////////////////////////////////////////////////////////////
/// @File name: cement_machine_loader.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	Cement/Asphalt machine Loader Manage Class
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef CEMENT_MACHINE_LOADER_H
#define CEMENT_MACHINE_LOADER_H

#include "../machine_loader.h"
#include "../data_node/mixer_node.h"
#include "../data_node/valued_hopper_node.h"

class CementMachineLoader : public MachineLoader
{
public:
    CementMachineLoader();

    ///*******************************************************
    /// @name:   CementMachineLoader::Load
    /// @author: h
    /// @return: DataNode*
    /// @param:  [in][const char *]szConfigPath
    /// @brief:  ′′?¨Hopperê÷D??á11
    ///*******************************************************
    virtual DataNode* Load(const char* szConfigPath);

protected:
    struct ST_HOPPER{
        char szName[128];
        INT32 id;
        INT32 iData;
		INT32 itype;

        INT32 iInputTap1;
        INT32 iInputTap2;
		vector<INT32> vIputTaps;

        INT32 iOutputTap1;
        INT32 iOutputTap2;

        INT32 iAnalogCanId;

        INT32 iBucketId;
        int iOpenDelay;
        int iCloseDelay;

		int iMeaMode;      //计量方式--例如：木质素的计量方式：0-秤；1-风机
		double dMeaRatio;  //计量系数--例如：木质素的计量系数：克/秒
    };

	enum  EN_HOPPER_ATTR_TYPE
	{
		HOPPER_ATTR_TYPE_REAL = 0,
		HOPPER_ATTR_TYPE_CALC,
		HOPPER_ATTR_TYPE_DEAL,
		HOPPER_ATTR_TYPE_TEMPERATURE
	};

protected:
    virtual void  LoadHopper(const char* szConfigFile, const char* szAppName, ST_HOPPER *pHopper);
    virtual void  LoadBucket(const char* szConfigFile, const char* szAppName, ST_HOPPER *pHopper);
    virtual void  LoadMixer(const char* szConfigFile, const char* szAppName, MixerNode *pMixer);
	virtual void  LoadTHopInputTap(const char* szConfigFile, const char* szAppName, ST_HOPPER *pHopper);

    virtual INT32 GetHopperCount(const char* szConfigFile);
    virtual INT32 GetBucketCount(const char* szConfigFile);
    static  int   CompareHopper4qosrt ( const void *a , const void *b );
#if _WIN32
	virtual void  LoadHopAttrStruct(const char* szConfigFile, const char* szAppName, ST_HOPPER *pHopper, EN_HOPPER_ATTR_TYPE en_hat);
	virtual int GetHopAttrCount(const char* szConfigFile, EN_HOPPER_ATTR_TYPE en_hat);
	virtual void LoadAttrHop(const char* szConfigPath, MixerNode* pMixer, EN_HOPPER_ATTR_TYPE en_hat);
	virtual void LoadSCTemp(const char* szConfigPath, MixerNode* pMixer);
#endif
	virtual void LoadMeasureMethod(const char* szConfigPath, int iAnaId, ValuedHopperNode *pVHopNode);
	virtual void LoadValueAlgorithm(const char* strAnaConfigPath, const char *szAppName,int iCount, ValuedHopperNode *pVHopNode);


	vector<int> m_vHopidList;
};

#endif // CEMENT_MACHINE_LOADER_H
