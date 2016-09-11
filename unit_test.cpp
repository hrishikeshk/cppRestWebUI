#define BOOST_TEST_MODULE crwmodule
#define BOOST_TEST_DYN_LINK
#include <boost/test/included/unit_test.hpp>

#include <string>
#include "Controller/CommonController.h"

BOOST_AUTO_TEST_CASE(test1) {
	OperationStatus op_stat;
	
	BOOST_TEST(op_stat.failed() == true);
}

BOOST_AUTO_TEST_CASE(test2) {
	CommonController cc;
	OperationStatus op_stat;
	std::string u = "hkk";
	std::string p = "hkk";
	int r = cc.areValidCredentials(u, p, op_stat);
	
	BOOST_TEST(r == 1);
	BOOST_TEST(op_stat.failed() == false);

	std::string u = "unknown";
	std::string p = "hkk";
	int r = cc.areValidCredentials(u, p, op_stat);
	
	BOOST_TEST(r == 0);
	BOOST_TEST(op_stat.failed() == true);
}
