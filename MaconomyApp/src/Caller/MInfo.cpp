#include "../../include/Caller/MInfo.h"
#include "../../include/Remote/json.hpp"

#include <iostream>


using namespace Maconomy;
using json = nlohmann::json;


// Get the reconnect header.
std::string MInfo::reconnectHeader() const {
	return "Maconomy-Authentication: X-Reconnect";
}


// Get the authorization header.
std::string MInfo::authorizationHeader() const {
	return "Authorization: X-Reconnect " + _reconnectToken;
}


// Get the content type header.
std::string MInfo::contentTypeHeader() const {
	return "Content-type: application/vnd.deltek.maconomy.containers-v2+json";
}


// Get the concurrency header.
std::string MInfo::concurrencyHeader() const {
	return "Maconomy-Concurrency-Control: " + _concurrencyToken;
}


// Get the logout header.
std::string MInfo::logoutHeader() const {
	return "Maconomy-Authentication: X-Log-Out";
}


// Set the cURL handle.
void MInfo::setHandle(CURL* handle) {
	_handle = handle;
}


// Set the reconnect token.
void MInfo::setReconnectToken() {
	if (!_handle) return;

	struct curl_header* header = getHeader("Maconomy-Reconnect");
	if (!header) return;

	_reconnectToken = header->value;
}


// Set the concurrency token.
void MInfo::setConcurrencyToken() {
	if (!_handle) return;

	struct curl_header* header = getHeader("Maconomy-Concurrency-Control");
	if (!header) return;

	_concurrencyToken = header->value;
}

#include <iostream>
using namespace std;
// Set the instance id.
void MInfo::setInstanceId() {
	if (_responseData.empty()) return;

	json parsed = json::parse(_responseData);
	parsed["meta"]["containerInstanceId"].get_to(_instanceId);
}


// Set the employee number.
void MInfo::setEmployeeNumber() {
	if (_responseData.empty()) return;

	json parsed = json::parse(_responseData);
	parsed["panes"]["card"]["records"][0]["data"]["employeenumber"].get_to(_employeeNumber);
}


// Set the row count.
void MInfo::setRowCount() {
	if (_responseData.empty()) return;

	json parsed = json::parse(_responseData);
	parsed["panes"]["table"]["meta"]["rowCount"].get_to(_rowCount);
}

// Set the response data.
void MInfo::setResponseData(std::string&& data) {
	_responseData = std::move(data);
}


// Get a header from the handle.
struct curl_header* MInfo::getHeader(const std::string& name) const {
	if (!_handle) return nullptr;

	struct curl_header* header;
	CURLHcode res = curl_easy_header(_handle,
									 name.c_str(),
									 0, CURLH_HEADER, -1,
									 &header);
	if (res != CURLHE_OK) {
		std::cerr << "curl_easy_header() failed: " << res << std::endl;
		return nullptr;
	}
	return header;
}