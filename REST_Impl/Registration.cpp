#include <iostream>
#include <vector>
#include <cpprest/json.h>

#include <Wt/Http/Response>
#include "Registration.h"
#include "../common/CommonUtils.h"
#include "../Controller/CommonController.h"
#include "../Controller/OperationStatus.h"

using namespace Wt::Http;

Registration::Registration() {
}

Registration::~Registration() {
}

void Registration::handleRequest(const Request& request, Response& response) {
	int contentLength = request.contentLength();
	if (contentLength > MAX_CONTENT_LENGTH) {
		return_bad_request(response, content_length_exceeded);
		return;
	}

	auto inputs = request.getParameterMap();
	
	auto userIter = inputs.find("username");
	if(userIter == inputs.end()){
		return_bad_request(response, bad_request_upass);
		return;
	}
	
	auto passIter = inputs.find("password");
	if(passIter == inputs.end()){
		return_bad_request(response, bad_request_upass);
		return;
	}
    std::vector<std::string> userVec = userIter->second;
    std::vector<std::string> passVec = passIter->second;
    if(userVec.size() != 1 || passVec.size() != 1 || userVec[0].length() < 1 || passVec[0].length() < 1){
		return_bad_request(response, bad_request_upass);
		return;
	}
    
	CommonController ctrl;
    OperationStatus op_stat = ctrl.registerNewTrader(userVec[0], passVec[0]);
	
	if(op_stat.failed()){
		return_bad_request_Exc(response, op_stat.get_error_code(), op_stat.get_exception_msg());
		return;
	}
	
	response.setMimeType("application/json");

	web::json::value resp_json;
	resp_json[L"trader"] = web::json::value::string(utility::conversions::to_string_t(userVec[0]));
	resp_json[L"balance"] = web::json::value(INITIAL_BALANCE);
	resp_json[L"status"] = web::json::value::string(U("SUCCESS"));
	
	utility::stringstream_t stream;
    resp_json.serialize(stream);
	
	std::ostream& out = response.out();
	out << stream.str() << std::endl;
}
