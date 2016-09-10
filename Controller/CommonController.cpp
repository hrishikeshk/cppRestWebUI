#include "CommonController.h"
#include "../Persistence/Trader.h"

OperationStatus CommonController::registerNewTrader(const std::string& user, const std::string& pass){
	OperationStatus op_stat;
	
	Trader tr(user, pass, INITIAL_BALANCE);
	op_stat.set_status(!(tr.registerTrader()));
	if (op_stat.failed()) {
		op_stat.set_error(tr.getMsg(), tr.getErrorCode());
	}
	return op_stat;
}
