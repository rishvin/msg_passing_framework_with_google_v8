#ifndef _PUBLIC_HPP_
#define _PUBLIC_HPP_

#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>
#include <sstream> 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <sys/time.h>
#include <vector>
#include <assert.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <poll.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
#include <pthread.h>

#include "lock.hpp"
#include "buffer.hpp"
#include "singleton.hpp"
#include "file.hpp"

#include "threads.hpp"
#include "process.hpp"

#include "log.hpp"
#include "socket.hpp"
#include "message.hpp"
#include "messagehandler.hpp"

using namespace std;

typedef Singleton<LogWriter> LogWriterService;

#define TRACE_INIT_EXCEPTION "Trace Init[FAILED]"

#define TRACE(THIS, MSG, PARAM) do{                                                                                       \
    std::stringstream sTrc;                                                                                               \
    LogWriter *pLogWriter = LogWriterService::Get();                                                                      \
    sTrc << std::endl << THIS->GetName() << " @ " << __DATE__<< "|" << __TIME__ << ":: " << MSG << "["<<(PARAM) << "]";   \
    pLogWriter->Write(sTrc);                                                                                              \
}while(0)
#endif
