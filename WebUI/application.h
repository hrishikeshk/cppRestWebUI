#ifndef APPLICATION_WEB_UI
#define APPLICATION_WEB_UI

#pragma warning(disable:4996)

#include <string>
#include <Wt/WApplication>
#include <Wt/WLogger>

using namespace Wt;

class Wt::WLineEdit;
class Wt::WText;
class Wt::WContainerWidget;

class MarketApplication : public WApplication {
public:
	MarketApplication(const WEnvironment& env);

private:
	WLineEdit* nameEdit_;
	WLineEdit* passEdit_;
	WText* message_;
	WLineEdit* stockCode;

	WLineEdit* bsc;
	WLineEdit* bqt;
	WLineEdit* bpr;

	WLineEdit* ssc;
	WLineEdit* sqt;
	WLineEdit* spr;

	WContainerWidget* center;

	std::string user;
	std::string pass;

	bool loginDone;

	void showLogin();
	void loginHandler();

	void showLandingPage();

	void logout() {
		loginDone = false;
		user = "";
		pass = "";
		quit();
	}

	void handleQuote();

	void handlePf();

	void handleTrx();

	void handleBuy();

	void handleSell();
};

int ui_landing(int argc, char **argv);

#endif
