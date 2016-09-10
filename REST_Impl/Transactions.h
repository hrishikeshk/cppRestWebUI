#ifndef TRANSACTIONS
#define TRANSACTIONS

#include <Wt/WResource>

using namespace Wt;
using namespace Wt::Http;

class Transactions : public WResource {
public:
	Transactions();
	virtual ~Transactions();
protected:
	virtual void handleRequest(const Request &request, Response &response);
};

#endif
