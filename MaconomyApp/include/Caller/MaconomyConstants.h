#pragma once

#include <string>


namespace Maconomy {

	// ----- Paths -----------------------------------------------

	// Server path.
	const std::string SERVER = "https://me96763-iaccess.deltekfirst.com";

	// Authorization path.
	std::string AUTHORIZATION() {
		return SERVER + "/" + "maconomy-api/auth/me96763";
	}

	// Instances path.
	std::string INSTANCES() {
		return SERVER + "/maconomy-api/containers/me96763/timeregistration/instances";
	}

	// Instance id path.
	std::string INSTANCE_ID(const std::string& id) {
		return INSTANCES() + "/" + id;
	}

	// ----- Error texts ------------------------------------------

	const std::string LOGIN_ERROR = "Login to Maconomy failed";

	const std::string INSTANCE_ERROR = "Instance retrieval failed";

	const std::string EMPLOYEE_NR_ERROR = "Employee number retrieval failed";

	const std::string ROW_COUNT_ERROR = "Row count retrieval failed";

}