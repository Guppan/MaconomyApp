#pragma once

#include <string>
#include <vector>

namespace Maconomy {

	// Holds the necessary data needed when performing
	// cURL calls.
	struct CallInfo {
		// Verbose (debug only).
		bool verbose;

		// Zero content POST?
		bool zeroPost;

		// Fail on error?
		bool failOnError;

		// Url to call.
		std::string url;

		// Credentials.
		std::string credentials;

		// Headers.
		std::vector<std::string> headers;

		// Data.
		std::string data;


		// Constructor.
		CallInfo() {
			clear();
		}


		// Add headers.
		void addHeader(const std::string& header) {
			headers.push_back(header);
		}


		// Clear.
		void clear() {
			verbose = false;
			zeroPost = false;
			failOnError = true;
			url.clear();
			credentials.clear();
			headers.clear();
			data.clear();
		}
	};

}