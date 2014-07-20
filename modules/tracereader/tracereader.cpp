#include "common.hpp"
#include "entry.hpp"
#include "tracereader.hpp"

Buffer<std::string> TrcReader::logBuffer;

ENTRY(TrcReader);

TrcReader::TrcReader()
    :Process("TrcReader")
{
}

int TrcReader::Main(int iArgc, char ** ppArgv)
{
    RegisterSigHandler(SIGUSER1, TrcReader::SignalHandler);
	RegisterSigHandler(SIGINTR, TrcReader::SignalHandler);
	LogReader logReader(logBuffer);
	logReader.Open();
	InternalWait();
	    
}


void TrcReader::SignalHandler(eUserSig eSignal)
{
    switch(eSignal)
    {
        case SIGUSER1:	
        {
			std::string sTrc;
            {
		        MutexGuard mutex(TrcReader::logBuffer);
	            while(false == TrcReader::logBuffer.IsEmpty())
	            {
	                sTrc.append(TrcReader::logBuffer.PopFront());
	            }
            }
            cerr<<sTrc;
			break;
        }
		case SIGINTR:
		{
		    ProcService proc;
			TrcReader *pThis = proc.Get();
			pThis->InternalSignal();
			break;
		}
    }
}