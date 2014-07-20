#! /usr/bin/python 

##########################################
#####          Framework          ########
#####   Written by Rishab Joshi   ########
##########################################

import os

from scripts.conf import FRM_PATH, FRM_OBJ_PATH, LIB_PATH, FRM_LIB
from scripts.helper import Print, GenObjs, GenStaticLib, MakeDir, Clean

Print("BUILDING FRAMEWORK")

if(False == os.path.exists(FRM_OBJ_PATH)):
    MakeDir(FRM_OBJ_PATH)

Print("BUILDING INTERMEDIATE OBJS")
ERR = GenObjs("cpp", FRM_PATH, FRM_OBJ_PATH)
if(0 != ERR):
    Print("BUILDING INTERMEDIATE OBJS [FAILED]")
    exit(1)
Print("BUILDING INTERMEDIATE OBJS [PASSED]")

Print("BUILDING FRAMEWORK LIB")

ERR = GenStaticLib(FRM_OBJ_PATH, FRM_LIB)
if(0 != ERR):
    Print("BUILDING FRAMEWORK LIB [FAILED]")
    exit(1)

Print("BUILDING FRAMEWORK LIB [PASSED]")

Print("MOVING FRAMEWORK LIB")
if(True == os.path.exists(LIB_PATH)):
    LIB = os.path.join(LIB_PATH, FRM_LIB)
    Clean(LIB)
else:
    MakeDir(LIB_PATH)
CMD = ' mv ' + FRM_LIB + ' ' + LIB_PATH 
os.system(CMD)

