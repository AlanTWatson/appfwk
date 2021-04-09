#ifndef APPFWK_INCLUDE_APPFWK_STDDEQUEUE_HPP_
#define APPFWK_INCLUDE_APPFWK_STDDEQUEUE_HPP_

/**
 *
 * @file StdDeQueue.hpp
 *
 * A std::deque-based implementation of Queue
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "appfwk/Queue.hpp"

#include <atomic>
#include <cassert>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>

namespace dunedaq {
namespace appfwk {
/**
 * @brief A Queue Implementation that uses a std::deque as its backend
 * @tparam T Data Type to be stored in the std::deque
 */
template<class T>
class StdDeQueue : public Queue<T>
{
public:
  using value_t = T;                                ///< Type of data stored in the StdDeQueue
  using duration_t = typename Queue<T>::duration_t; ///< Type used for expressing timeouts

  /**
   * @brief StdDeQueue Constructor
   * @param name Name of this StdDeQueue instance
   */
  explicit StdDeQueue(const std::string& name, size_t capacity);

  bool can_pop() const noexcept override { return m_size.load() > 0; }
  void pop(value_t& val, const duration_t&) override; // Throws QueueTimeoutExpired if a timeout occurs

  bool can_push() const noexcept override { return m_size.load() < this->get_capacity(); }
  void push(value_t&&, const duration_t&) override; // Throws QueueTimeoutExpired if a timeout occurs

  // Delete the copy and move operations since various member data instances
  // (e.g., of std::mutex or of std::atomic) aren't copyable or movable

  virtual void get_info(opmonlib::InfoCollector& /*ci*/, int /*level*/) override ;

  StdDeQueue(const StdDeQueue&) = delete;            ///< StdDeQueue is not copy-constructible
  StdDeQueue& operator=(const StdDeQueue&) = delete; ///< StdDeQueue is not copy-assignable
  StdDeQueue(StdDeQueue&&) = delete;                 ///< StdDeQueue is not move-constructible
  StdDeQueue& operator=(StdDeQueue&&) = delete;      ///< StdDeQueue is not move-assignable

private:
  void try_lock_for(std::unique_lock<std::mutex>&, const duration_t&);

  std::deque<value_t> m_deque;
  std::atomic<size_t> m_size = 0;

  std::mutex m_mutex;
  std::condition_variable m_no_longer_full;
  std::condition_variable m_no_longer_empty;
};

} // namespace appfwk

} // namespace dunedaq

#include "detail/StdDeQueue.hxx"

#endif // APPFWK_INCLUDE_APPFWK_STDDEQUEUE_HPP_
