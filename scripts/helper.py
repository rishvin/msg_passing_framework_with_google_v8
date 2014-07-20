#! /usr/bin/python 

##########################################
#####          helper.py          ########
#####   Written by Rishab Joshi   ########
##########################################

import os
from conf import CC, INC_PATH, AR, REM, PAT, PAT_SUB_LEN, MKDIR, DEP_LIB, FRM_LIB_NAME, OBJ_PATH, LIB_PATH, MOD_PATH

def Print(msg):
    TOTAL_LEN = len(msg) + (PAT_SUB_LEN *2) + 2 #Extra two for white spaces
    print(PAT * TOTAL_LEN)
    print(PAT * PAT_SUB_LEN + ' ' + msg + ' ' + PAT *PAT_SUB_LEN)
    print(PAT *TOTAL_LEN)

def Clean(path):
    CMD = REM + ' -rf ' + path
    os.system(CMD)

def MakeDir(path):
    CMD = MKDIR + ' -p ' + path
    os.system(CMD)

def CreateObjName(file):
    fileName, fileExtension = os.path.splitext(file)
    return str(fileName) + '.o'

def CreateLibOpt(libName):
    return ' -l' + libName

def GenObjs(ext, typePath, objPath):
    for root, dirs, files in os.walk(typePath):
        for file in files:
            if file.endswith(ext):
                 FILE = os.path.join(root, file)
                 TARGET = os.path.join(objPath, CreateObjName(file))
                 CMD = CC + ' -c -I' + INC_PATH + ' -o ' + TARGET + ' ' + FILE
                 ERR = os.system(CMD)
                 if(0 != ERR):
                     return ERR
    return ERR


def GenStaticLib(typePath, libName):
    PATH = os.path.join(typePath, '*.o')
    CMD = AR + ' rcs ' + libName + ' ' + PATH
    ERR = os.system(CMD)
    return ERR

def GenMod(typePath, libList, ModName):
    LIBS_OPT = CreateLibOpt(DEP_LIB)
    LIBS_OPT += CreateLibOpt(FRM_LIB_NAME)
    #for lib in libList: #External list if any
    #LIBS_OPT += CreateLibOpt(lib)
    MOD_SPEC_LIBS = libList
    PATH = os.path.join(typePath, '*.o')
    CMD = CC + ' -o ' + ModName + ' ' + PATH + ' '  + MOD_SPEC_LIBS + ' -L' + LIB_PATH + LIBS_OPT
    #print(CMD)
    ERR = os.system(CMD)
    return ERR

