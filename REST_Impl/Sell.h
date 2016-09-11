#ifndef SELL
#define SELL

#include <Wt/WResource>

using namespace Wt;
using namespace Wt::Http;

class Sell : public WResource {
public:
	Sell();
	virtual ~Sell();
protected:
	virtual void handleRequest(const Request &request, Response &response);
};

#endif
