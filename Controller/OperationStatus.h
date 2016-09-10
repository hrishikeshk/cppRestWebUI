#ifndef OPERATION_STATUS
#define OPERATION_STATUS

#include <string>
#include "../common/CommonUtils.h"

class OperationStatus{
	bool m_status;
	err_code m_err_code;
	std::string m_exception_msg;
	public:
		OperationStatus();
		bool failed();
		void set_error(const std::string& msg, err_code ec) {
			m_exception_msg = msg;
			m_err_code = ec;
		};

		err_code get_error_code();

		std::string get_exception_msg() {
			return m_exception_msg;
		}
		void set_status(bool stat) {
			m_status = stat;
		}
};

#endif
