#ifndef BUY_PERSISTENCE
#define BUY_PERSISTENCE

#include <string>
#include <Wt/Dbo/Dbo>
#include "../common/CommonUtils.h"

class BuyPersistence {
	std::string user;
	std::string code;
	int quantity;
	int offer_price;

	std::string status;
	std::string buy_comment;

	std::string msg;
	err_code m_ec;

//	Wt::Dbo::ptr<Trader> verifyBalanceCash(const std::string& user, int cost, Wt::Dbo::Session& session, bool& result);
	//Wt::Dbo::ptr<Stock> verifyStockCode(const std::string& code, Wt::Dbo::Session& session, bool& valid_stock);

	bool verifyBalanceCash(const std::string& user, int cost, Wt::Dbo::Session& session);
	bool verifyStockCode(const std::string& code, int new_sale_price, Wt::Dbo::Session& session);

	bool updateBuyerTransaction(const std::string& user, const std::string& code, int quantity, Wt::Dbo::Session& session);
	bool updateBuyerPortfolio(const std::string& user, const std::string& code, int quantity, int cost, Wt::Dbo::Session& session);

public:
	BuyPersistence():user("guest"), code("Dummy"), quantity(0), offer_price(0), buy_comment("Un-executed"), status("PENDING") {
	}
	BuyPersistence(const std::string& u, const std::string& c, int qty, int of_prc);

	bool buy(std::tuple<int, std::string, std::string >& tupResult);

	std::string& getMsg() {
		return msg;
	}
	err_code getErrorCode() {
		return m_ec;
	}
};

#endif
