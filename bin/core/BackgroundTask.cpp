/**
 * @file BackgroundTask.cpp
 * @author Antonius Torode
 * @date 09/17/2026
 * @brief Implements the BackgroundTask methods. This implementation was 
 *        originally defined in the header file but since moved here.
 */

// Include the associated header file.
#include "BackgroundTask.hpp"

#include <iostream>


namespace threading
{
	BackgroundTask::~BackgroundTask() 
	{ 
		stop(); 
	}
	

	void BackgroundTask::start() 
	{ 
		if (!worker.joinable()) // Only start if no thread is running.
			worker = std::thread([this] { this->threadLoop(); });
	}


	void BackgroundTask::stop() 
	{     
		stopRequested = true; // Set the flag unconditionally.
			if (worker.joinable()) 
				worker.join(); // Always join if the thread is joinable.
	}


	bool BackgroundTask::isRunning() const 
	{ 
		return !stopRequested.load(); 
	}
	
	
	void BackgroundTask::setConditionMet(bool value) 
	{  
		conditionMet.store(value); 
	}


	bool BackgroundTask::isConditionMet() const 
	{ 
		return conditionMet.load(); 
	}
	

	void BackgroundTask::toggleCondition()
	{ 
		setConditionMet(!isConditionMet());
		if (context != nullptr && context->verboseMode) 
			std::cout << "TOGGLED BACKGROUND CONDITION: " 
					  << (isConditionMet() ? "1" : "0") << std::endl;
	}
	

	void BackgroundTask::setContext(const RuntimeContext& ctx) 
	{ 
		context = &ctx; 
	}


	void BackgroundTask::doWhenStopped() 
	{}
	

	void BackgroundTask::threadLoop() 
	{ 
		while (!stopRequested.load()) 
			run();
		doWhenStopped();
	}
} // namespace threading
