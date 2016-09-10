#ifndef PORTFOLIOLIST
#define PORTFOLIOLIST

#include <Wt/WResource>

using namespace Wt;
using namespace Wt::Http;

class PortfolioList : public WResource {
public:
	PortfolioList();
	virtual ~PortfolioList();
protected:
	virtual void handleRequest(const Request &request, Response &response);
};

#endif
