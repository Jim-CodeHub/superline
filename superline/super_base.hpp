/**-----------------------------------------------------------------------------------------------------------------
 * @file	super_base.hpp
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


#ifndef __SUPER_BASE_HPP__
#define __SUPER_BASE_HPP__


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											SUPER_BASE INCLUDES 
 *
 *------------------------------------------------------------------------------------------------------------------
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>


namespace NS_SUPERLINE{


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											SUPER_BASE DATA BLOCK
 *
 *------------------------------------------------------------------------------------------------------------------
*/

/**
 *	@brief shared memory header information 
 **/
struct shmhead{
	int rd_inx , wr_inx, blocks, _size; /**< Size of blocks		   */
};

union semun { int val; }; 

/**
 *	@brief shared memory information 
 **/
struct shminfo{
	key_t		   ipc_key; /**< IPC key						   */
	int				shmid ; /**< shared memory ID			 	   */

	struct shmhead *m_head;	/**< shared memory header info  	   */
	char		   *offset; /**< valid data offset in memory 	   */
	
	int			sem_rdmtx ; /**< read mutex semaphore			   */
	int			sem_wrmtx ; /**< wrtie mutex semaphore			   */
	int			sem_1_spc ; /**< blocks space number in sh-memory  */
	int			sem_0_spc ; /**< empty  space number in sh-memory  */
};

/**
*	@brief super_base class and function set 
 **/
class super_base{
	public:
		void	 free(void);

		int get_shmid(void);
		int get_semrd(void);
		int get_semwr(void);
		int get_sems1(void);
		int get_sems0(void);

	protected:
		super_base(const char *pathname, int proj_id, int blocks, int _size);
		~super_base();

		void init(const char *pathname, int proj_id, int blocks, int _size);

	protected:
		void P(int semid);
		void V(int semid);

	protected:
		struct shminfo _shminfo; 
};


} /* namespace NS_SUPERLINE*/


#endif /*__SUPER_BASE_HPP__*/

