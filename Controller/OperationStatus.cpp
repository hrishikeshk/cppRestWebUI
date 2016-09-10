#include "OperationStatus.h"

OperationStatus::OperationStatus():m_status(true), m_err_code(bad_request_upass) {

}

bool OperationStatus::failed(){
	return m_status;
}

err_code OperationStatus::get_error_code(){
	return m_err_code;
}
