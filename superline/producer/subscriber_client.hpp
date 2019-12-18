/**-----------------------------------------------------------------------------------------------------------------
 * @file	subscriber_client.hpp
 * @brief	Send subscriber message to subscribtion server, Implement with POSX.1 semaphore and shared memory 
 *
 *			productor
 *				|+
 *				----------------------------------------------
 *				0 0	0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
 *				----------------------------------------------
 *															|-
 *														 consumer
 *
 * @note	1. Single data block read and write are mutually-exclusive 
 *			2. The overall data block transfer is asyn
 *			3. Transferring a single data block only or fast reads and writes results in synchronization 
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/


#ifndef __SUBSCRIBER_CLIENT_HPP__
#define __SUBSCRIBER_CLIENT_HPP__


/*------------------------------------------------------------------------------------------------------------------
 * 
 *										CIP4/SUBSCRIBER_CLIENT INCLUDES 
 *
 *------------------------------------------------------------------------------------------------------------------
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <httpcd/http_client.hpp>


namespace NS_HTTPCD{


/*------------------------------------------------------------------------------------------------------------------
 * 
 *										CIP4/SUBSCRIBER_CLIENT DATA BLOCK
 *
 *------------------------------------------------------------------------------------------------------------------
*/

/**
 *	@brief httpcd subscriber client class and function set 
 **/
class subscriber_client : public http_client{
	public:
		subscriber_client(const char *pathname, int proj_id, size_t _size=1024*1024);

	private:
		char *shmaddr;
		int semid;
};


} /* namespace NS_HTTPCD*/


#endif /*__SUBSCRIBER_CLIENT_HPP__*/

