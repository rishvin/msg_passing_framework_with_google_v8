#include "common.hpp"
#include "entry.hpp"
#include "control.hpp"

#define SPAWN     "spawn(\"<Process Name>\", \"<IP>\", \"<Port>\")"
#define KILL      "kill(\"<PID>\", \"<Signal>\")"
#define SEND_MSG  "send_msg(\"<IP>\", \"<Port>\", \"<Msg>\")"

ENTRY(Control);


Control::Control()
    :Process("Control"),
    pDispatcher(NULL)
{
    pIsolate = v8::Isolate::GetCurrent();
}

Control::~Control()
{
    DispatchService::Delete();
}

void Control::Help(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    const char  * cData = 
		"\n############# Help #############\n"
		"\nSpawn Process: " SPAWN
		"\nKill Process:  " KILL
		"\nSend Message:  " SEND_MSG ;
		
	args.GetReturnValue().Set(v8::String::New(cData));
}

void Control::Spawn(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if(3 != args.Length())
    {
        args.GetReturnValue().Set(v8::String::New("Usage: " SPAWN));
		return;
    }
 
	v8::String::AsciiValue procObj(args[0]);
	v8::String::AsciiValue procIP(args[1]);
	v8::String::AsciiValue procPort(args[2]);
	
	char *pcArgs[4];
    pcArgs[0] = *procObj;
	pcArgs[1] = *procIP;
	pcArgs[2] = *procPort;
	pcArgs[3] = NULL;
	
	Control *pProc = ProcService::Get();
	int  iResult = static_cast<int>(pProc->SpawnProcess(pcArgs[0], pcArgs));
	char cBuffer[32] = {'\0'};
	sprintf(cBuffer, "Spawned Process: %d", iResult);
	args.GetReturnValue().Set(v8::String::New(cBuffer));
}   


void Control::Kill(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if(2 != args.Length())
    {
        args.GetReturnValue().Set(v8::String::New("Usage: " KILL));
		return;
    }

	int iPid = static_cast<int>(args[0]->Int32Value());
	int iSignal = static_cast<int>(args[1]->Int32Value());

	int iRet = static_cast<int>(kill(iPid, iSignal));
	if(-1 == iRet)
	{
	    iRet = errno;
	}

    char cBuffer[32] = {'\0'};
	sprintf(cBuffer, "kill retcode: %d", iRet);
	args.GetReturnValue().Set(v8::String::New(cBuffer));
}


void Control::Dispatch(unsigned int &uAddr, unsigned short &usPort, short sFamily, char *pcMsg)
{
    SockAddr serverAddr(uAddr , usPort, sFamily);
	dHandle(serverAddr);
	
	TLV tlv;
    tlv.tl.iType   = 2;
    tlv.tl.uLength = strlen(pcMsg);
    tlv.pcValue    = pcMsg;	
	unsigned int uMsgLen = 0;
	char *pMsg = NULL;

	(void)MsgOper::Create(pMsg, uMsgLen, tlv);
	if(NULL != pMsg)
	{
	    Control *pProc = ProcService::Get();
		pDispatcher->Send(pMsg, dHandle);
	}
}
void Control::SendMsg(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if(3 != args.Length())
    {
        args.GetReturnValue().Set(v8::String::New("Usage: " SEND_MSG));
		return;
    }
    
    unsigned int uAddr = args[0]->Int32Value();
	unsigned short usPort = htons(args[1]->Int32Value());
	short sFamily = AF_INET;
	v8::String::AsciiValue sMsg(args[2]);
	char *pcMsg = *sMsg;
    Control *pProc = ProcService::Get();
	pProc->Dispatch(uAddr, usPort, sFamily, pcMsg);
	args.GetReturnValue().Set(0);
}
		    

void Control::CreateTemplate()
{
    v8::Handle<v8::ObjectTemplate> handObject = v8::ObjectTemplate::New();
	handObject->Set(v8::String::New("help"), v8::FunctionTemplate::New(Control::Help));	
	handObject->Set(v8::String::New("spawn"), v8::FunctionTemplate::New(Control::Spawn));
	handObject->Set(v8::String::New("kill"), v8::FunctionTemplate::New(Control::Kill));
	handObject->Set(v8::String::New("send_msg"), v8::FunctionTemplate::New(Control::SendMsg));
	handContext = v8::Context::New(pIsolate, NULL, handObject);
	assert(false == handContext.IsEmpty());
}

void Control::ExecuteCommand(v8::Handle<v8::String> sCommand, 
                          v8::Handle<v8::String> vShellName)
{
	v8::HandleScope(pIsolate);
	v8::TryCatch TryCatch;
	v8::Handle<v8::Script> handScript = v8::Script::Compile(sCommand);
	if(true == handScript.IsEmpty())
	{	    
		v8::Handle<v8::Value> handException = TryCatch.Exception();
		v8::String::AsciiValue sException(handException);
		printf("Response: %s", "None");
        return;   
	}
	v8::Handle<v8::Value> vResult = handScript->Run();
	if(true == vResult.IsEmpty())
	{
		v8::Handle<v8::Value> handException = TryCatch.Exception();
		v8::String::AsciiValue sException(handException);
		printf("\n>> Exception: %s", *sException);
	}
	else
	{
	    if(true == vResult->IsString())
	    {
		    printf("Response: %s", *v8::String::AsciiValue(vResult));
	    }
		else
		if(true == vResult->IsInt32())
		{
			printf("Response: %d", vResult->Int32Value());
		}
	}
}

void Control::InitShell()
{
    v8::Local<v8::String> sShellName(v8::String::New("(Config)"));
	v8::Context::Scope contextScope(handContext);
	printf("\n #### Config Shell ####\n");
    while(true)
    {
        printf("\n>> ");
		char cBuffer[256];
		char *pChar = fgets(cBuffer, sizeof(cBuffer), stdin);
		v8::HandleScope handle_scope(pIsolate);
		ExecuteCommand(v8::String::New(pChar), sShellName);
    }
}
		 
		
int Control::Main(int iArgc, char **ppArgv)
{
    
    pDispatcher = DispatchService::Create();
    assert(NULL != pDispatcher);
    pDispatcher->Init(); 
	
    v8::HandleScope(pIsolate);
    CreateTemplate();
	handContext->Enter();
	InitShell();
	 v8::V8::Dispose();
    return 0;
}
