#include "Stock.h"
#include "../common/CommonUtils.h";
#include <Wt/Dbo/backend/Sqlite3>

Stock::Stock(const std::string& c, int p):code(c), lastSalePrice(p), msg("") {
}

bool Stock::getLastSalePrice() {
	try {
		Wt::Dbo::backend::Sqlite3 sl(getSQLiteDBName());
		Wt::Dbo::Session session;
		session.setConnection(sl);

		session.mapClass<Stock>("stock");
		Wt::Dbo::Transaction trx(session);

		Wt::Dbo::collection<Wt::Dbo::ptr<Stock> > matchingStocks = session.find<Stock>().where("stockcode=?").bind(code);
		for (Wt::Dbo::collection<Wt::Dbo::ptr<Stock> >::const_iterator i = matchingStocks.begin();
			  i != matchingStocks.end(); ++i
			) {

			lastSalePrice = (*i)->lastSalePrice;
			break;
		}

		msg = "Retrieved matching stocks";
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
