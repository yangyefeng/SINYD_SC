/////////////////////////////////////////////////////////////////////////////
/// @File name: multivalued_hopper_node.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	Mto Measure Hopper Node
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef MULTIVALUED_HOPPER_NODE_H
#define MULTIVALUED_HOPPER_NODE_H

#include "valued_hopper_node.h"

/**
  @brief 具有多个入料口的、可计量的料仓。对应水泥／沥青拌和机的多对一部署方式

  */
class MultivaluedHopperNode : public ValuedHopperNode
{
public:
    MultivaluedHopperNode();

    /**
    @overload
    */
    virtual void Initialize(DataProvider *pDataProvider);

    /**
    @overload
    */
    virtual void Execute();

    /**
      @overload
      */
    virtual string ToString();

protected:
    /**
      @overload
      */
    virtual void OnOutputStart(HopperNode *pHopper);

    /**
      @overload
      */
    virtual void OnOutputDone(HopperNode *pHopper);

    /**
      @overload
      */
    virtual void FireOutputStart();

	/**
	@overload
	*/
	virtual void FireOutputDone();


protected:
    /**
      @overload
      */
    //virtual double Measure();

protected:
    struct ST_VALUE_TRACE{
        SydTime time;
        INT32   iHopperId;
        INT32   iAction;
    };

protected:

    vector<ST_VALUE_TRACE> m_vecTraces;
	vector<int> m_vecHopid;
};

#endif // MULTIVALUED_HOPPER_NODE_H
