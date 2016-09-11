#ifndef STOCK
#define STOCK

#include <string>
#include <Wt/Dbo/Dbo>
#include "../common/CommonUtils.h"

class Stock {
	std::string code;
	int lastSalePrice;
	std::string msg;
	err_code m_ec;
public:
	Stock():code("Dummy"), lastSalePrice(-1) {
	}
	Stock(const std::string& c, int p);
	std::string getCode()const {
		return code;
	}
	template<class Action>
	void persist(Action& a) {
		Wt::Dbo::field(a, code, "stockcode");
		Wt::Dbo::field(a, lastSalePrice, "lastsaleprice");
	};

	bool getLastSalePrice();

	int result() const{
		return lastSalePrice;
	}

	std::string& getMsg() {
		return msg;
	}
	err_code getErrorCode() {
		return m_ec;
	}
	void setLastSalePrice(int new_price) {
		lastSalePrice = new_price;
	}
};
/*
namespace Wt {
	namespace Dbo {
		template<>
		struct dbo_traits<Stock> : public dbo_default_traits {
			static const char *versionField() {
				return 0;
			}
		};
	}
}
*/
#endif
