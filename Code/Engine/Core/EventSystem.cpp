#include "Engine/Core/EventSystem.hpp"

EventSystem* g_theEventSystem = nullptr;

EventSystem::EventSystem(EventSystemConfig const& config) : m_config(config)
{
	
}

EventSystem::~EventSystem()
{
}

void EventSystem::Startup()
{
}

void EventSystem::Shutdown()
{
	UnsubscribeAllEvents();
}

void EventSystem::BeginFrame()
{
}

void EventSystem::EndFrame()
{
}

void EventSystem::SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	m_subListLock.lock();
	for (int funcIndex = 0; funcIndex < m_subscriptionListsByEventName[eventName].size(); funcIndex++) {
		if (m_subscriptionListsByEventName[eventName][funcIndex] == nullptr) {
			m_subscriptionListsByEventName[eventName][funcIndex] = functionPtr;
			m_subListLock.unlock();
			return;
		}
	}
	m_subscriptionListsByEventName[eventName].push_back(functionPtr);
	m_subListLock.unlock();
}

void EventSystem::UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	m_subListLock.lock();
	for (int funcIndex = 0; funcIndex < m_subscriptionListsByEventName[eventName].size(); funcIndex++) {
		if (functionPtr == m_subscriptionListsByEventName[eventName][funcIndex]) {
			m_subscriptionListsByEventName[eventName][funcIndex] = nullptr;
		}
	}
	m_subListLock.unlock();
}

void EventSystem::FireEvent(std::string const& eventName, EventArgs& args)
{
	for (int funcIndex = 0; funcIndex < m_subscriptionListsByEventName[eventName].size(); funcIndex++) {
		if (m_subscriptionListsByEventName[eventName][funcIndex]) {
			EventCallbackFunction callbackFunciton = m_subscriptionListsByEventName[eventName][funcIndex];
			bool isConsumed = false;
			isConsumed = callbackFunciton(args);
			if (isConsumed)
				break;
		}
	}
}

void EventSystem::FireEvent(std::string const& eventName)
{
	m_subListLock.lock();
	for (int funcIndex = 0; funcIndex < m_subscriptionListsByEventName[eventName].size(); funcIndex++) {
		if (m_subscriptionListsByEventName[eventName][funcIndex]) {
			EventArgs args;
			EventCallbackFunction callbackFunciton = m_subscriptionListsByEventName[eventName][funcIndex];
			bool isConsumed = false;
			isConsumed = callbackFunciton(args);
			if (isConsumed)
				break;
		}	
	}
	m_subListLock.unlock();
}

bool EventSystem::CheckIfAEvent(std::string const& eventName)
{
	m_subListLock.lock();
	if (m_subscriptionListsByEventName.find(eventName) == m_subscriptionListsByEventName.end()) {
		m_subListLock.unlock();
		return false;
	}
	m_subListLock.unlock();
	return true;
}

Strings EventSystem::GetAllEvents()
{
	Strings events;
	for (auto const& imap : m_subscriptionListsByEventName) {
		events.push_back(imap.first);
	}
	return events;
}

void EventSystem::UnsubscribeAllEvents()
{
	m_subListLock.lock();
	Strings events;
	events = GetAllEvents();
	for (int eventsIndex = 0; eventsIndex < events.size(); eventsIndex++) {
		for (int funcIndex = 0; funcIndex < m_subscriptionListsByEventName[events[eventsIndex]].size(); funcIndex++) {
			m_subscriptionListsByEventName[events[eventsIndex]][funcIndex] = nullptr;
		}
	}
	m_subListLock.unlock();
}

void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	g_theEventSystem->SubscribeEventCallbackFunction(eventName, functionPtr);
}

void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	g_theEventSystem->UnsubscribeEventCallbackFunction(eventName, functionPtr);
}

void FireEvent(std::string const& eventName, EventArgs& args)
{
	g_theEventSystem->FireEvent(eventName, args);
}

void FireEvent(std::string const& eventName)
{
	g_theEventSystem->FireEvent(eventName);
}

bool CheckIfAEvent(std::string const& eventName)
{
	return g_theEventSystem->CheckIfAEvent(eventName);
}

Strings GetAllEvents()
{
	return g_theEventSystem->GetAllEvents();
}

void UnsubscribeAllEvents()
{
	g_theEventSystem->UnsubscribeAllEvents();
}
