#ifndef TRADER
#define TRADER

#include <string>
#include <Wt/Dbo/Dbo>
#include "../common/CommonUtils.h"

class Trader {
	std::string name;
	std::string pass;
	int balance;
	std::string msg;
	err_code m_ec;
public:
	Trader():name("guest"), pass("guest"), balance(0) {
	}
	Trader(const std::string& n, const std::string& p, int b);

	template<class Action>
	void persist(Action& a) {
		Wt::Dbo::field(a, name, "username");
		Wt::Dbo::field(a, pass, "password");
		Wt::Dbo::field(a, balance, "balancecash");
	};

	int getBalance() const {
		return balance;
	}
	void setBalance(int new_balance) {
		balance = new_balance;
	}

	bool registerTrader();

	bool verifyCredentials();

	std::string& getMsg() {
		return msg;
	}
	err_code getErrorCode() {
		return m_ec;
	}
};
/*
namespace Wt {
	namespace Dbo {
		template<>
		struct dbo_traits<Trader> : public dbo_default_traits {
			static const char *versionField() {
				return 0;
			}
		};
	}
}
*/

#endif
