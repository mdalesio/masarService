TOP=../..

include $(TOP)/configure/CONFIG


PROD_HOST += testDSLGetLiveMachine
testDSLGetLiveMachine_SRCS += testDSLGetLiveMachine.cpp
testDSLGetLiveMachine_LIBS += masarCore nt pvAccess pvData Com
testDSLGetLiveMachine_SYS_LIBS += $(PYTHON)

PROD_HOST += testDSLRetrieveSnapshot
testDSLRetrieveSnapshot_SRCS += testDSLRetrieveSnapshot.cpp
testDSLRetrieveSnapshot_LIBS += masarCore nt pvAccess pvData Com
testDSLRetrieveSnapshot_SYS_LIBS += $(PYTHON)

PROD_HOST += testDSLRetrieveServiceConfigs
testDSLRetrieveServiceConfigs_SRCS += testDSLRetrieveServiceConfigs.cpp
testDSLRetrieveServiceConfigs_LIBS += masarCore nt pvAccess pvData Com
testDSLRetrieveServiceConfigs_SYS_LIBS += $(PYTHON)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

