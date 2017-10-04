#pragma once

// The timer task used in AITimer.

#include<vector>
#include<algorithm>
#include<set>
#include<chrono>
#include<memory>

using timetype = std::chrono::steady_clock::time_point;
using lambdatype = std::function<void()>;


// The timer class used in AITimer.

class AIFrameTimer {

  // Container class for timed callbacks
  class TimerContainer {
    private:
      using handletype = int;
      using pairtype = std::pair<handletype, lambdatype>;
      timetype m_time;
      mutable std::vector<pairtype> m_callbacks;
      mutable handletype next_handle;

    public:
      TimerContainer(timetype const& time, lambdatype const& callback) : m_time(time), m_callbacks(1, pairtype(0, callback)), next_handle(1) { }
      //TimerContainer(TimerContainer const&& other) : m_time(std::move(other.m_time)), m_callbacks(std::move(other.m_callbacks)) {}
      handletype push_back (lambdatype const& callback) const
      {
        m_callbacks.push_back(pairtype(next_handle, callback));
        return next_handle++;
      }

      void call() const { for(pairtype in_vector : m_callbacks) in_vector.second(); }

      bool is_in(handletype handle) const
      {
        for(pairtype in_vector : m_callbacks)
          if(handle == in_vector.first)
            return true;
        return false;
      }

      bool remove(handletype handle) const
      {
        m_callbacks.erase(std::remove_if(m_callbacks.begin(), m_callbacks.end(), [handle](pairtype const& in_vector){return handle == in_vector.first;}), m_callbacks.end());
        return m_callbacks.empty();
      }

      friend bool operator<(TimerContainer const& lhs, TimerContainer const& rhs) {return lhs.m_time < rhs.m_time;}
      //friend bool operator==(TimerContainer const& lhs, timetype const& rhs) {return lhs.m_time == rhs;}
      //TimerContainer& operator=(TimerContainer const& other) {return TimerContainer(other);}
  };

  private:
    static std::set<TimerContainer> timer_containers;
    std::set<TimerContainer>::iterator my_container;
    int my_handle;

  public:
    AIFrameTimer() {}
    void create(timetype const& interval, lambdatype const& callback);
    void cancel();
    bool isRunning() const;
};

