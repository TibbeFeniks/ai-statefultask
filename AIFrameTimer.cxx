// The timer task used by AITimer

#include "sys.h"
#include "AIFrameTimer.h"

void AIFrameTimer::create(timetype const& interval, lambdatype const& callback)
{
  auto result = timer_containers.emplace(interval, callback);
  if(!result.second)
    my_handle = result.first->push_back(callback);
  else
    my_handle = 0;
  my_container = result.first;
}

void AIFrameTimer::cancel()
{
  if(my_container->remove(my_handle))
    timer_containers.erase(my_container);
}

bool AIFrameTimer::isRunning() const
{
  return my_container->is_in(my_handle);
}

#if 0
void AIFrameTimer::expire(timetype const& interval)
{
  auto result = timer_containers.find(interval);
  if(result == timer_containers.end())
    return;
  result->call();
  timer_containers.erase(result);
}
#endif

