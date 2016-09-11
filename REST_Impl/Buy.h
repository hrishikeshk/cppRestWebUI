#ifndef BUY
#define BUY

#include <Wt/WResource>

using namespace Wt;
using namespace Wt::Http;

class Buy : public WResource {
public:
	Buy();
	virtual ~Buy();
protected:
	virtual void handleRequest(const Request &request, Response &response);
};

#endif
