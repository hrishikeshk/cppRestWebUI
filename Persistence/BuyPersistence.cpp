#include "BuyPersistence.h"
#include <vector>
#include "../common/CommonUtils.h"
#include <Wt/Dbo/backend/Sqlite3>
#include "Trader.h"
#include "Stock.h"
#include "Stock_Transaction.h"
#include "Portfolio.h"
#include <chrono>

using namespace std::chrono;

BuyPersistence::BuyPersistence(const std::string& u, const std::string& c, int qty, int of_prc):user(u),
											code(c), quantity(qty), offer_price(of_prc), status("PENDING"), msg("") {
}

bool BuyPersistence::verifyBalanceCash(const std::string& user, int cost, Wt::Dbo::Session& session) {
	Wt::Dbo::ptr<Trader> oneTrader = session.find<Trader>().where("username=?").bind(user);
	if (oneTrader.get()) {
		int current_balance = oneTrader->getBalance();
		if (cost > current_balance) {
			return false;
		}
		oneTrader.modify()->setBalance(current_balance - cost);
		oneTrader.flush();
	}
	return true;
}

bool BuyPersistence::verifyStockCode(const std::string& code, int new_sale_price, Wt::Dbo::Session& session) {
	Wt::Dbo::ptr<Stock> oneStock = session.find<Stock>().where("stockcode=?").bind(code);
	if (oneStock.get()) {
		oneStock.modify()->setLastSalePrice(new_sale_price);
		oneStock.flush();
		return true;
	}
	return false;
}

bool BuyPersistence::updateBuyerTransaction(const std::string& user, const std::string& code, int quantity, Wt::Dbo::Session& session) {
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

bool BuyPersistence::updateBuyerPortfolio(const std::string& user, const std::string& code, int quantity, int cost, Wt::Dbo::Session& session) {
	try {
		Wt::Dbo::ptr<Portfolio> pf = session.find<Portfolio>().where("username=? and stockcode=?").bind(user).bind(code);
		if (pf.get()) {
			int currQty = pf->getQty();
			int currCost = pf->getTotalCost();
			pf.modify()->setQty(currQty + quantity);
			pf.modify()->setTotalCost(currCost + cost);

			pf.flush();
		}
		else {
			Portfolio* pf = new Portfolio(user, code, quantity, cost);
			Wt::Dbo::ptr<Portfolio> spf = session.add(pf);
		}
	}
	catch (...) {
		return false;
	}
	return true;
}

bool BuyPersistence::buy(std::tuple<int, std::string, std::string>& tupResult) {
	try {
		Wt::Dbo::backend::Sqlite3 sl(getSQLiteDBName());
		Wt::Dbo::Session session;
		session.setConnection(sl);

		session.mapClass<Trader>("trader");
		session.mapClass<Stock>("stock");
		session.mapClass<Stock_Transaction>("stock_transaction");
		session.mapClass<Portfolio>("portfolio");

		Wt::Dbo::Transaction trx(session);
		bool has_balance = true, valid_stock = false;
		has_balance = verifyBalanceCash(user, quantity * offer_price, session);
		valid_stock = verifyStockCode(code, offer_price, session);
		if(has_balance && valid_stock){
			bool final = updateBuyerTransaction(user, code, quantity, session) &&
						 updateBuyerPortfolio(user, code, quantity, quantity * offer_price, session);
			if (!final) {
				msg = "Failed to buy : Error updating persistence. returning without commit.";
				m_ec = unknown_error;
				return false;
			}
		}
		else {
			if (!valid_stock) {
				msg = "Failed to buy : Bad stock name";
				m_ec = bad_request_stockcode;
			}
			else if (!has_balance) {
				msg = "Failed to buy : Insufficient balance funds";
				m_ec = insufficient_funds_balance;
			}
			return false;
		}
		
		msg = "Successful buy";
		tupResult = std::make_tuple(offer_price, std::string("EXECUTED"), std::string("Bought required stock quantity to portfolio"));
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
