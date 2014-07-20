#ifndef _TRACEREADER_HPP_
#define _TRACEREADER_HPP_

class TrcReader: public Process
{
    public:
	    TrcReader();
	    int Main(int argc, char **argv);
	    static void SignalHandler(eUserSig eSignal);
	private:
	    static Buffer<std::string> logBuffer;
};

#endif
