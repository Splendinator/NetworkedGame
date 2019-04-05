#pragma once

#include "Interfaces\BaseNetwork.h"
#include "Message.h"

#include <stdio.h>

#include <iostream>
namespace domnet {
	class MessageQueue {
	public:
		volatile bool locked = false;
		BaseMessage *queue[MESSAGE_QUEUE_SIZE];
		volatile size_t head = 0;
		volatile size_t tail = 0;



		bool isEmpty() { return head == tail; }



		void addMessagesToQueue(char *buf, size_t size)
		{

			locked = true;
			BaseMessage *m = (BaseMessage *)buf;
			size_t i = 0, j = m->size;

			//printf("%x %d\n", buf, size);
		

			while (j <= size) {
				std::cout << "HERE\n";
				//printf("Adding a single message of length %d.\n", m->size);

				addMessage(m);

				i = j;
				m = (BaseMessage *)(buf + i);
				//printf("DEBUG: %d, %d, %d, %d, %d, %d\n", i, j, m->size, m->type, (int)m, size);
				if (j == size) break;
				j += m->size;

			}
			//printf("DEBUG: %d, %d, %d, %d, %d\n", i, j, m->size, m->type, (int)m);

			locked = false;
			//printf("Done!\n");
		}
		int i = 0;
		BaseMessage *getMessage()
		{
			++i;
			while (locked) { }

			return popMessage();
		}



	private:
		void addMessage(BaseMessage *m) {
			queue[(head++) % MESSAGE_QUEUE_SIZE] = m;
			if (head == tail) ++tail;	//If we wrap around the entire queue, overwrite the oldest message.
		}

		//Returns a message, or nullptr if there are none left.
		BaseMessage *popMessage(unsigned char *senderId = nullptr) {
			if (tail == head)
				return nullptr;
			else
			return queue[(tail++) % MESSAGE_QUEUE_SIZE];
		}


	};
}