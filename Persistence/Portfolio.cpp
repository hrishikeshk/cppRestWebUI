#include "Portfolio.h"
#include <vector>
#include "../common/CommonUtils.h"
#include <Wt/Dbo/backend/Sqlite3>
#include "Stock.h"

Portfolio::Portfolio(const std::string& u):user(u), code("Dummy"), quantity(0), totalcost(0), msg("") {
}

bool Portfolio::fetchPortfolioForUser(std::vector<std::tuple<std::string, int, int, double > >& vecResult) {
	try {
		Wt::Dbo::backend::Sqlite3 sl(getSQLiteDBName());
		Wt::Dbo::Session session;
		session.setConnection(sl);

		session.mapClass<Portfolio>("portfolio");
		session.mapClass<Stock>("stock");
		Wt::Dbo::Transaction trx(session);
		bool status_flag;
		Wt::Dbo::collection<Wt::Dbo::ptr<Portfolio> > matchingPfes = session.find<Portfolio>().where("username=?").bind(user);
		for (Wt::Dbo::collection<Wt::Dbo::ptr<Portfolio> >::const_iterator i = matchingPfes.begin();
			  i != matchingPfes.end(); ++i
			) {
			Wt::Dbo::ptr<Stock> oneStock = session.find<Stock>().where("stockcode=?").bind((*i)->code);
			vecResult.emplace_back((*i)->code, (*i)->quantity, (*i)->totalcost, ((*i)->quantity) * oneStock->result());
		}

		msg = "Retrieved matching portfolio entries";
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
