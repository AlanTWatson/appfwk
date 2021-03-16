/**
 * @file QueueRegistry.cpp
 *
 * The QueueRegistry class implementation
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "appfwk/QueueRegistry.hpp"

#include <map>
#include <memory>
#include <string>

namespace dunedaq::appfwk {

std::unique_ptr<QueueRegistry> QueueRegistry::s_instance = nullptr;

QueueRegistry&
QueueRegistry::get()
{
  if (!s_instance) {
    s_instance.reset(new QueueRegistry());
  }
  return *s_instance;
}

void
QueueRegistry::configure(const std::map<std::string, QueueConfig>& config_map)
{
  if (m_configured) {
    throw QueueRegistryConfigured(ERS_HERE);
  }

  m_queue_config_map = config_map;
  m_configured = true;
}

QueueConfig::queue_kind
QueueConfig::stoqk(const std::string& name)
{
  if (name == "StdDeQueue" || name == "std_deque")
    return queue_kind::kStdDeQueue;
  else if (name == "FollySPSCQueue")
    return queue_kind::kFollySPSCQueue;
  else if (name == "FollyMPMCQueue")
    return queue_kind::kFollyMPMCQueue;
  else
    throw QueueKindUnknown(ERS_HERE, name);
}

} // namespace dunedaq::appfwk
