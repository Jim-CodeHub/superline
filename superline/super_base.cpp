/**-----------------------------------------------------------------------------------------------------------------
 * @file	super_base.cpp
 * @brief	Producer - Consumer C++ library in system env with semphare and shared memory under POIX.1 API
 *
 *			 producer
 *				|+
 *				----------------------------------------------
 *				0 0	0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
 *				----------------------------------------------
 *															|-
 *														 consumer
 *
 * @note	Same Single data block read and write are mutually-exclusive 
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#include <superline/super_base.hpp>

using namespace NS_SUPERLINE;


/*
--------------------------------------------------------------------------------------------------------------------
*
*			                                  FUNCTIONS IMPLEMENT
*
--------------------------------------------------------------------------------------------------------------------
*/

/**
 *	@brief		 Init superline base info
 *	@param[in]	 pathname - project path with file name  
 *	@param[in]   proj_id  - project id 
 *	@param[in]   _size	  - size of shared memeory (super line size)
 *	@param[in]   blocks   - Hold the number of products
 *	@return		 None
 *	@note		 Super line can only be create by one end, SET 0 for blocks if it's an open end
 **/
super_base::super_base(const char *pathname, int proj_id, int blocks, size_t _size)
{
	/*-----------------------------------------------------------------------------*/
	/**< 1 - 0 Get IPC key */

	this->_shminfo.ipc_key = ftok(pathname, proj_id);

	if (-1 == _shminfo.ipc_key) { perror("ftok"); exit(-1); }

	/*-----------------------------------------------------------------------------*/
	/**< 1 - 1 Open OR Create shared memory */ 

    this->_shminfo.shmid = shmget(_shminfo.ipc_key, _size, IPC_CREAT);

	if (-1 == _shminfo.shmid) { perror("shmget"); exit(-1); }

	/*-----------------------------------------------------------------------------*/
	/**< 1 - 2 Connect to the shared memory space */

    this->_shminfo.m_head = (struct shmhead *)shmat(_shminfo.shmid, NULL, 0);

	if (_shminfo.m_head == (void *)-1) { perror("shmat"); exit(-1); }

	if (0 != blocks) /**< Set data if superline has not been initialized */ 
	{
		_shminfo.m_head->blocks = blocks;		   /**< Hold the number of product */
		_shminfo.m_head->_size  = new int[blocks]; /**< size info of each block	   */
		_shminfo.m_head->rd_inx = 0;			   /**< Read index in the memory   */
		_shminfo.m_head->wr_inx = 0;			   /**< Write index in the memory  */
	}
	
	this->_shminfo.offset = (char *)(_shminfo.m_head + 1);/**< Set valid address   */ 

	/*-----------------------------------------------------------------------------*/
	/**< 2 - 0 Open OR Create semapore */ 

	this->_shminfo.sem_rdmtx = semget(_shminfo.ipc_key + 0, 1, IPC_CREAT);
	this->_shminfo.sem_wrmtx = semget(_shminfo.ipc_key + 1, 1, IPC_CREAT);
	this->_shminfo.sem_1_spc = semget(_shminfo.ipc_key + 2, 1, IPC_CREAT);
	this->_shminfo.sem_0_spc = semget(_shminfo.ipc_key + 3, 1, IPC_CREAT);

	if (0 != blocks) /**< Set data if superline has not been initialized */ 
	{
		union semun { int val; };

		union semun un_mutex = {1}, un_1_spc = {0}, un_0_spc = {blocks};

		int ret = semctl(_shminfo.sem_rdmtx, 0, SETVAL, un_mutex);
			ret = semctl(_shminfo.sem_wrmtx, 0, SETVAL, un_mutex);
			ret = semctl(_shminfo.sem_1_spc, 0, SETVAL, un_1_spc);
			ret = semctl(_shminfo.sem_0_spc, 0, SETVAL, un_0_spc);

		if (-1 == ret) { perror("shmctl"); }
	}
}

/**
 *	@brief		 Destroy superline  
 *	@param[in]	 None 
 *	@param[out]	 None
 *	@return		 None
 *	@note		 The function SHALL be called by who create the superline 
 **/
void super_base::free(void)
{
	delete [] this->_shminfo.m_head->_size; /**< Free blocks size info			  */

	shmdt(_shminfo.m_head);	/**< Detaches from shared memory					  */	 
	_shminfo.m_head = NULL;

	shmctl(_shminfo.shmid, IPC_RMID, 0); /**< Release shared memory               */

	semctl(_shminfo.sem_rdmtx, 0, IPC_RMID, 0);	/**< Delete semaphore read mutex  */ 
	semctl(_shminfo.sem_wrmtx, 0, IPC_RMID, 0);	/**< Delete semaphore write mutex */ 
	semctl(_shminfo.sem_1_spc, 0, IPC_RMID, 0);	/**< Delete semaphore spc_1		  */ 
	semctl(_shminfo.sem_0_spc, 0, IPC_RMID, 0);	/**< Delete semaphore spc_0		  */ 
}

/**
 *	@brief		 Semaphore P operation -> sem-- 
 *	@param[in]	 semid - semaphore ID 
 *	@param[out]	 None
 *	@return		 None
 **/
void super_base::P(int semid)
{
	struct sembuf s[1] = {0, -1, 0};

	if (semop(semid, s, 1) == -1) { perror("semop"); exit(-1); }

	return;
}

/**
 *	@brief		 Semaphore V operation -> sem++
 *	@param[in]	 semid - semaphore ID 
 *	@param[out]	 None
 *	@return		 None
 **/
void super_base::V(int semid)
{
	struct sembuf s[1] = {0,  1, 0};

	if (semop(semid, s, 1) == -1) { perror("semop"); exit(-1); }

	return;
}

