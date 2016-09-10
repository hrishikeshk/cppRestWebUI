#include <iostream>
#include <vector>
#include <tuple>

#include <cpprest/json.h>

#include <Wt/Http/Response>
#include "Quote.h"
#include "../common/CommonUtils.h"
#include "../Controller/CommonController.h"
#include "../Controller/OperationStatus.h"

using namespace Wt::Http;

Quote::Quote() {
}

Quote::~Quote() {
}

void Quote::handleRequest(const Request& request, Response& response) {
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
    
	auto stockIter = inputs.find("stockcode");
	if (stockIter == inputs.end()) {
		return_bad_request(response, bad_request_stockcode);
		return;
	}
	std::vector<std::string> stockVec = stockIter->second;
	if (stockVec.size() != 1 || stockVec[0].length() < 1) {
		return_bad_request(response, bad_request_stockcode);
		return;
	}
	CommonController ctrl;
    std::tuple<OperationStatus, int> op_result = ctrl.getQuote(userVec[0], passVec[0], stockVec[0]);
	
	const OperationStatus& op_stat = std::get<0>(op_result);

	if(op_stat.failed()){
		return_bad_request_Exc(response, op_stat.get_error_code(), op_stat.get_exception_msg());
		return;
	}
	
	response.setMimeType("application/json");

	web::json::value resp_json;
	resp_json[L"stockcode"] = web::json::value::string(utility::conversions::to_string_t(stockVec[0]));
	resp_json[L"lastsaleprice"] = web::json::value(std::get<1>(op_result) );
	resp_json[L"status"] = web::json::value::string(U("Success"));
	
	utility::stringstream_t stream;
    resp_json.serialize(stream);
	
	std::ostream& out = response.out();
	out << stream.str() << std::endl;
}
