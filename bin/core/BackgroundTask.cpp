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
	
	
    bool BackgroundTask::hasFailed() const noexcept
    {
        return static_cast<bool>(storedException);
    }
    
    
    std::exception_ptr BackgroundTask::getException() const
    {
        return storedException;
    }


    void BackgroundTask::rethrowExceptionIfAny() const
    {
        if (storedException)
        {
            std::cerr << "[BackgroundTask"
	                  << (!taskName.empty() ? ":" + taskName : "" )
	                  << "] rethrowExceptionIfAny() — rethrowing now!" << std::endl;
            std::rethrow_exception(storedException);
        }
    }


	void BackgroundTask::doWhenStopped() 
	{}
	
	
	
    void BackgroundTask::setTaskName(const std::string& newTaskName)
    {
        taskName = newTaskName;
    }
	

	void BackgroundTask::threadLoop() 
	{
	    try
	    { // run() could potentially throw, which is in a separate thread.
		    while (!stopRequested.load()) 
			    run();
	    }
	    catch (...)
	    {
	        std::cerr << "[BackgroundTask" 
	                  << (!taskName.empty() ? ":" + taskName : "") 
	                  << "] Caught exception in worker thread!" << std::endl;
            // Capture whatever was thrown (MIAException, std::exception, or unknown).
            storedException = std::current_exception();
            stopRequested.store(true);
	    }
	    
		doWhenStopped();
	}
} // namespace threading
