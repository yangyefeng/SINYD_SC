#include "data_node.h"

DataNode::DataNode()
{
    m_pDataProvider=NULL;
}

DataNode::~DataNode(){
    m_pDataProvider=NULL;
}

void DataNode::Initialize(DataProvider *pDataProvider)
{
    m_pDataProvider=pDataProvider;
}

void DataNode::Execute(){

}


void DataNode::Release(){

 }

DataProvider* DataNode::GetDataProvider(){
    return m_pDataProvider;
}
