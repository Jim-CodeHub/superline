/**-----------------------------------------------------------------------------------------------------------------
 * @file	superline_server.hpp
 * @brief	Recv message from superline server, Implement with POSX.1 semaphore and shared memory 
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/


#ifndef __SUPERLINE_SERVER_HPP__
#define __SUPERLINE_SERVER_HPP__


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											SUPERLINE_SERVER INCLUDES 
 *
 *------------------------------------------------------------------------------------------------------------------
*/

#include <superline/super_base.hpp>


namespace NS_SUPERLINE{


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											SUPERLINE_SERVER DATA BLOCK
 *
 *------------------------------------------------------------------------------------------------------------------
*/

/**
 *	@brief superline server class and function set 
 **/
class superline_server : public super_base{
	public:
		superline_server(const char *pathname, int proj_id, int blocks, size_t _size)
			:super_base(pathname, proj_id, blocks, _size){};

		void *recv(void *data);
};


} /* namespace NS_SUPERLINE*/


#endif /*__SUPERLINE_SERVER_HPP__*/

