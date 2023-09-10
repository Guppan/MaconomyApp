#include "../../include/Caller/Caller.h"
#include "../../include/Caller/MInfo.h"
#include "../../include/Caller/CallInfo.h"
#include "../../include/Importer/Entry.h"
#include "../../include/Misc/Constants.h"

#include <iostream>


using namespace Maconomy;


// Write data method.
size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
	data->append((char*)ptr, size * nmemb);
	return size * nmemb;
}


// Constructor.
Caller::Caller() {
	curl_global_init(CURL_GLOBAL_ALL);
	_handle = curl_easy_init();
	_info.setHandle(_handle);
}


// Destructor.
Caller::~Caller() {
	// Cleanup handle.
	curl_easy_cleanup(_handle);
	curl_global_cleanup();
}


// Login to Maconomy.
bool Caller::login(const std::string& username,
				   const std::string& password) {
	std::cout << "Logging into Maconomy...\n";

	struct CallInfo cInfo {};

	cInfo.failOnError = false;
	cInfo.url = maconomyAuthorization();
	cInfo.credentials = username + ":" + password;
	cInfo.addHeader(_info.reconnectHeader());

	if (!successfulRequest(cInfo, MACONOMY_LOGIN_ERROR)) return false;
	_info.setReconnectToken();
	curl_easy_reset(_handle);

	std::cout << "Login successful\n";
	return true;
}


// Logout from Maconomy.
void Caller::logout() {
	struct CallInfo cInfo {};

	cInfo.failOnError = false;
	cInfo.url = maconomyAuthorization();
	cInfo.addHeader(_info.authorizationHeader());
	cInfo.addHeader(_info.logoutHeader());

	performRequest(cInfo);
	curl_easy_reset(_handle);
}


// Update tokens.
bool Caller::updateTokens(const std::string& date) {
	std::cout << "Updating Maconomy tokens...\n";
	struct CallInfo cInfo {};

	// Set instance id.
	cInfo.url = maconomyInstances();;
	cInfo.addHeader(_info.authorizationHeader());
	cInfo.addHeader(_info.contentTypeHeader());
	cInfo.data = "{\"panes\":{}}";

	if (!successfulRequest(cInfo, MACONOMY_INSTANCE_ERROR)) return false;
	_info.setInstanceId();
	_info.setConcurrencyToken();
	curl_easy_reset(_handle);

	// Set employee number.
	cInfo.clear();
	cInfo.zeroPost = true;
	cInfo.url = maconomyInstanceId(_info.instanceId()) + "/data;any";
	cInfo.addHeader(_info.authorizationHeader());
	cInfo.addHeader(_info.concurrencyHeader());
	cInfo.addHeader(_info.contentTypeHeader());

	if (!successfulRequest(cInfo, MACONOMY_EMPLOYEE_NR_ERROR)) return false;
	_info.setConcurrencyToken();
	_info.setEmployeeNumber();
	curl_easy_reset(_handle);

	// Set row count.
	cInfo.clear();
	cInfo.url = maconomyInstanceId(_info.instanceId()) + "/data/panes/card/0";
	cInfo.addHeader(_info.authorizationHeader());
	cInfo.addHeader(_info.concurrencyHeader());
	cInfo.addHeader(_info.contentTypeHeader());
	cInfo.data = "{\"data\":{\"datevar\":\"" + date + "\",\"employeenumbervar\":\"" + _info.employeeNumber() + "\"}}";

	if (!successfulRequest(cInfo, MACONOMY_ROW_COUNT_ERROR)) return false;
	_info.setConcurrencyToken();
	_info.setRowCount();
	curl_easy_reset(_handle);

	std::cout << "Updating tokens successful\n";
	return true;
}


// Add multiple rows in the weekly time sheet.
void Caller::addRows(std::vector<Entry*> entries) {
	std::cout << "Starting upload to Maconomy...\n";

	int total = static_cast<int>(entries.size());
	int transfered{};

	for (auto& entry : entries) {
		struct CallInfo cInfo {};

		cInfo.url = maconomyInstanceId(_info.instanceId()) + "/data/panes/table?row=end";
		cInfo.addHeader(_info.authorizationHeader());
		cInfo.addHeader(_info.concurrencyHeader());
		cInfo.addHeader(_info.contentTypeHeader());
		cInfo.data = entry->toJson();

		if (performRequest(cInfo) != CURLE_OK) entry->valid = false;
		else ++transfered;

		_info.setConcurrencyToken();
		curl_easy_reset(_handle);
	}

	// Print result of transfer.
	std::cout << "-----------------------------------------------\n";
	std::cout << "Upload to Maconomy completed:\n";
	std::cout << "Total entries: " << total << '\n';
	std::cout << "Successful:    " << transfered << '\n';
	std::cout << "Failed:        " << total - transfered << '\n';
}


// Perform a cURL request.
int Caller::performRequest(const struct CallInfo& info) {
	if (info.url.empty()) return -1;
	if (!_handle) _handle = curl_easy_init();

	if (info.verbose) {
		curl_easy_setopt(_handle, CURLOPT_VERBOSE, 1L);
	}

	if (info.zeroPost) {
		curl_easy_setopt(_handle, CURLOPT_POST, 1L);
		curl_easy_setopt(_handle, CURLOPT_POSTFIELDSIZE, 0L);
	}

	if (info.failOnError) {
		curl_easy_setopt(_handle, CURLOPT_FAILONERROR, 1L);
	}

	curl_easy_setopt(_handle, CURLOPT_URL, info.url.c_str());

	if (!info.credentials.empty()) {
		curl_easy_setopt(_handle, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
		curl_easy_setopt(_handle, CURLOPT_USERPWD, info.credentials.c_str());
	}

	struct curl_slist* headers = NULL;
	for (const auto& header : info.headers) {
		headers = curl_slist_append(headers, header.c_str());
	}
	curl_easy_setopt(_handle, CURLOPT_HTTPHEADER, headers);

	if (!info.data.empty() and !info.zeroPost) {
		curl_easy_setopt(_handle, CURLOPT_POSTFIELDS, info.data.c_str());
	}

	curl_easy_setopt(_handle, CURLOPT_SSL_OPTIONS, CURLSSLOPT_NATIVE_CA);

	std::string responseData;
	curl_easy_setopt(_handle, CURLOPT_WRITEFUNCTION, writeFunction);
	curl_easy_setopt(_handle, CURLOPT_WRITEDATA, &responseData);

	CURLcode res = curl_easy_perform(_handle);
	curl_slist_free_all(headers);

	if (res != CURLE_OK) {
		std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
		curl_easy_cleanup(_handle);
		return res;
	}

	_info.setResponseData(std::move(responseData));
	return 0;
}


// Returns true if the request was successful.
bool Caller::successfulRequest(const struct CallInfo& info,
							   const std::string& error) {
	int res = performRequest(info);
	if (res != CURLE_OK) {
		curl_easy_reset(_handle);
		std::cerr << error << std::endl;
		return false;
	}
	return true;
}