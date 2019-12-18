/**-----------------------------------------------------------------------------------------------------------------
 * @file	subscriber_client.cpp
 * @brief	Send subscriber message to subscribtion server, Implement with POSX.1 semaphore and shared memory 
 * 
 *			productor
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

#include <httpcd/subscriber/subscriber_client.hpp>

using namespace NS_HTTPCD;


/*
--------------------------------------------------------------------------------------------------------------------
*
*			                                  VARIABLE DECLARATION 
*
--------------------------------------------------------------------------------------------------------------------
*/



/*
--------------------------------------------------------------------------------------------------------------------
*
*			                                  FUNCTIONS IMPLEMENT
*
--------------------------------------------------------------------------------------------------------------------
*/

/**
 *	@brief		 Init subscriber with POSIX.1 semaphore and shared memory 
 *	@param[in]	 None 
 *	@param[out]  None 
 *	@return		 None
 *	@exception   "Const char *", subscriber SHALL start first
 **/
subscriber_client::subscriber_client(const char *pathname, int proj_id, size_t _size)
{
	/*-----------------------------------------------------------------------------*/
	/**< Get IPC key */

    key_t key = ftok(pathname, proj_id);

	if (-1 == key) { perror("Subscriber ftok"); exit(-1); }

	/*-----------------------------------------------------------------------------*/
	/**< Open shared memory that created by subscriber server */

    int shmid = shmget(key, _size, IPC_CREAT);

	if (-1 == shmid) { perror("Subscriber shmget"); exit(-1); }

    struct shmid_ds buf;

    if (-1 == shmctl(shmid, IPC_STAT, &buf)) { perror("Subscriber shmctl"); exit(-1); }

    if(buf.shm_cpid == getpid())
    {
		throw ("Subscriber server SHALL start first ");
    }

	/*-----------------------------------------------------------------------------*/
	/**< Connect to the shared memory space */

    this->shmaddr = (char *)shmat(shmid, NULL, 0);

	if (this->shmaddr == (void *)-1) { perror("Subscriber shmat"); exit(-1); }

	/*-----------------------------------------------------------------------------*/
	/**< Open semaphore that created by subscriber server */

	this->semid = semget(key, 0, IPC_CREAT);

	if (-1 == semid) { perror("Subscriber semget"); exit(-1); }
}

/**
 *	@brief		 Subscriber destructure 
 *	@param[in]	 None 
 *	@param[out]  None 
 *	@return		 None
 **/
subscriber_client::~subscriber_client()
{
	shmdt(this->shmaddr); /**< Detaches shared memory connection */
}


#if 0
    int ret, shmid;
    key_t key;
    char *shmaddr;
    struct shmid_ds buf;

    key = ftok(jmfConf.Key_Path.c_str(), jmfConf.Project_ID);
    DEBUG(key, ==, -1, "ftok error ", -1);

    shmid = shmget(key, jmfConf.Max_ShareMemory, IPC_CREAT);
    DEBUG(shmid, ==, -1, "shmget error ", -1);

    ret = shmctl(shmid, IPC_STAT, &buf);
    DEBUG(ret, ==, -1, "shmctl error ", -1);

    if(buf.shm_cpid == getpid())
    {
        puts("JMFSignaler haven't start, funciton registerSignal failure!"); 
        exit(-1);
    }

    shmaddr = (char *)shmat(shmid, NULL, 0);
    DEBUG(shmaddr, ==, (void *)-1, "shmat error ", -1);

    strcpy(shmaddr, query.ToString().getBytes());

    kill(buf.shm_cpid, SIGALRM);

    ret = shmdt(shmaddr);
    DEBUG(ret , ==, -1, "shmdt error ", -1);
#endif

#if 0
void subscriber::init(void)
{
	if (true == subscriber_emit_flag) { return; }; /**< Easy progress protect */

	subscriber_emit_flag = true;

	//fpathconf(pipefd[0], _PC_PIPE_BUF) //get pipe buffer size, > size , linux will not keep atom 

	signal(SIGCHLD, SIG_IGN); /**< Ignore child signal */

	switch (fork())
	{
		case -1: /**< PID = -1; fork error */
			perror("Subscriber pipe fork");	exit(-1);
			break;

		case  0: /**< Child progress  */
			close(pipefd[1]); /**< Close unused write end */

			while (true)
			{
				//while (read(pipefd[0], &buf, 1) > 0) //BLOCK read, until data comming, and end until read <= 0
				//									   //THIS STYLE WILL BLOCK UNTIL father close write fd
				//read(pipefd[])
				//DO SOMETHINE BELOW AND DO NOT CLOSE IT SELF ANY MORE
			}

			break;

		default: /**< Father progress */
			close(pipefd[0]); /**< Close unused read end  */

			//write(pipefd[1], buff, buffsize)); //BOLCK write, the functio will work , util pipe is empty
			//											  //To be sure,  do not block progress, just read it, as long as data comming
			//											  //AND DO NOT CLOSE IT ANY MORE

			;
	}

	return;
}
#endif

