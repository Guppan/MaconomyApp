#pragma once

#include "../Remote/json.hpp"

#include <initializer_list>
#include <string>


namespace Maconomy {

	// Does the Json object contains all provided keys?
	bool contains(nlohmann::json& json,
				  const std::initializer_list<std::string>& list);

	// Safely access objects in a Json object.
	class JsonAccess {
	private:
		// Pointer to the current Json object.
		nlohmann::json* _json;

	public:
		// Constructor / Destructor.
		JsonAccess(nlohmann::json* json);
		virtual ~JsonAccess();


		// Get a pointer to the Json object.
		nlohmann::json* get();

		// Safely access the Json object with the provided key.
		JsonAccess operator[](const std::string& key);

		// Safely access the Json array with the provided index.
		JsonAccess operator[](const size_t index);
	};

}