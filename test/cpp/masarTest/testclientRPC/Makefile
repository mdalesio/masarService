TOP=../..

include $(TOP)/configure/CONFIG

PROD_HOST += testclientRPC
testclientRPC_SRCS += testclientRPC.cpp
testclientRPC_LIBS += gather nt pvAccess pvData Com
testclientRPC_SYS_LIBS += $(PYTHON)

PROD_HOST += testGetLiveMachine
testGetLiveMachine_SRCS += testGetLiveMachine.cpp
testGetLiveMachine_LIBS += gather nt pvAccess pvData Com
testGetLiveMachine_SYS_LIBS += $(PYTHON)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

