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
 *	@brief		 Constructure 
 *	@brief		 Init superline base info
 *	@param[in]	 pathname - project path with file name  
 *	@param[in]   proj_id  - project id 
 *	@param[in]	 blocks	  -  product number in shared memory
 *	@param[in]   _size	  - size of each blocks 
 *	@return		 None
 *	@note		 Param 'blocks' and '_size' will be invalid if the shared memory has already exsit.
 **/
super_base::super_base(const char *pathname, int proj_id, int blocks, int _size)
{
	this->init(pathname, proj_id, blocks, _size);
}

/**
 *	@brief		 Destructure 
 *	@param[in]   None 
 *	@param[out]  None 
 *	@return		 None
 **/
super_base::~super_base()
{
	/**< Detaches shared memory */ 
	if (-1 == shmdt(_shminfo.m_head)) { perror ("shmdt"); exit(-1); }
}

/**
 *	@brief		 Init superline base info
 *	@param[in]	 pathname - project path with file name  
 *	@param[in]   proj_id  - project id 
 *	@param[in]	 blocks	  -  product number in shared memory
 *	@param[in]   _size	  - size of each blocks 
 *	@return		 None
 *	@note		 Param 'blocks' and '_size' will be invalid if the shared memory has already exsit.
 **/
void super_base::init(const char *pathname, int proj_id, int blocks, int _size)
{
	union semun un_1_spc = {0}, un_0_spc = {blocks}, un_mutex = {1};

	/*-----------------------------------------------------------------------------------------*/
	/**< 1 - 0 Get IPC key */

	if (-1 == (this->_shminfo.ipc_key = ftok(pathname, proj_id))){
		perror("ftok"); exit(-1);}

	int shmid = -1;  /** If shared memory has already exsit */
	if (-1 != (shmid = shmget(this->_shminfo.ipc_key, 0, 0))) { goto _LOAD; }

	/*-----------------------------------------------------------------------------------------*/
	/**< 1 - 1 Create shared memory */ 

    if (-1 == (this->_shminfo.shmid = shmget(_shminfo.ipc_key, _size*blocks, IPC_CREAT|0644))){
		perror("shmget"); exit(-1);}

	/*-----------------------------------------------------------------------------------------*/
	/**< 1 - 2 Connect to the shared memory space */

    if ((void *)-1 == (this->_shminfo.m_head = (struct shmhead *)shmat(_shminfo.shmid, NULL, 0))){
		perror("shmat"); exit(-1);}

	this->_shminfo.offset = (char *)(_shminfo.m_head + 1); /**< Set valid address  */ 

	/*-----------------------------------------------------------------------------------------*/
	/**< 2 - 0 Create semapore */ 

	if (-1 == (this->_shminfo.sem_rdmtx = semget(_shminfo.ipc_key + 0, 1, IPC_CREAT|0644))){
		perror("semget"); exit(-1);}

	if (-1 == (this->_shminfo.sem_wrmtx = semget(_shminfo.ipc_key + 1, 1, IPC_CREAT|0644))){
		perror("semget"); exit(-1);}

	if (-1 == (this->_shminfo.sem_1_spc = semget(_shminfo.ipc_key + 2, 1, IPC_CREAT|0644))){
		perror("semget"); exit(-1);}

	if (-1 == (this->_shminfo.sem_0_spc = semget(_shminfo.ipc_key + 3, 1, IPC_CREAT|0644))){
		perror("semget"); exit(-1);}

	/*-----------------------------------------------------------------------------------------*/
	/**< 3 - 0 Set values of shared memory header info */ 

	this->_shminfo.m_head->rd_inx = 0;		/**< Read index in the memory   */
	this->_shminfo.m_head->wr_inx = 0;		/**< Read index in the memory   */
	this->_shminfo.m_head->blocks = blocks; /**< Hold the number of product */
	this->_shminfo.m_head->_size  = _size;	/**< Size of each blocks        */

	/*-----------------------------------------------------------------------------------------*/
	/**< 3 - 1 Set values of semaphore info */ 

	if (-1 == semctl(this->_shminfo.sem_rdmtx, 0, SETVAL, un_mutex)){
		perror("semctl"); exit(-1);}

	if (-1 == semctl(this->_shminfo.sem_wrmtx, 0, SETVAL, un_mutex)){
		perror("semctl"); exit(-1);}

	if (-1 == semctl(this->_shminfo.sem_1_spc, 0, SETVAL, un_1_spc)){
		perror("semctl"); exit(-1);}

	if (-1 == semctl(this->_shminfo.sem_0_spc, 0, SETVAL, un_0_spc)){
		perror("semctl"); exit(-1);}

	return;
_LOAD:
	/*-----------------------------------------------------------------------------------------*/
	/**< 1 - 1 Connect to the shared memory space */

    if ((void *)-1 == (this->_shminfo.m_head = (struct shmhead *)shmat(shmid, NULL, 0))){
		perror("shmat"); exit(-1);}

	this->_shminfo.offset = (char *)(_shminfo.m_head + 1); /**< Set valid address  */ 

	/*-----------------------------------------------------------------------------------------*/
	/**< 1 - 2 Set shared memory id */ 

	this->_shminfo.shmid = shmid;

	/*-----------------------------------------------------------------------------------------*/
	/**< 1 - 3 Open semaphore */ 

	if (-1 == (this->_shminfo.sem_rdmtx = semget(_shminfo.ipc_key + 0, 0, IPC_CREAT|0644))){
		perror("semget"); exit(-1);}

	if (-1 == (this->_shminfo.sem_wrmtx = semget(_shminfo.ipc_key + 1, 0, IPC_CREAT|0644))){
		perror("semget"); exit(-1);}

	if (-1 == (this->_shminfo.sem_1_spc = semget(_shminfo.ipc_key + 2, 0, IPC_CREAT|0644))){
		perror("semget"); exit(-1);}

	if (-1 == (this->_shminfo.sem_0_spc = semget(_shminfo.ipc_key + 3, 0, IPC_CREAT|0644))){
		perror("semget"); exit(-1);}

	return;
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

/**
 *	@brief		 Manually destructure
 *	@param[in]	 None 
 *	@param[out]	 None
 *	@return		 None
 *	@node		 Call the function, JUST ONLY when all calling processes SHALL ended 
 **/
void super_base::free(void)
{
	/*-----------------------------------------------------------------------------------------*/
	/**< 1 - 0 Release shared memory */ 

	if (-1 == shmctl(_shminfo.shmid, IPC_RMID, 0)){
		perror ("shmctl"); exit(-1);}

	/*-----------------------------------------------------------------------------------------*/
	/**< 1 - 1 Release semaphore */ 

	if (-1 == semctl(_shminfo.sem_rdmtx, 0, IPC_RMID, 0)){
		perror ("semctl"); exit(-1);}

	if (-1 == semctl(_shminfo.sem_wrmtx, 0, IPC_RMID, 0)){
		perror ("semctl"); exit(-1);}

	if (-1 == semctl(_shminfo.sem_1_spc, 0, IPC_RMID, 0)){
		perror ("semctl"); exit(-1);}

	if (-1 == semctl(_shminfo.sem_0_spc, 0, IPC_RMID, 0)){
		perror ("semctl"); exit(-1);}

	return;
}

