#include "Trader.h"
#include "../common/CommonUtils.h";
#include <Wt/Dbo/backend/Sqlite3>

Trader::Trader(const std::string& n, const std::string& p, int b):name(n), pass(p), balance(b), msg("") {
}

bool Trader::registerTrader() {
	try {
		Wt::Dbo::backend::Sqlite3 sl(getSQLiteDBName());
		Wt::Dbo::Session session;
		session.setConnection(sl);

		session.mapClass<Trader>("trader");
		Wt::Dbo::Transaction trx(session);

		int count = session.query<int>("select count(1) from trader").where("username=?").bind(name);

		if (count == 0) {
			Trader* tr2 = new Trader(name, pass, 100000);
			Wt::Dbo::ptr<Trader> traderPtr = session.add(tr2);
			trx.commit();
			msg = "Successfully registered new user";
			return true;
		}
		else {
			msg = "Failed to register. User name already exists.";
			m_ec = user_already_exists;
			trx.commit();
			return false;
		}
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
