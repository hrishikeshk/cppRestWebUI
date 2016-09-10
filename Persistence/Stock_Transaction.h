#ifndef STOCK_TRANSACTION
#define STOCK_TRANSACTION

#include <string>
#include <Wt/Dbo/Dbo>
#include "../common/CommonUtils.h"

class Stock_Transaction {
	std::string user;
	std::string code;
	int quantity;
	int datetime;
	std::string status;
	std::string msg;
	err_code m_ec;
public:
	Stock_Transaction():user("guest"), code("Dummy"), quantity(0), datetime(4), status("PENDING") {
	}
	Stock_Transaction(const std::string& u);

	template<class Action>
	void persist(Action& a) {
		Wt::Dbo::field(a, user, "username");
		Wt::Dbo::field(a, code, "stockcode");
		Wt::Dbo::field(a, quantity, "quantity");
		Wt::Dbo::field(a, datetime, "datetime");
		Wt::Dbo::field(a, status, "status");
	};

	bool fetchTransactionsForUser(std::vector<std::tuple<std::string, int, int, bool > >& vecResult);

	std::string& getMsg() {
		return msg;
	}
	err_code getErrorCode() {
		return m_ec;
	}
};

namespace Wt {
	namespace Dbo {
		template<>
		struct dbo_traits<Stock_Transaction> : public dbo_default_traits {
			static const char *versionField() {
				return 0;
			}
		};
	}
}

#endif
