#ifndef PORTFOLIO
#define PORTFOLIO

#include <string>
#include <map>
#include <Wt/Dbo/Dbo>
#include "../common/CommonUtils.h"

class Portfolio {
	std::string user;
	std::string code;
	int quantity;
	int totalcost;
	std::string status;
	std::string msg;
	err_code m_ec;

	std::map<std::string, int> getAllStocks(Wt::Dbo::Session& session) const;
public:
	Portfolio():user("guest"), code("Dummy"), quantity(0), totalcost(0){
	}
	Portfolio(const std::string& u);

	Portfolio(const std::string& u, const std::string& c, int q, int t):user(u), code(c), quantity(q), totalcost(t) {
	}
	int getQty() const{
		return quantity;
	}
	void setQty(int newQ){
		quantity = newQ;
	}
	void setTotalCost(int newTC) {
		totalcost = newTC;
	}
	int getTotalCost() const{
		return totalcost;
	}
	template<class Action>
	void persist(Action& a) {
		Wt::Dbo::field(a, user, "username");
		Wt::Dbo::field(a, code, "stockcode");
		Wt::Dbo::field(a, quantity, "quantity");
		Wt::Dbo::field(a, totalcost, "totalcost");
	};

	bool fetchPortfolioForUser(std::vector<std::tuple<std::string, int, int, double > >& vecResult);

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
		struct dbo_traits<Portfolio> : public dbo_default_traits {
			static const char *versionField() {
				return 0;
			}
		};
	}
}
*/
#endif
