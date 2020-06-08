#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include "Events/EventListener.h"
#include <typeindex>

class EventDispatcher{
	using ListenerList = std::vector<EventListenerBase*>;

public:
	static EventDispatcher* GetInstance();

 	template<typename T, typename EventType>
	void AddListener(T* classInstance, void (T::*memberFunc)(EventType*)){
		ListenerList* listeners = m_ListenersMap[typeid(EventType)];

		if(!listeners){
			listeners = new ListenerList();
			m_ListenersMap[typeid(EventType)] = listeners;
		}

		listeners->push_back(new EventListener<EventType>(std::bind(memberFunc, classInstance, std::placeholders::_1)));
	}

	//TO DO: Make an event queue and process it at end of frame
	template<typename EventType>
	void Broadcast(EventType* event){
		ListenerList* listeners = m_ListenersMap[typeid(EventType)];

		if (!listeners){
			return;
		}

		for (EventListenerBase* listener : *listeners){
			if (listener){
				listener->Call(event);
			}
		}
	}

private:
	static EventDispatcher* ms_Instance;
	EventDispatcher() = default;

	std::map<std::type_index, ListenerList*> m_ListenersMap;
};

#endif