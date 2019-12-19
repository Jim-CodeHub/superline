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
 *	@brief		 Send data to superline server (consumer) 
 *	@param[in]	 data 
 *	@param[out]  None 
 *	@return		 None
 *	@note		 1. The server will block if superline has already empty 
 *				 2. The server can be called with multi-process
 **/
void *superline_server::recv(void *data)
{
	P(_shminfo.sem_1_spc); /**< Wating for has block on superline */
	P(_shminfo.sem_rdmtx); /**< Multi-process mutex lock */

	memmove(data, _shminfo.m_head + _shminfo.m_head->rd_inx * _shminfo.m_head->_size[_shminfo.m_head->rd_inx]
			,_shminfo.m_head->_size[_shminfo.m_head->rd_inx]);

	_shminfo.m_head->rd_inx += 1;
	_shminfo.m_head->rd_inx %= (_shminfo.m_head->blocks); /**< Loop read */

	V(_shminfo.sem_rdmtx); /**< Multi-process mutext unlock */ 
	V(_shminfo.sem_0_spc); /**< Acc free space for superline */

	return data;
}
