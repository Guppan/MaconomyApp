#pragma once

#include "MInfo.h"

#include <memory>
#include <string>
#include <vector>
#include <curl/curl.h>

/*
	TODO:
		1. Add functionality for creating a sheet.
		2. Implement state machine for handling bad requests.
*/

namespace Maconomy {

	// Forward declarations.
	struct CallInfo;
	struct Entry;


	// Responsible for performing calls to the Maconomy API.
	class Caller
	{
	public:
		// Typedefs.
		typedef std::unique_ptr<Caller> ptr;

	private:
		// Curl handle.
		CURL* _handle;

		// Maconomy info.
		MInfo _info;

	public:
		// Constructor / Destructor.
		Caller();
		virtual ~Caller();


		// Login to Maconomy.
		bool login(const std::string& username,
				   const std::string& password);

		// Logout from Maconomy.
		void logout();

		// Update tokens.
		bool updateTokens(const std::string& date);

		// Add multiple rows in the weekly time sheet.
		void addRows(std::vector<Entry*> entries);

		// Perform a cURL request.
		int performRequest(const struct CallInfo& info);

	private:
		// Returns true if the request was successful.
		// Otherwise outputs 'error' to std::cerr and returns false.
		bool successfulRequest(const struct CallInfo& info,
							   const std::string& error);
	};

}
