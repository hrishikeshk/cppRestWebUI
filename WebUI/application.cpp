#pragma warning(disable:4996)

#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>

#include <Wt/WLogger>

#include <Wt/WServer>
#include <iostream>

#include "../REST_Impl/Registration.h"
#include "../REST_Impl/Quote.h"
#include "../REST_Impl/Transactions.h"
#include "../REST_Impl/PortfolioList.h"

#include <functional>
#include "application.h"
#include "../common/CommonUtils.h"

using namespace Wt;

class HelloApplication : public WApplication {
public:
	HelloApplication(const WEnvironment& env);

private:
	WLineEdit *nameEdit_;
	WText *greeting_;

	void greet(const std::string& message);
};

HelloApplication::HelloApplication(const WEnvironment& env)
	: WApplication(env)
{
	setTitle("Hello world");                               // application title

	root()->addWidget(new WText("Your name, please ? "));  // show some text
	nameEdit_ = new WLineEdit(root());                     // allow text input
	nameEdit_->setFocus();                                 // give focus

	WPushButton *button
		= new WPushButton("Greet me.", root());              // create a button
	button->setMargin(5, Left);                            // add 5 pixels margin

	root()->addWidget(new WBreak());                       // insert a line break

	greeting_ = new WText(root());                         // empty text

	//button->clicked().connect(this, &HelloApplication::greet);
	//button->clicked().connect(boost::bind(&HelloApplication::greet, this, "You pressed button"));

	auto bound_greet = std::bind(&HelloApplication::greet, this, "You pressed button, with std::bind");
	button->clicked().connect(bound_greet);

	nameEdit_->enterPressed().connect
		(boost::bind(&HelloApplication::greet, this, "You pressed enter"));

	/*button->clicked().connect(std::bind([=]() {
		greeting_->setText("Hello there, " + nameEdit_->text());
	}));
	*/
}

void HelloApplication::greet(const std::string& message) {
	greeting_->setText("Hello there, " + nameEdit_->text() + ", have a nice " + message);
}

WApplication *createApplication(const WEnvironment& env) {
	return new HelloApplication(env);
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
}
