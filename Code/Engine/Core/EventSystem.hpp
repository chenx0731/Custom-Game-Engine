#pragma once
#include "Engine/Core/NamedStrings.hpp"
#include <vector>
#include <string>
#include <map>
#include <mutex>

struct EventSystemConfig
{

};

typedef NamedStrings EventArgs;
typedef bool (*EventCallbackFunction)(EventArgs& args);
typedef std::vector<EventCallbackFunction> SubscriptionList;


class EventSystem
{
public:
	EventSystem(EventSystemConfig const& config);
	~EventSystem();
	
	void Startup();
	void Shutdown();
	void BeginFrame();
	void EndFrame();

	void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
	void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
	void FireEvent(std::string const& eventName, EventArgs& args);
	void FireEvent(std::string const& eventName);

	bool CheckIfAEvent(std::string const& eventName);
	Strings GetAllEvents();
	void UnsubscribeAllEvents();
protected:
	EventSystemConfig							m_config;
	std::mutex									m_subListLock;
	std::map<std::string, SubscriptionList>		m_subscriptionListsByEventName;
};

extern EventSystem* g_theEventSystem;

void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
void FireEvent(std::string const& eventName, EventArgs& args);
void FireEvent(std::string const& eventName);
bool CheckIfAEvent(std::string const& eventName);
Strings GetAllEvents();
void UnsubscribeAllEvents();