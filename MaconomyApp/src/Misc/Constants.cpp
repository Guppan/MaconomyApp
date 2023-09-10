#include "../../include/Misc/Constants.h"

#include <algorithm>


using namespace Maconomy;


// Is the give key 'key' in the domain 'domain'?
bool Maconomy::inDomain(const std::string& key,
						const std::vector<std::string>& domain) {
	auto it = std::find(domain.cbegin(), domain.cend(), key);
	return it != std::end(domain);
}


// Maconomy authorization.
std::string Maconomy::maconomyAuthorization() {
	return MACONOMY_SERVER + "/" + MACONOMY_AUTH;
}


// Maconomy instances.
std::string Maconomy::maconomyInstances() {
	return MACONOMY_SERVER + MACONOMY_INSTANCES;
}


// Maconomy instance id.
std::string Maconomy::maconomyInstanceId(const std::string& id) {
	return maconomyInstances() + "/" + id;
}