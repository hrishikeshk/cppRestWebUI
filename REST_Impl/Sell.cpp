#include <iostream>
#include <vector>
#include <tuple>

#include <cpprest/json.h>

#include <Wt/Http/Response>
#include "Sell.h"
#include "../common/CommonUtils.h"
#include "../Controller/CommonController.h"
#include "../Controller/OperationStatus.h"

using namespace Wt::Http;

Sell::Sell() {
}

Sell::~Sell() {
}

void Sell::handleRequest(const Request& request, Response& response) {
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
	//
	auto qtyIter = inputs.find("quantity");
	if (qtyIter == inputs.end()) {
		return_bad_request(response, bad_request_quantity);
		return;
	}
	std::vector<std::string> qtyVec = qtyIter->second;
	if (qtyVec.size() != 1 || qtyVec[0].length() < 1) {
		return_bad_request(response, bad_request_quantity);
		return;
	}

	char* end = nullptr;
	int qty = strtol(qtyVec[0].c_str(), &end, 10);
	if (*end || qty < 1) {
		return_bad_request(response, bad_request_quantity);
		return;
	}
	//
	auto priceIter = inputs.find("price");
	if (priceIter == inputs.end()) {
		return_bad_request(response, bad_request_price);
		return;
	}
	std::vector<std::string> priceVec = priceIter->second;
	if (priceVec.size() != 1 || priceVec[0].length() < 1) {
		return_bad_request(response, bad_request_price);
		return;
	}
	
	end = nullptr;
	int price = strtol(priceVec[0].c_str(), &end, 10);
	if (*end || price < 1) {
		return_bad_request(response, bad_request_price);
		return;
	}

	CommonController ctrl;
    std::tuple<OperationStatus, std::tuple<int, std::string, std::string > > op_result = ctrl.sell(userVec[0], 
																									passVec[0],
																									stockVec[0],
																									qty,
																									price);
	
	const OperationStatus& op_stat = std::get<0>(op_result);

	if(op_stat.failed()){
		return_bad_request_Exc(response, op_stat.get_error_code(), op_stat.get_exception_msg());
		return;
	}
	
	response.setMimeType("application/json");

	web::json::value resp_json;

	resp_json[L"stockcode"] = web::json::value::string(utility::conversions::to_string_t(stockVec[0]));
	resp_json[L"quantity"] = web::json::value(qty);

	auto resultTuple = std::get<1>(op_result);

	resp_json[L"price"] = web::json::value(std::get<0>(resultTuple));

	resp_json[L"Sell_Request_Status"] = web::json::value::string(utility::conversions::to_string_t(std::get<1>(resultTuple)));
	resp_json[L"Sell_Request_Comment"] = web::json::value::string(utility::conversions::to_string_t(std::get<2>(resultTuple)));
	
	resp_json[L"status"] = web::json::value::string(U("SUCCESS"));
	resp_json[L"Trader"] = web::json::value::string(utility::conversions::to_string_t(userVec[0]));

	utility::stringstream_t stream;
    resp_json.serialize(stream);
	
	std::ostream& out = response.out();
	out << stream.str() << std::endl;
}
