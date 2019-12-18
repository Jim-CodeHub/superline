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
 * @note	1. Single data block read and write are mutually-exclusive 
 *			2. The overall data block transfer is asyn
 *			3. Transferring a single data block only or fast reads and writes results in synchronization 
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
	int rd_inx, wr_inx, blocks, _size; /**< Size of blocks		   */
};

/**
 *	@brief shared memory information 
 **/
struct shminfo{
	struct shmhead *m_head;	/**< shared memory header infor  	   */
	char		   *offset; /**< valid data offset in memory 	   */
	
	int				shmid ; /**< shared memory ID			 	   */
	int			sem_mutex ; /**< mutex semaphore			 	   */
	int			sem_1_spc ; /**< blocks space number in sh-memory  */
	int			sem_0_spc ; /**< empty  space number in sh-memory  */
};

/**
*	@brief super_base class and function set 
 **/
class super_base{
	public:
		super_base();
};


} /* namespace NS_SUPERLINE*/


#endif /*__SUPER_BASE_HPP__*/

