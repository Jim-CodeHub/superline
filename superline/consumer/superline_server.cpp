/**-----------------------------------------------------------------------------------------------------------------
 * @file	subscriber_server.cpp
 * @brief	Recv message from superline client, Implement with POSX.1 semaphore and shared memory 
 * 
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#include <superline/consumer/superline_server.hpp>

using namespace NS_SUPERLINE;


/*
--------------------------------------------------------------------------------------------------------------------
*
*			                                  FUNCTIONS IMPLEMENT
*
--------------------------------------------------------------------------------------------------------------------
*/

/**
 *	@brief		 Recv data from superline server (producer) 
 *	@param[in]	 data 
 *	@param[out]  None 
 *	@return		 None
 *	@note		 1. The server will block if superline has already empty 
 *				 2. *** EACH DATA SIZE SHALL NOT LARGER THAN BLOCK SIZE
 **/
void *superline_server::recv(void *data)
{
	P(_shminfo.sem_1_spc); /**< Wating for has block on superline */
	P(_shminfo.sem_rdmtx); /**< Multi-process mutex lock */

	memmove(data, _shminfo.offset + _shminfo.m_head->rd_inx * _shminfo.m_head->_size, _shminfo.m_head->_size);

	_shminfo.m_head->rd_inx += 1;
	_shminfo.m_head->rd_inx %= (_shminfo.m_head->blocks); /**< Loop read */

	V(_shminfo.sem_rdmtx); /**< Multi-process mutext unlock */ 
	V(_shminfo.sem_0_spc); /**< Acc free space for superline */

	return data;
}

/**
 *	@brief	    Set subscriber_server deamon 
 *	@param[in]  message - true/false 
 *	@param[out] None
 *	@return		None
 *	@note	    If param message is false, some message will still work on currunt terminal	
 *				, which is helpful for debugging
 **/
void superline_server::set_deamon(bool message)
{
	int ret = daemon(0, message);

	if (-1 == ret) {perror("Socket deamon set failure"); exit(-1);}

	return;
}
