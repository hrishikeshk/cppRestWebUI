#include "SellPersistence.h"
#include <vector>
#include "../common/CommonUtils.h"
#include <Wt/Dbo/backend/Sqlite3>
#include "Trader.h"
#include "Stock.h"
#include "Stock_Transaction.h"
#include "Portfolio.h"
#include <chrono>

using namespace std::chrono;

SellPersistence::SellPersistence(const std::string& u, const std::string& c, int qty, int of_prc):user(u),
											code(c), quantity(qty), offer_price(of_prc), status("PENDING"), msg("") {
}

bool SellPersistence::updateBalanceCash(const std::string& user, int proceeds, Wt::Dbo::Session& session) {
	Wt::Dbo::ptr<Trader> oneTrader = session.find<Trader>().where("username=?").bind(user);
	if (oneTrader.get()) {
		int current_balance = oneTrader->getBalance();
		oneTrader.modify()->setBalance(current_balance + proceeds);
		oneTrader.flush();
	}
	return true;
}

bool SellPersistence::verifyPortfolio(const std::string& user, const std::string& code, int qty, Wt::Dbo::Session& session) {
	try {
		Wt::Dbo::ptr<Portfolio> oneStockQty = session.find<Portfolio>().where("stockcode=? and username=?").bind(code).bind(user);
		if (oneStockQty.get()) {
			return (oneStockQty->getQty() >= qty);
		}
		else {
			return false;
		}
	}
	catch (...) {
		return false;
	}
	return true;
}

bool SellPersistence::verifyStockCode(const std::string& code, int new_sale_price, Wt::Dbo::Session& session) {
	Wt::Dbo::ptr<Stock> oneStock = session.find<Stock>().where("stockcode=?").bind(code);
	if (oneStock.get()) {
		oneStock.modify()->setLastSalePrice(new_sale_price);
		oneStock.flush();
		return true;
	}
	return false;
}

bool SellPersistence::updateSellerTransaction(const std::string& user, const std::string& code, int quantity, Wt::Dbo::Session& session) {
	try {
		milliseconds ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch() );
		Stock_Transaction* tr = new Stock_Transaction(user, code, quantity, ms.count() / 1000, "EXECUTED");
		Wt::Dbo::ptr<Stock_Transaction> stPtr = session.add(tr);
	}
	catch (...) {
		return false;
	}
	return true;
}

bool SellPersistence::updateSellerPortfolio(const std::string& user, const std::string& code, int quantity, Wt::Dbo::Session& session) {
	try {
		Wt::Dbo::ptr<Portfolio> oneStockPf = session.find<Portfolio>().where("stockcode=? and username=?").bind(code).bind(user);
		if (oneStockPf.get()) {
			int currentQty = oneStockPf->getQty();
			int currentCost = oneStockPf->getTotalCost();
			if (currentQty >= quantity) {
				if (currentQty - quantity > 0) {
					oneStockPf.modify()->setQty(currentQty - quantity);
					oneStockPf.modify()->setTotalCost((currentCost / currentQty) * (currentQty - quantity));
					oneStockPf.flush();
				}
				else {
					oneStockPf.remove();
				}
			}
			else
				return false; //// never expected to hit this.
		}
		else {
			return false;
		}
	}
	catch (...) {
		return false;
	}
	return true;
}

bool SellPersistence::sell(std::tuple<int, std::string, std::string>& tupResult) {
	try {
		Wt::Dbo::backend::Sqlite3 sl(getSQLiteDBName());
		Wt::Dbo::Session session;
		session.setConnection(sl);

		session.mapClass<Trader>("trader");
		session.mapClass<Stock>("stock");
		session.mapClass<Stock_Transaction>("stock_transaction");
		session.mapClass<Portfolio>("portfolio");

		Wt::Dbo::Transaction trx(session);
		bool has_pf, valid_stock;
		has_pf = verifyPortfolio(user, code, quantity, session);
		valid_stock = verifyStockCode(code, offer_price, session);
		if(has_pf && valid_stock){
			bool final = updateBalanceCash(user, quantity * offer_price, session) &&
						 updateSellerTransaction(user, code, quantity, session) &&
						 updateSellerPortfolio(user, code, quantity, session);
			if (!final) {
				msg = "Failed to sell : Error updating persistence. returning without commit.";
				m_ec = unknown_error;
				return false;
			}
		}
		else {
			if (!valid_stock) {
				msg = "Failed to sell : Bad stock name";
				m_ec = bad_request_stockcode;
			}
			else if (!has_pf) {
				msg = "Failed to sell : Insufficient portfolio";
				m_ec = insufficient_portfolio;
			}
			return false;
		}
		
		msg = "Successful sell";
		tupResult = std::make_tuple(offer_price, std::string("EXECUTED"), std::string("Sold required stock quantity from portfolio"));
		trx.commit();
		return true;
	}
	catch (std::exception& e) {
		msg = e.what();
		m_ec = unknown_error;
		return false;
	}
	catch (...) {
		msg = "Unknown error";
		m_ec = unknown_error;
		return false;
	}
}
