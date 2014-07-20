#! /usr/bin/python 

##########################################
#####            Modules          ########
#####   Written by Rishab Joshi   ########
##########################################

import os
from scripts.conf import MOD_OBJ_PATH, MOD_PATH, BIN_PATH
from scripts.helper import Print, GenObjs, GenMod, MakeDir, Clean

MOD_LIST = {'master', 'node1', 'node2', 'tracereader', 'control'}

#Mention external lib dependeny if any
EXT_LIBS = {
		'master'      : '',
		'node1'       : '',
		'node2'       : '',
		'tracereader' : '',
		'control'     : 'modules/control/lib/libv8_base.ia32.a modules/control/lib/libv8_snapshot.a'
	   }

Print("BUILDING MODULES")

for module in MOD_LIST:
    Print("BUILDING " + module)
    Print("BUILDING INTERMEDIATE OBJS")

    MODULE_PATH = os.path.join(MOD_PATH, module)
    MODULE_OBJ_PATH = os.path.join(MOD_OBJ_PATH, module)
    #print(MODULE_OBJ_PATH)
    if(False == os.path.exists(MODULE_OBJ_PATH)):
        MakeDir(MODULE_OBJ_PATH)

    ERR = GenObjs("cpp", MODULE_PATH, MODULE_OBJ_PATH)
    if(0 != ERR):
        Print("BUILDING INTERMEDIATE OBJS [FAILED]")
        exit(0)

    Print("BUILDING INTERMEDIATE OBJS [PASSED]")

    Print("BUILDING EXECUTABLE")
    EXT_LIB = EXT_LIBS[module]
    ERR = GenMod(MODULE_OBJ_PATH, EXT_LIB, module)
    if(0 != ERR):
        Print("BUILDING EXECUTABLE [FAILED]")
        exit(0)

    Print("BUILDING " + module + " [PASSED]")

    Print("MOVING " + module)
    if(True == os.path.exists(BIN_PATH)):
        EXE = os.path.join(BIN_PATH, module)
        Clean(EXE)
    else:
        MakeDir(BIN_PATH)
    CMD = ' mv ' + module + ' ' + BIN_PATH 
    os.system(CMD)

