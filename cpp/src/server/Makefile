TOP = ../../..
include $(TOP)/configure/CONFIG
include $(TOP)/configure/CONFIG_PY

USR_INCLUDES += -I${PYTHON_BASE}/include/${PYTHON}

INC += dsl.h

LIBRARY = masarServer

SERVER = $(TOP)/cpp/src/server/

SRC_DIRS += $(SERVER)/service
INC += masarService.h
LIBSRCS += masarService.cpp

SRC_DIRS += $(SERVER)/dslPY
INC += dslPY.h
LIBSRCS += dslPY.cpp

masarServer_LIBS += gather nt pvAccess pvData Com
#masarService_LIBS += $(PYTHON)


$(PYTHON)_DIR = $(PYTHON_BASE)/lib/$(PYTHON)/config

include $(TOP)/configure/RULES

