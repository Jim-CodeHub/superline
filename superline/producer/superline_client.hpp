/**-----------------------------------------------------------------------------------------------------------------
 * @file	superline_client.hpp
 * @brief	Send message to superline server, Implement with POSX.1 semaphore and shared memory 
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/


#ifndef __SUPERLINE_CLIENT_HPP__
#define __SUPERLINE_CLIENT_HPP__


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											SUPERLINE_CLIENT INCLUDES 
 *
 *------------------------------------------------------------------------------------------------------------------
*/

#include <superline/super_base.hpp>


namespace NS_SUPERLINE{


/*------------------------------------------------------------------------------------------------------------------
 * 
 *											SUPERLINE_CLIENT DATA BLOCK
 *
 *------------------------------------------------------------------------------------------------------------------
*/

/**
 *	@brief superline client class and function set 
 *	@note  The client can be called with multi-process (in different object)
 **/
class superline_client : public super_base{
	public:
		superline_client(const char *pathname, int proj_id):super_base(pathname, proj_id, 0, 0){};

		void send( const void *data, int size );

	private:
};


} /* namespace NS_SUPERLINE*/


#endif /*__SUPERLINE_CLIENT_HPP__*/

