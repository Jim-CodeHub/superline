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
 * @note	1. Single data block read and write are mutually-exclusive 
 *			2. The overall data block transfer is asyn
 *			3. Transferring a single data block only or fast reads and writes results in synchronization 
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
 *	@param[in]	 None 
 *	@param[out]  None 
 *	@return		 None
 **/
super_base::super_base()
{
}

