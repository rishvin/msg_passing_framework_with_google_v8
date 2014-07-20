#! /usr/bin/python 

##########################################
#####            conf.py          ########
#####   Written by Rishab Joshi   ########
##########################################

CC=' g++ -g '
AR=' ar '
REM=' rm '
MKDIR= ' mkdir '

FRM_LIB_NAME='framework'
FRM_LIB='lib' + FRM_LIB_NAME + '.a'
DEP_LIB='pthread'

PAT="#"
PAT_SUB_LEN=4


INC_PATH='./include'
OBJ_PATH='./obj'
FRM_OBJ_PATH= OBJ_PATH + "/framework"
MOD_OBJ_PATH= OBJ_PATH + "/modules"
FRM_PATH='./framework'
MOD_PATH='./modules'
LIB_PATH='./lib'
BIN_PATH='./bin'
