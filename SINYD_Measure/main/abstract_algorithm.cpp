#include "abstract_algorithm.h"

//AbstractAlgorithm::AbstractAlgorithm()
//{
//}
//
//AbstractAlgorithm::~AbstractAlgorithm()
//{
//}

AlgorithmFactory::AlgorithmFactory()
{
}

AlgorithmFactory::~AlgorithmFactory()
{
}
//工厂模式
DataAlgorithm* AlgorithmFactory::createAlgorithm(EN_CALC_METHOD_TYPE type)
{
	DataAlgorithm* pDAlgorithm = NULL;
	switch (type)
	{
	case CALC_METHOD_FIRST:
		pDAlgorithm = new FirstDataAlgorithm();
		break;
	case CALC_METHOD_END:
		pDAlgorithm = new EndDataAlgorithm();
		break;
	case CALC_METHOD_MAX:
		pDAlgorithm = new MaxDataAlgorithm();
		break;
	case CALC_METHOD_MIN:
		pDAlgorithm = new MinDataAlgorithm();
		break;
	case CALC_METHOD_AVERAGE:
		pDAlgorithm = new AverageDataAlgorithm();
		break;
	default:
		break;
	}

	return pDAlgorithm;
}
