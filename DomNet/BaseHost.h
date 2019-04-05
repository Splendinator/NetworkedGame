#pragma once

#include <cstdint>
#include <thread>

#include "Interfaces\BaseNetwork.h"



namespace domnet {








	class BaseHost
	{
	public:
		BaseHost() { };
		~BaseHost();

		//Checks if any messages have been recieved, and executes functions if they have.

		//This function should be executed by a seperate thread, and should 


		//Returns a message, or nullptr if there are none left.
	


		





	protected:



		//*** MUST BE CALLED AFTER RECIEVING PACKETS ***
		//Populates the message queue with messages from buffer 'buf' of size 'size'
	




		//To be run by seperate thread
		//virtual int listenFunc() = 0;
		
		
		




	

		//CONSIDER MAKING AN OS INDEPENDANT MESSAGE QUEUE IN THIS CLASS
		//THAT ALL OSes CAN INHERIT FROM
	};

	

	

}

