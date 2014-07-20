#ifndef _CONTROL_HPP_
#define _CONTROL_HPP_

#include "v8.h"


class Control:public Process
{
    public:
        Control();
		~Control();
		void CreateTemplate();
		static void Help(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void Spawn(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void Kill(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void SendMsg(const v8::FunctionCallbackInfo<v8::Value>& args);
		void InitShell();
		void ExecuteCommand(v8::Handle<v8::String> sCommand, 
                          v8::Handle<v8::String> vShellName);
        int Main(int argc, char **argv);
    private:
		void Dispatch(unsigned int &uAddr, unsigned short &usPort, short sFamily, char *pcMsg);
		v8::Isolate *pIsolate;
		v8::Handle<v8::Context> handContext;
		DispatchHandle dHandle;
		DispatchHandler *pDispatcher;
};
#endif


