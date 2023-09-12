#include "../../include/Misc/Functionality.h"


using namespace Maconomy;


// Does the Json object contains all provided keys?
bool Maconomy::contains(nlohmann::json& json,
						const std::initializer_list<std::string>& list) {
	for (const auto& key : list) {
		if (!json.contains(key)) return false;
	}
	return true;
}


// Constructor.
JsonAccess::JsonAccess(nlohmann::json* json)
	: _json{ json } {}


// Destructor.
JsonAccess::~JsonAccess() {
	_json = nullptr;
}


// Get a pointer to the Json object.
nlohmann::json* JsonAccess::get() {
	return _json;
}


// Safely access the Json object with the provided key.
JsonAccess JsonAccess::operator[](const std::string& key) {
	if (!_json || !_json->contains(key)) return nullptr;
	return &_json->at(key);
}


// Safely access a Json array or object with the provided index.
JsonAccess JsonAccess::operator[](const size_t index) {
	if (!_json || !_json->is_structured()) return nullptr;
	if (index >= _json->size()) return nullptr;
	return &_json[index];
}