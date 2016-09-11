#pragma warning(disable:4996)

#include <tuple>
#include <functional>
#include <iostream>
//#include <chrono>
//#include <ctime>

#include <Wt/WServer>

#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WBorderLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>
#include <Wt/WToolBar>
#include <Wt/WTable>
#include <Wt/WTableCell>

#include "../REST_Impl/Registration.h"
#include "../REST_Impl/Quote.h"
#include "../REST_Impl/Transactions.h"
#include "../REST_Impl/PortfolioList.h"
#include "../REST_Impl/Buy.h"
#include "../REST_Impl/Sell.h"
#include "application.h"

#include "../common/CommonUtils.h"
#include "../Controller/CommonController.h"
#include "../Controller/OperationStatus.h"

//using namespace std::chrono;

MarketApplication::MarketApplication(const WEnvironment& env)
	: WApplication(env), user(""), pass(""), loginDone(false){

	showLogin();
}

void MarketApplication::showLogin(){

	setTitle("Stock Market Application : Trader Login");

	WContainerWidget* loginBoxContainer = new WContainerWidget();
	loginBoxContainer->resize(200, 200);

	WVBoxLayout* outerVB = new WVBoxLayout();
	loginBoxContainer->setLayout(outerVB);

	WHBoxLayout* userRow = new WHBoxLayout();
	
	WText* userLabel = new WText("User name");
	nameEdit_ = new WLineEdit();
	nameEdit_->setFocus();
	userRow->addWidget(userLabel);
	userRow->addWidget(nameEdit_);

	WHBoxLayout* passRow = new WHBoxLayout();
	
	WText* passLabel = new WText("Password");
	passEdit_ = new WLineEdit();
	passRow->addWidget(passLabel);
	passRow->addWidget(passEdit_);

	WHBoxLayout* buttonRow = new WHBoxLayout();

	WPushButton *loginButton = new WPushButton("Login");
	WPushButton *clearButton = new WPushButton("Clear");
	WPushButton *regButton = new WPushButton("Register");

	buttonRow->addWidget(loginButton);
	buttonRow->addWidget(clearButton);
	buttonRow->addWidget(regButton);
	
	outerVB->addLayout(userRow);
	outerVB->addLayout(passRow);
	outerVB->addLayout(buttonRow);

	WHBoxLayout* reg_msg_row = new WHBoxLayout();

	message_ = new WText("Login with credentials");
	reg_msg_row->addWidget(message_);

	outerVB->addLayout(reg_msg_row);

	WContainerWidget* vertDummy = new WContainerWidget();
	vertDummy->resize(275, 275);
	root()->addWidget(vertDummy);
	root()->addWidget(new WBreak());
	root()->addWidget(loginBoxContainer);

	root()->setContentAlignment(Wt::AlignCenter);

	auto bound_login = std::bind(&MarketApplication::loginHandler, this);
	loginButton->clicked().connect(bound_login);
	passEdit_->enterPressed().connect(bound_login);

	nameEdit_->enterPressed().connect(std::bind([=]() {
												passEdit_->setFocus();
											}
									 ));
	clearButton->clicked().connect(std::bind([=]() {
		nameEdit_->setText("");
		passEdit_->setText("");
	})
	);

	regButton->clicked().connect(std::bind([=]() {
		CommonController cc;
		auto user = nameEdit_->text();
		auto pass = passEdit_->text();
		OperationStatus op_stat = cc.registerNewTrader(user.toUTF8(), pass.toUTF8());
		if (op_stat.failed()) {
			std::string err = op_stat.get_exception_msg();
			message_->setText(std::string("Registration failed: ") + err);
		}
		else {
			root()->clear();
			showLandingPage();
		}
	})
	);
}

void MarketApplication::loginHandler() {
	CommonController cc;
	OperationStatus op_stat;
	auto username = nameEdit_->text();
	auto password = passEdit_->text();

	if (cc.areValidCredentials(username.toUTF8(), password.toUTF8(), op_stat)) {
		loginDone = true;
		user = username.toUTF8();
		pass = password.toUTF8();
		root()->clear();
		showLandingPage();
	}
	else {
		loginDone = false;
		message_->setText("Login failed. Try again.");
	}
}

void MarketApplication::showLandingPage() {
	WContainerWidget *container = new Wt::WContainerWidget();
	container->resize(800, 800);

	WBorderLayout *layout = new Wt::WBorderLayout();
	container->setLayout(layout);

	WText *item = new Wt::WText("Main Landing page : Perform operations using toolbar and center form");
	WPushButton* logoutButton = new WPushButton("Sign out");
		
	WContainerWidget* northItem = new WContainerWidget();
	WHBoxLayout* northBox = new WHBoxLayout();
	northBox->addWidget(item);
	northBox->addWidget(logoutButton);
	northItem->setLayout(northBox);

	layout->addWidget(northItem, Wt::WBorderLayout::North);

	Wt::WContainerWidget *tbContainer = new Wt::WContainerWidget();
	Wt::WToolBar *toolBar = new Wt::WToolBar(tbContainer);

	WPushButton* quoteButton = new WPushButton("Get Quote");
	WPushButton* pfButton = new WPushButton("View Portfolio");
	WPushButton* trxButton = new WPushButton("View Transactions");
	WPushButton* buyButton = new WPushButton("Buy");
	WPushButton* sellButton = new WPushButton("Sell");

	toolBar->addButton(quoteButton);
	toolBar->addSeparator();

	toolBar->addButton(pfButton);
	toolBar->addSeparator();
	
	toolBar->addButton(trxButton);
	toolBar->addSeparator();

	toolBar->addButton(buyButton);
	toolBar->addSeparator();

	toolBar->addButton(sellButton);

	toolBar->setOrientation(Horizontal);

	tbContainer->resize(100, 600);

	layout->addWidget(tbContainer, Wt::WBorderLayout::West);

	center = new WContainerWidget();
	WText* citem = new Wt::WText("Work space in Center");
	center->addWidget(citem);

	layout->addWidget(center, Wt::WBorderLayout::Center);

	root()->addWidget(container);

	logoutButton->clicked().connect(std::bind([=]() {
		logout();
	}));

	quoteButton->clicked().connect(std::bind([=]() {
		center->clear();
		handleQuote();
	}));

	pfButton->clicked().connect(std::bind([=]() {
		center->clear();
		handlePf();
	}));

	trxButton->clicked().connect(std::bind([=]() {
		center->clear();
		handleTrx();
	}));

	buyButton->clicked().connect(std::bind([=]() {
		center->clear();
		handleBuy();
	}));

	sellButton->clicked().connect(std::bind([=]() {
		center->clear();
		handleSell();
	}));
}

void MarketApplication::handleBuy() {
	WContainerWidget* buyFormCont = new WContainerWidget();
	center->addWidget(buyFormCont);

	WText* blsc = new WText("Stock code");
	WText* blqt = new WText("Quantity");
	WText* blpr = new WText("Price");

	bsc = new WLineEdit();
	bqt = new WLineEdit();
	bpr = new WLineEdit();

	buyFormCont->addWidget(blsc);
	buyFormCont->addWidget(bsc);
	buyFormCont->addWidget(new WBreak());

	buyFormCont->addWidget(blqt);
	buyFormCont->addWidget(bqt);
	buyFormCont->addWidget(new WBreak());

	buyFormCont->addWidget(blpr);
	buyFormCont->addWidget(bpr);
	buyFormCont->addWidget(new WBreak());

	WPushButton* bs = new WPushButton("Buy Stock");
	buyFormCont->addWidget(bs);

	WPushButton* cl = new WPushButton("Clear");
	buyFormCont->addWidget(cl);

	buyFormCont->addWidget(bs);
	buyFormCont->addWidget(cl);

	buyFormCont->resize(500, 700);

	cl->clicked().connect(std::bind([=]() {
		bsc->setText("");
		bqt->setText("");
		bpr->setText("");
	}));

	bs->clicked().connect(std::bind([=]() {
		std::string stockcode = bsc->text().toUTF8();
		std::string quantity = bqt->text().toUTF8();
		std::string price = bpr->text().toUTF8();
		
		char* end = nullptr;
		int qty = strtol(quantity.c_str(), &end, 10);
		if (*end || qty < 1) {
			center->clear();
			center->addWidget(new WText("Failed to buy, invalid number in quantity"));
			return;
		}

		end = nullptr;
		int of_price = strtol(price.c_str(), &end, 10);
		if (*end || qty < 1) {
			center->clear();
			center->addWidget(new WText("Failed to buy, invalid number in price"));
			return;
		}
		CommonController cc;
		std::tuple<OperationStatus, std::tuple<int, std::string, std::string > >
			bsresult = cc.buy(user, pass, stockcode, qty, of_price);

		OperationStatus op_stat = std::get<0>(bsresult);
		if (op_stat.failed()) {
			center->clear();
			center->addWidget(new WText("Failed to buy: " + op_stat.get_exception_msg()));
		}
		else {
			center->clear();
			auto tup = std::get<1>(bsresult);
			center->addWidget(new WText("Bought stock successfully!") );
		}
	}));
}

void MarketApplication::handleSell() {
	WContainerWidget* sellFormCont = new WContainerWidget();
	center->addWidget(sellFormCont);

	WText* slsc = new WText("Stock code");
	WText* slqt = new WText("Quantity");
	WText* slpr = new WText("Price");

	ssc = new WLineEdit();
	sqt = new WLineEdit();
	spr = new WLineEdit();

	sellFormCont->addWidget(slsc);
	sellFormCont->addWidget(ssc);
	sellFormCont->addWidget(new WBreak());

	sellFormCont->addWidget(slqt);
	sellFormCont->addWidget(sqt);
	sellFormCont->addWidget(new WBreak());

	sellFormCont->addWidget(slpr);
	sellFormCont->addWidget(spr);
	sellFormCont->addWidget(new WBreak());

	WPushButton* ss = new WPushButton("Sell Stock");
	sellFormCont->addWidget(ss);

	WPushButton* cl = new WPushButton("Clear");
	sellFormCont->addWidget(cl);

	sellFormCont->addWidget(ss);
	sellFormCont->addWidget(cl);

	sellFormCont->resize(500, 700);

	cl->clicked().connect(std::bind([=]() {
		ssc->setText("");
		sqt->setText("");
		spr->setText("");
	}));

	ss->clicked().connect(std::bind([=]() {
		std::string stockcode = ssc->text().toUTF8();
		std::string quantity = sqt->text().toUTF8();
		std::string price = spr->text().toUTF8();

		char* end = nullptr;
		int qty = strtol(quantity.c_str(), &end, 10);
		if (*end || qty < 1) {
			center->clear();
			center->addWidget(new WText("Failed to buy, invalid number in quantity"));
			return;
		}

		end = nullptr;
		int of_price = strtol(price.c_str(), &end, 10);
		if (*end || qty < 1) {
			center->clear();
			center->addWidget(new WText("Failed to buy, invalid number in price"));
			return;
		}
		CommonController cc;
		std::tuple<OperationStatus, std::tuple<int, std::string, std::string > >
			ssresult = cc.sell(user, pass, stockcode, qty, of_price);

		OperationStatus op_stat = std::get<0>(ssresult);
		if (op_stat.failed()) {
			center->clear();
			center->addWidget(new WText("Failed to sell: " + op_stat.get_exception_msg()));
		}
		else {
			center->clear();
			auto tup = std::get<1>(ssresult);
			center->addWidget(new WText("Sold stock successfully!") );
		}
	}));
}

void MarketApplication::handleQuote() {
	WText* quoteLabel = new WText("enter stock code to view");
	stockCode = new WLineEdit();
	WPushButton* seeQuote = new WPushButton("See Quote");

	WHBoxLayout* hbq = new WHBoxLayout();
	hbq->addWidget(quoteLabel);
	hbq->addWidget(stockCode);
	hbq->addWidget(seeQuote);

	WContainerWidget* smallQ = new WContainerWidget();
	smallQ->setLayout(hbq);
	smallQ->resize(400, 50);

	center->addWidget(smallQ);
	center->setContentAlignment(Wt::AlignCenter);

	seeQuote->clicked().connect(std::bind([=]() {
		std::string sc = stockCode->text().toUTF8();
		CommonController cc;
		std::tuple<OperationStatus, int> quoteResult = cc.getQuote(user, pass, sc);
		OperationStatus op_stat = std::get<0>(quoteResult);
		if (op_stat.failed()) {
			center->addWidget(new WText("Failed to fetch Quote: " + op_stat.get_exception_msg()));
		}
		else {
			center->clear();
			center->addWidget(new WText("Quote for stock code: " + sc + 
								" = " + std::to_string(std::get<1>(quoteResult))));
		}
	}));
}

void MarketApplication::handlePf() {
	CommonController cc;
	std::tuple<OperationStatus, std::vector<std::tuple<std::string, int, int, double > > >
		pfTuple = cc.getPortfolio(user, pass);
	OperationStatus op_stat = std::get<0>(pfTuple);
	if (op_stat.failed()) {
		center->addWidget(new WText("Failed to fetch Portfolio: " + op_stat.get_exception_msg()));
	}
	else {
		Wt::WTable *table = new Wt::WTable();
		table->setHeaderCount(1);
		table->setWidth(Wt::WLength("100%"));

		table->elementAt(0, 0)->addWidget(new Wt::WText("#"));
		table->elementAt(0, 1)->addWidget(new Wt::WText("Stock Code"));
		table->elementAt(0, 2)->addWidget(new Wt::WText("Quantity"));
		table->elementAt(0, 3)->addWidget(new Wt::WText("Total Cost"));
		table->elementAt(0, 4)->addWidget(new Wt::WText("Current Value"));

		auto vecTup = std::get<1>(pfTuple);
		for (unsigned int i = 0; i < vecTup.size(); ++i) {
			auto tup = vecTup[i];
			table->elementAt(i + 1, 0)->addWidget(new Wt::WText(std::to_string(i + 1)) );

			table->elementAt(i + 1, 1)->addWidget(new Wt::WText(std::get<0>(tup)));
			table->elementAt(i + 1, 2)->addWidget(new Wt::WText(std::to_string(std::get<1>(tup))));
			table->elementAt(i + 1, 3)->addWidget(new Wt::WText(std::to_string(std::get<2>(tup))));
			table->elementAt(i + 1, 4)->addWidget(new Wt::WText(std::to_string((int)std::get<3>(tup))));
		}
		center->addWidget(table);
	}
}

void MarketApplication::handleTrx() {
	CommonController cc;
	std::tuple<OperationStatus, std::vector<std::tuple<std::string, int, int, bool > > >
		trxTuple = cc.getTransactions(user, pass);
	OperationStatus op_stat = std::get<0>(trxTuple);
	if (op_stat.failed()) {
		center->addWidget(new WText("Failed to fetch Transactions: " + op_stat.get_exception_msg()));
	}
	else {
		Wt::WTable *table = new Wt::WTable();
		table->setHeaderCount(1);
		table->setWidth(Wt::WLength("100%"));

		table->elementAt(0, 0)->addWidget(new Wt::WText("#"));
		table->elementAt(0, 1)->addWidget(new Wt::WText("Stock Code"));
		table->elementAt(0, 2)->addWidget(new Wt::WText("Quantity"));
		table->elementAt(0, 3)->addWidget(new Wt::WText("Timestamp(Sec since 1970)"));
		table->elementAt(0, 4)->addWidget(new Wt::WText("Status"));

		auto vecTup = std::get<1>(trxTuple);
		for (unsigned int i = 0; i < vecTup.size(); ++i) {
			auto tup = vecTup[i];
			table->elementAt(i + 1, 0)->addWidget(new Wt::WText(std::to_string(i + 1)));

			table->elementAt(i + 1, 1)->addWidget(new Wt::WText(std::get<0>(tup)));
			table->elementAt(i + 1, 2)->addWidget(new Wt::WText(std::to_string(std::get<1>(tup))));
			/*
			milliseconds ms(std::get<2>(tup) * 1000);
			seconds secs = duration_cast<seconds>(ms);
			std::time_t t = secs.count();
			*/
			table->elementAt(i + 1, 3)->addWidget(new Wt::WText(std::to_string(std::get<2>(tup))));
			
			if (std::get<3>(tup) == 0) {
				table->elementAt(i + 1, 4)->addWidget(new Wt::WText("PENDING"));
			}
			else {
				table->elementAt(i + 1, 4)->addWidget(new Wt::WText("EXECUTED"));
			}
			
		}
		center->addWidget(table);
	}
}

WApplication* createApplication(const WEnvironment& env) {
	return new MarketApplication(env);
}

int ui_landing(int argc, char **argv) {
    try {
		WServer server(argv[0], "");
		try {
			server.setServerConfiguration(argc, argv);
			
			Registration dr;
			server.addResource(&dr, "/RegisterTrader");

			Quote qu;
			server.addResource(&qu, "/Quote");

			Transactions tx;
			server.addResource(&tx, "/Transactions");

			PortfolioList pf;
			server.addResource(&pf, "/PortfolioList");

			Buy by;
			server.addResource(&by, "/Buy");

			Sell sl;
			server.addResource(&sl, "/Sell");
			
			server.addEntryPoint(Wt::Application, createApplication);

			if (server.start()) {
				WServer::waitForShutdown();
				server.stop();
			}
			else {
				return 1;
			}
			return 0;
		}
		catch (std::exception& e) {
			return 1;
		}
	}
	catch (WServer::Exception& e) {
		return 1;
	}
	catch (std::exception& e) {
		return 1;
	}
	catch (...) {
		return 1;
	}
	return 0;
}
