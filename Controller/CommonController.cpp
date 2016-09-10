#include "CommonController.h"
#include "../Persistence/Trader.h"
#include "../Persistence/Stock.h"
#include "../Persistence/Stock_Transaction.h"
#include "../Persistence/Portfolio.h"

//#include <ppltasks.h>
//#include <array>

//using namespace concurrency;

OperationStatus CommonController::registerNewTrader(const std::string& user, const std::string& pass) const{
	OperationStatus op_stat;
	
	Trader tr(user, pass, INITIAL_BALANCE);
	op_stat.set_status(!(tr.registerTrader()));
	if (op_stat.failed()) {
		op_stat.set_error(tr.getMsg(), tr.getErrorCode());
	}
	return op_stat;
}

int CommonController::areValidCredentials(const std::string& user, const std::string& pass, OperationStatus& op_stat) const {
	
	Trader tr(user, pass, 0);
	op_stat.set_status(!tr.verifyCredentials());
	if (op_stat.failed()) {
		return 0;
	}
	return 1;
}

int CommonController::getLastSalePrice(const std::string& stockcode, OperationStatus& op_stat) const {

	Stock st(stockcode, -1);
	op_stat.set_status(!st.getLastSalePrice());
	if (op_stat.failed() || st.result() == -1) {
		op_stat.set_error(st.getMsg(), st.getErrorCode());
		return -1;
	}
	return st.result();
}

std::tuple<OperationStatus, int> CommonController::getQuote(const std::string& user, 
															const std::string& pass, 
															const std::string& stockcode) const{
	
	OperationStatus op_stat;
	
	/*
	const CommonController* pthis = this;
	std::array<concurrency::task<int>, 2> task_arr = {
		create_task([=](const std::string& user, const std::string& pass) ->int {
										 return this->areValidCredentials(user, pass);
	                                 }),
		create_task([=](const std::string& stockcode) ->int {
										 return this->getLastSalePrice(stockcode);
									 })
	};
	int lastSalePrice = -1;

	auto joinTask = concurrency::when_all( std::begin(task_arr), std::end(task_arr)).
					then(
						[=](std::vector<int> results) {
							if (results[0] == 1) {
								lastSalePrice = results[1];
								op_stat.set_status(0);
							}
							else {
								op_stat.set_status(1);
							}
						}
					);

	joinTask.wait();
	*/
	int lastSalePrice = -1;
	int creds_valid = areValidCredentials(user, pass, op_stat);
	if (creds_valid == 1) {
		lastSalePrice = getLastSalePrice(stockcode, op_stat);
		if (op_stat.failed() || lastSalePrice == -1) {
			op_stat.set_status(1);
			op_stat.set_error("Failed to find Stock with code", bad_request_stockcode);
		}
		else {
			op_stat.set_status(0);
		}
	}
	else {
		op_stat.set_status(1);
	}
	return std::make_tuple(op_stat, lastSalePrice);
}

std::tuple<OperationStatus, std::vector<std::tuple<std::string, int, int, bool > > >
	CommonController::getTransactions(const std::string& user, const std::string& pass) const {

	OperationStatus op_stat;
	int creds_valid = areValidCredentials(user, pass, op_stat);
	if (creds_valid == 1) {
		Stock_Transaction trx(user);
		std::vector<std::tuple<std::string, int, int, bool > > retVec;
		op_stat.set_status(!trx.fetchTransactionsForUser(retVec));
		if (op_stat.failed()) {
			op_stat.set_error(trx.getMsg(), trx.getErrorCode());
		}
		return std::make_tuple(op_stat, retVec);
	}
	else {
		op_stat.set_status(1);
		std::vector<std::tuple<std::string, int, int, bool > > retVec;
		return std::make_tuple(op_stat, retVec);
	}
}

std::tuple<OperationStatus, std::vector<std::tuple<std::string, int, int, double > > >
CommonController::getPortfolio(const std::string& user, const std::string& pass) const {

	OperationStatus op_stat;
	int creds_valid = areValidCredentials(user, pass, op_stat);
	if (creds_valid == 1) {
		Portfolio pf(user);
		std::vector<std::tuple<std::string, int, int, double > > retVec;
		op_stat.set_status(!pf.fetchPortfolioForUser(retVec));
		if (op_stat.failed()) {
			op_stat.set_error(pf.getMsg(), pf.getErrorCode());
			return std::make_tuple(op_stat, retVec);
		}
		return std::make_tuple(op_stat, retVec);
	}
	else {
		op_stat.set_status(1);
		std::vector<std::tuple<std::string, int, int, double > > retVec;
		return std::make_tuple(op_stat, retVec);
	}
}
