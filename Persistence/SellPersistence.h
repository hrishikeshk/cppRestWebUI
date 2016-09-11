#ifndef SELL_PERSISTENCE
#define SELL_PERSISTENCE

#include <string>
#include <Wt/Dbo/Dbo>
#include "../common/CommonUtils.h"

class SellPersistence {
	std::string user;
	std::string code;
	int quantity;
	int offer_price;

	std::string status;
	std::string sell_comment;

	std::string msg;
	err_code m_ec;

	bool verifyPortfolio(const std::string& user, const std::string& code, int qty, Wt::Dbo::Session& session);
	bool verifyStockCode(const std::string& code, int new_sale_price, Wt::Dbo::Session& session);

	bool updateBalanceCash(const std::string& user, int proceeds, Wt::Dbo::Session& session);
	bool updateSellerTransaction(const std::string& user, const std::string& code, int quantity, Wt::Dbo::Session& session);
	bool updateSellerPortfolio(const std::string& user, const std::string& code, int quantity, Wt::Dbo::Session& session);

public:
	SellPersistence():user("guest"), code("Dummy"), quantity(0), offer_price(0), sell_comment("Un-executed"), status("PENDING") {
	}
	SellPersistence(const std::string& u, const std::string& c, int qty, int of_prc);

	bool sell(std::tuple<int, std::string, std::string >& tupResult);

	std::string& getMsg() {
		return msg;
	}
	err_code getErrorCode() {
		return m_ec;
	}
};

#endif
