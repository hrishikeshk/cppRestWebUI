#ifndef QUOTE
#define QUOTE

#include <Wt/WResource>

using namespace Wt;
using namespace Wt::Http;

class Quote : public WResource {
public:
	Quote();
	virtual ~Quote();
protected:
	virtual void handleRequest(const Request &request, Response &response);
};

#endif
