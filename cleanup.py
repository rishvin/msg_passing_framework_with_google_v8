#! /usr/bin/python 

##########################################
#####           CleanUp           ########
#####   Written by Rishab Joshi   ########
##########################################

import os, sys
from scripts.conf import OBJ_PATH, FRM_OBJ_PATH, MOD_OBJ_PATH
from scripts.helper import Print, Clean, MakeDir


ARGC = len(sys.argv)
USAGE = "Usage: clean <all|framework|modules>"

if(2 != ARGC):
    print(USAGE)
    exit(0)

if("all" == sys.argv[1]):
    Print("CLEANING ALL")
    Clean(OBJ_PATH)
elif("framework" == sys.argv[1]):
    Print("CLEANING FRAMEWORK")
    Clean(FRM_OBJ_PATH)
elif("modules" == sys.argv[1]):
    Print("CLEANING MODULES")
    Clean(MOD_OBJ_PATH)
else:
    print(USAGE)



