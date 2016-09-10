#include <iostream>
#include <vector>
#include <tuple>

#include <cpprest/json.h>

#include <Wt/Http/Response>
#include "PortfolioList.h"
#include "../common/CommonUtils.h"
#include "../Controller/CommonController.h"
#include "../Controller/OperationStatus.h"

using namespace Wt::Http;

PortfolioList::PortfolioList() {
}

PortfolioList::~PortfolioList() {
}

void PortfolioList::handleRequest(const Request& request, Response& response) {
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
    std::tuple<OperationStatus, std::vector<std::tuple<std::string, int, int, double > > > op_result = ctrl.getPortfolio(userVec[0], 
																															passVec[0]);
	
	const OperationStatus& op_stat = std::get<0>(op_result);

	if(op_stat.failed()){
		return_bad_request_Exc(response, op_stat.get_error_code(), op_stat.get_exception_msg());
		return;
	}
	
	response.setMimeType("application/json");

	web::json::value resp_json;

	web::json::value pf_array;
	std::vector<std::tuple<std::string, int, int, double > >& vecPf = std::get<1>(op_result);
	for (unsigned int i = 0; i < vecPf.size(); ++i) {
		web::json::value pfe;
		pfe[L"stockcode"] = web::json::value::string(utility::conversions::to_string_t(std::get<0>(vecPf[i]) ) );
		pfe[L"quantity"] = web::json::value(std::get<1>(vecPf[i]));
		pfe[L"totalcost"] = web::json::value(std::get<2>(vecPf[i]));
		pfe[L"currentvalue"] = web::json::value(std::get<3>(vecPf[i]));
		
		pf_array[i] = pfe;
	}
	
	resp_json[L"PortfolioList"] = pf_array;
	resp_json[L"status"] = web::json::value::string(U("Success"));
	resp_json[L"Trader"] = web::json::value::string(utility::conversions::to_string_t(userVec[0]));

	utility::stringstream_t stream;
    resp_json.serialize(stream);
	
	std::ostream& out = response.out();
	out << stream.str() << std::endl;
}
