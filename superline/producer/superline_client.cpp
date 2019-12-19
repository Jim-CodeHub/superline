/**-----------------------------------------------------------------------------------------------------------------
 * @file	subscriber_client.cpp
 * @brief	Send message to superline server, Implement with POSX.1 semaphore and shared memory 
 * 
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#include <superline/producer/superline_client.hpp>

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
 *	@note		 1. The client will block if superline has already full
 *				 2. *** EACH DATA SIZE SHALL NOT LARGER THAN BLOCK SIZE
 **/
void superline_client::send(const void *data)
{
	P(_shminfo.sem_0_spc); /**< Wating for free space on superline */
	P(_shminfo.sem_wrmtx); /**< Multi-process mutex lock */

	memmove(_shminfo.offset + _shminfo.m_head->wr_inx * _shminfo.m_head->_size, data, _shminfo.m_head->_size);

	_shminfo.m_head->wr_inx += 1;
	_shminfo.m_head->wr_inx %= (_shminfo.m_head->blocks); /**< Loop write */

	V(_shminfo.sem_wrmtx); /**< Multi-process mutext unlock */ 
	V(_shminfo.sem_1_spc); /**< Acc non-free space for superline */

	return;
}

