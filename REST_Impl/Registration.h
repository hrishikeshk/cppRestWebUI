#ifndef REGISTRATION
#define REGISTRATION

#include <Wt/WResource>

using namespace Wt;
using namespace Wt::Http;

class Registration : public WResource {
public:
	Registration();
	virtual ~Registration();
protected:
	virtual void handleRequest(const Request &request, Response &response);
};

#endif
