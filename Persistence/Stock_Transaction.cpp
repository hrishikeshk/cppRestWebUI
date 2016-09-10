#include "Stock_Transaction.h"
#include <vector>
#include "../common/CommonUtils.h"
#include <Wt/Dbo/backend/Sqlite3>

Stock_Transaction::Stock_Transaction(const std::string& u):user(u), code("Dummy"), quantity(0), datetime(4), status("PENDING"), msg("") {
}

bool Stock_Transaction::fetchTransactionsForUser(std::vector<std::tuple<std::string, int, int, bool > >& vecResult) {
	try {
		Wt::Dbo::backend::Sqlite3 sl(getSQLiteDBName());
		Wt::Dbo::Session session;
		session.setConnection(sl);

		session.mapClass<Stock_Transaction>("stock_transaction");
		Wt::Dbo::Transaction trx(session);
		bool status_flag;
		Wt::Dbo::collection<Wt::Dbo::ptr<Stock_Transaction> > matchingTrxs = session.find<Stock_Transaction>().where("username=?").bind(user);
		for (Wt::Dbo::collection<Wt::Dbo::ptr<Stock_Transaction> >::const_iterator i = matchingTrxs.begin();
			  i != matchingTrxs.end(); ++i
			) {
			if ((*i)->status == std::string("PENDING")) {
				status_flag = false;
			}
			else {
				status_flag = true;
			}
			vecResult.emplace_back((*i)->code, (*i)->quantity, (*i)->datetime, status_flag);
		}
		msg = "Retrieved matching transactions";
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
