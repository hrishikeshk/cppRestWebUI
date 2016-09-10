#ifndef COMMON_CONTROLLER
#define COMMON_CONTROLLER

#include <string>
#include "../common/CommonUtils.h"
#include "OperationStatus.h"

class CommonController{
	public:
	  OperationStatus registerNewTrader(const std::string& user, const std::string& pass);
};

#endif
