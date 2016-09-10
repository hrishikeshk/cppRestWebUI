#ifndef COMMON_CONTROLLER
#define COMMON_CONTROLLER

#include <string>
#include <tuple>
#include "../common/CommonUtils.h"
#include "OperationStatus.h"

class CommonController{
public:
	int areValidCredentials(const std::string& user, const std::string& pass, OperationStatus& op_stat) const;

	int getLastSalePrice(const std::string& stockcode, OperationStatus& op_stat) const;

	OperationStatus registerNewTrader(const std::string& user, const std::string& pass) const;

	std::tuple<OperationStatus, int> getQuote(const std::string& user, const std::string& pass, const std::string& stockcode) const;

	std::tuple<OperationStatus, std::vector<std::tuple<std::string, int, int, bool > > >
		getTransactions(const std::string& user, const std::string& pass) const;

	std::tuple<OperationStatus, std::vector<std::tuple<std::string, int, int, double > > >
		getPortfolio(const std::string& user, const std::string& pass) const;
};

#endif
