#Makefile at top of application tree
TOP = ..
include $(TOP)/configure/CONFIG

USR_INCLUDES += -I${PYTHON_BASE}/include/${PYTHON}

SRC = $(TOP)/src

PROD_HOST += masarServiceRun
masarServiceRun_LIBS += masarCore nt pvAccess pvData Com
masarServiceRun_SYS_LIBS += $(PYTHON)

LIBRARY += masarCore
masarCore_LIBS += nt pvAccess pvData Com

SRC_DIRS += $(SRC)/masarServiceRun

masarCore_SRCS += masarServiceRun.cpp

SRC_DIRS += $(SRC)/server

INC += dsl.h

SRC_DIRS += $(SRC)/server/service

INC += masarService.h
masarCore_SRCS += masarService.cpp

SRC_DIRS += $(SRC)/server/dslPY

SRC_DIRS += $(SERVER)/dslPY
INC += dslPY.h
masarCore_SRCS += dslPY.cpp

SRC_DIRS += $(SRC)/util

INC += pyhelper.h

SRC_DIRS += $(SRC)/client/gatherV3Data

INC += gatherV3Data.h

masarCore_SRCS += gatherV3Data.cpp

SRC_DIRS += $(SRC)/python

masarCore_SRCS += alarmPy.cpp
masarCore_SRCS += channelRPCPy.cpp
masarCore_SRCS += controlPy.cpp
masarCore_SRCS += displayPy.cpp
masarCore_SRCS += gatherV3DataPy.cpp
masarCore_SRCS += ntmultiChannelPy.cpp
masarCore_SRCS += ntnameValuePy.cpp
masarCore_SRCS += ntscalarPy.cpp
masarCore_SRCS += nttablePy.cpp
masarCore_SRCS += timeStampPy.cpp

# Needed on RHEL/CentOS
USR_SYS_LIBS += util

include $(TOP)/configure/RULES


