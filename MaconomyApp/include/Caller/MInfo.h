#pragma once

#include <string>
#include <curl/curl.h>

namespace Maconomy {

	// Holds information needed when doing requests to Maconomy.
	class MInfo {
	private:
		// Curl handle.
		CURL* _handle;

		// Reconnect token.
		std::string _reconnectToken;

		// Concurrency token.
		std::string _concurrencyToken;

		// Instance id.
		std::string _instanceId;

		// Employee number.
		std::string _employeeNumber;

		// Row count.
		unsigned _rowCount;

		// Response data.
		std::string _responseData;

	public:
		// Constructor / Destructor.
		MInfo() : _handle{ nullptr }, _rowCount{ 0 } { }
		~MInfo() = default;


		// Headers.
		std::string reconnectHeader() const;
		std::string authorizationHeader() const;
		std::string contentTypeHeader() const;
		std::string concurrencyHeader() const;
		std::string logoutHeader() const;

		// Getters.
		const std::string& instanceId() const { return _instanceId; }
		const std::string& employeeNumber() const { return _employeeNumber; }
		unsigned rowCount() const { return _rowCount; }

		// Setters.
		void setHandle(CURL* handle);
		void setReconnectToken();
		void setConcurrencyToken();
		void setInstanceId();
		void setEmployeeNumber();
		void setRowCount();
		void setResponseData(std::string&& data);

	private:
		// Get a header from the handle.
		struct curl_header* getHeader(const std::string& name) const;
	};

}