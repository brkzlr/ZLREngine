#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

class Event;

class EventListenerBase{
public:
	virtual void Call(Event* event) = 0;
};

template<typename EventType>
class EventListener : public EventListenerBase{
public:
	EventListener(const std::function<void(EventType*)>& callback)
	: m_Callback(callback){

	}

	virtual void Call(Event* event) override{
		m_Callback(static_cast<EventType*>(event));
	}

private:
	std::function<void(EventType*)> m_Callback;
};

#endif
