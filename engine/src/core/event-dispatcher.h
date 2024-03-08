#pragma once
#include "event.h"
#include <memory>
#include "pch.h"
#include <typeinfo>

namespace ge
{
    // dispatcher will manage all the events and register subscriber to all the events
    // dispatcher will apply f to the events.
    struct EventDispatcher
    {
        // template-metaProgramming
        // requires c++20
        template <std::derived_from<IEvent> T, typename... Args>
        GE_INLINE void addEvent(Args &&...args)
        {
            GE_STATIC_ASSERT(std::is_base_of<IEvent, T>::value);
            // typeId will generate ever-increasing id for event
            _events[typeId<T>()] = std::make_unique<T>(std::forward<Args>(args)...);
        }

        template <std::derived_from<IEvent> T>
        GE_INLINE void subscribe(EventSubscriberFunc<T> &&f)
        {
            GE_STATIC_ASSERT(std::is_base_of<IEvent, T>::value);
            _subscribers[typeId<T>()].emplace_back(std::make_shared<DefaultEventSubscriber<T>>(f));
        }

        // event polling
        // dispatch per frame
        GE_INLINE void dispatch()
        {
            if (!_events.size())
            {
                return;
            }
            for (auto &[eid, event] : _events)
            {
                for (auto &sub : _subscribers[eid])
                {
                    if (sub->reactTo(event.get()))
                    {
                        break;
                    }
                }
            }
            _events.clear();
        }

    private:
        std::unordered_map<EventId, std::unique_ptr<IEvent>> _events;
        std::unordered_map<EventId, EventSubscribers> _subscribers;
    };
}