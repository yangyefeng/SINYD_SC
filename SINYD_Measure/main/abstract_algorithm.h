
#ifndef ABSTRACT_ALGORITHM_H
#define ABSTRACT_ALGORITHM_H

#include <vector>
using namespace std;
#include "../Data/Include/Operation_conf.h"
#include "../Include/DebugLogger.h"
#include "data_algorithm/data_algorithm.h"

//class AbstractAlgorithm
//{
//public:
//	AbstractAlgorithm();
//	~AbstractAlgorithm();
//
//	virtual DataAlgorithm* createAlgorithm(EN_CALC_METHOD_TYPE type) = 0;
//};

class AlgorithmFactory {//:public AbstractAlgorithm {

public:
	AlgorithmFactory();
	~AlgorithmFactory();

	DataAlgorithm* createAlgorithm(EN_CALC_METHOD_TYPE type);
};

#endif // ABSTRACT_ALGORITHM_H
