#include "OperationStatus.h"

OperationStatus::OperationStatus():m_status(true), m_err_code(bad_request_upass) {

}

bool OperationStatus::failed() const{
	return m_status;
}

err_code OperationStatus::get_error_code() const{
	return m_err_code;
}
