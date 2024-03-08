#pragma once
#include "typeid.h"
#include "pch.h"

namespace ge
{
  using EventId = uint32_t;

  struct IEvent
  {
    GE_INLINE virtual ~IEvent() = default;
  };

  // callback lambda
  template <typename T>
  using EventSubscriberFunc = std::function<bool(const T &)>;

  struct IEventSubscriber
  {
    GE_INLINE virtual ~IEventSubscriber() = default;
    virtual bool reactTo(IEvent *) = 0;
  };

  template <typename T>
  struct DefaultEventSubscriber : IEventSubscriber
  {
    GE_INLINE DefaultEventSubscriber(EventSubscriberFunc<T> &f) : _f{f}
    {
    }

    // cannot use IEvent &e
    GE_INLINE bool reactTo(IEvent *e)
    {
      // payload of event
      auto payload = static_cast<const T &>(*e);
      return _f(payload);
    }
  private:
    EventSubscriberFunc<T> _f;
  };

  using EventSubscribers = std::vector<std::shared_ptr<IEventSubscriber>>;
}