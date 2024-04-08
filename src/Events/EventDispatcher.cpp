#include "precomp.h"
#include "Events/EventDispatcher.h"

EventDispatcher* EventDispatcher::ms_Instance = nullptr;

EventDispatcher* EventDispatcher::GetInstance(){
	if (ms_Instance == nullptr){
		ms_Instance = new EventDispatcher;
	}
	return ms_Instance;
}
