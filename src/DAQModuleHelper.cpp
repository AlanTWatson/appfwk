/**
 * @file DAQModuleHelper.cpp DAQModule helper functions
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "appfwk/DAQModuleHelper.hpp"
#include "appfwk/Issues.hpp"
#include "appfwk/app/Nljs.hpp"

#include "coredal/ConnectionReference.hpp"

#include <string>
#include <vector>

namespace dunedaq::appfwk {

NameUidMap_t
connection_index(const nlohmann::json& iniobj, std::vector<std::string> required)
{
  NameUidMap_t ret;
  for (auto cr : connection_refs(iniobj)) {
    ret[cr.name] = cr.uid;
  }
  for (auto name : required) {
    if (ret.find(name) == ret.end()) {
      throw InvalidSchema(ERS_HERE, "missing connection: " + name);
    }
  }
  return ret;
}


NameUidMap_t
connection_index(const dunedaq::coredal::DaqModule* conf, std::vector<std::string> required)
{
 // I think the connection reference list should be all we need here
  NameUidMap_t ret;
  for (const auto cr: conf->get_conn_refs()) {
     ret[cr->get_cr_name()] = cr->get_uid();
  }
  
  for (auto name : required) {
    if (ret.find(name) == ret.end()) {
      throw InvalidSchema(ERS_HERE, "missing connection: " + name);
    }
  }
  return ret;
}

app::ConnectionReferences_t
connection_refs(const nlohmann::json& iniobj)
{
  return iniobj.get<app::ModInit>().conn_refs;
}

std::string
connection_uid(const nlohmann::json& iniobj, const std::string& name)
{
  return connection_index(iniobj, { name })[name];
}


std::string
connection_uid(const dunedaq::coredal::DaqModule* conf, const std::string& name)
{
  return connection_index(conf, { name })[name];
}

} // namespace dunedaq::appfwk
