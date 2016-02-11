/*alarmPy.cpp */
/*
 *Copyright - See the COPYRIGHT that is included with this distribution.
 *EPICS pvServiceCPP is distributed subject to a Software License Agreement found
 *in file LICENSE that is included with this distribution.
 */
/* Author:  Marty Kraimer Date: 2011.12 */

#undef _POSIX_C_SOURCE
#undef _XOPEN_SOURCE
#include <Python.h>


#include <pv/pvData.h>
#include <pv/alarm.h>
#include <pv/pvAlarm.h>
#include <stdexcept>

#include "pyhelper.h"

using namespace epics::pvData;
using namespace std;

namespace {
typedef EnCapsule<Alarm> alarm_t;

static PyObject * _init(PyObject *willbenull, PyObject *args)
{
    std::cerr<<__FILE__<<":"<<__LINE__<<"\n";
    return alarm_t::wrap(Alarm());
}

static PyObject * _destroy(PyObject *willBeNull, PyObject *args)
{
    Py_RETURN_NONE;
}


static PyObject * _str(PyObject *willBeNull, PyObject *args)
{
    PyObject *pcapsule = 0;
    if(!PyArg_ParseTuple(args,"O!:alarmPy",
        &PyCapsule_Type, &pcapsule))
    {
        return NULL;
    }
    Alarm* alarm = alarm_t::unwrap(pcapsule);
    if(!alarm) return NULL;

    std::ostringstream strm;
    strm<<"message "<<alarm->getMessage()
        <<" severity "<<(*AlarmSeverityFunc::getSeverityNames())[alarm->getSeverity()]
        <<" status "<<(*AlarmStatusFunc::getStatusNames())[alarm->getStatus()];

    return Py_BuildValue("s",strm.str().c_str());
}

static PyObject * _getAlarmPy(PyObject *willBeNull, PyObject *args)
{
    PyObject *pcapsule = 0;
    if(!PyArg_ParseTuple(args,"O!:alarmPy",
        &PyCapsule_Type, &pcapsule))
    {
        return NULL;
    }
    Alarm* alarm = alarm_t::unwrap(pcapsule);
    if(!alarm) return NULL;

    Py_INCREF(pcapsule);
    return pcapsule;
}

static PyObject * _getMessage(PyObject *willBeNull, PyObject *args)
{
    PyObject *pcapsule = 0;
    if(!PyArg_ParseTuple(args,"O!:alarmPy",
        &PyCapsule_Type, &pcapsule))
    {
        return NULL;
    }
    Alarm* alarm = alarm_t::unwrap(pcapsule);
    if(!alarm) return NULL;

    return Py_BuildValue("s",alarm->getMessage().c_str());
}

static PyObject * _setMessage(PyObject *willBeNull, PyObject *args)
{
    PyObject *pcapsule = 0;
    char *buffer;
    if(!PyArg_ParseTuple(args,"O!s:alarmPy",
        &PyCapsule_Type, &pcapsule,
        &buffer))
    {
        return NULL;
    }
    Alarm* alarm = alarm_t::unwrap(pcapsule);
    if(!alarm) return NULL;

    alarm->setMessage(buffer);
    Py_RETURN_NONE;
}

static PyObject * _getSeverity(PyObject *willBeNull, PyObject *args)
{
    PyObject *pcapsule = 0;
    if(!PyArg_ParseTuple(args,"O!:alarmPy",
        &PyCapsule_Type, &pcapsule))
    {
        return NULL;
    }
    Alarm* alarm = alarm_t::unwrap(pcapsule);
    if(!alarm) return NULL;

    string severity = (*AlarmSeverityFunc::getSeverityNames())[alarm->getSeverity()];
    return Py_BuildValue("s",severity.c_str());
}

static PyObject * _setSeverity(PyObject *willBeNull, PyObject *args)
{
    PyObject *pcapsule = 0;
    char *buffer;
    if(!PyArg_ParseTuple(args,"O!s:alarmPy",
        &PyCapsule_Type, &pcapsule,
        &buffer))
    {
        return NULL;
    }
    Alarm* alarm = alarm_t::unwrap(pcapsule);
    if(!alarm) return NULL;

    StringArrayPtr choiceArrayPtr = AlarmSeverityFunc::getSeverityNames();
    size_t nchoices = choiceArrayPtr.get()->size();
    for(size_t i=0;i<nchoices; i++) {
        string choice = (*choiceArrayPtr)[i];
        if(choice.compare(buffer)==0) {
            alarm->setSeverity(AlarmSeverityFunc::getSeverity(i));
            Py_RETURN_NONE;
        }
    }
    PyErr_SetString(PyExc_ValueError,"unknown severity");
    return NULL;
}

static PyObject * _getStatus(PyObject *willBeNull, PyObject *args)
{
    PyObject *pcapsule = 0;
    if(!PyArg_ParseTuple(args,"O!:alarmPy",
        &PyCapsule_Type, &pcapsule))
    {
        return NULL;
    }
    Alarm* alarm = alarm_t::unwrap(pcapsule);
    if(!alarm) return NULL;

    string severity = (*AlarmStatusFunc::getStatusNames())[alarm->getStatus()];
    return Py_BuildValue("s",severity.c_str());
}

static PyObject * _setStatus(PyObject *willBeNull, PyObject *args)
{
    PyObject *pcapsule = 0;
    char *buffer;
    if(!PyArg_ParseTuple(args,"O!s:alarmPy",
        &PyCapsule_Type, &pcapsule,
        &buffer))
    {
        PyErr_SetString(PyExc_SyntaxError,
           "Bad argument. Expected (pvt,string)");
        return NULL;
    }
    Alarm* alarm = alarm_t::unwrap(pcapsule);
    if(!alarm) return NULL;

    StringArrayPtr choiceArrayPtr = AlarmStatusFunc::getStatusNames();
    size_t nchoices = choiceArrayPtr.get()->size();
    for(size_t i=0;i<nchoices; i++) {
        string choice = (*choiceArrayPtr)[i];
        if(choice.compare(buffer)==0) {
            alarm->setStatus(AlarmStatusFunc::getStatus(i));
            Py_RETURN_NONE;
        }
    }
    PyErr_SetString(PyExc_ValueError,"unknown status");
    return NULL;
}

static PyObject * _getSeverityChoices(PyObject *willBeNull, PyObject *args)
{
    PyObject *pcapsule = 0;
    if(!PyArg_ParseTuple(args,"O:alarmPy",
        &pcapsule))
    {
        return NULL;
    }

    StringArrayPtr choices = AlarmSeverityFunc::getSeverityNames();
    if(choices.get()->size()!=5) {
        throw std::logic_error("number severity choices not 5");
    }
    return Py_BuildValue(
        "(s,s,s,s,s)",
        (*choices)[0].c_str(),
        (*choices)[1].c_str(),
        (*choices)[2].c_str(),
        (*choices)[3].c_str(),
        (*choices)[4].c_str());
}

static PyObject * _getStatusChoices(PyObject *willBeNull, PyObject *args)
{
    PyObject *pcapsule = 0;
    if(!PyArg_ParseTuple(args,"O:alarmPy",
        &pcapsule))
    {
        return NULL;
    }

    StringArrayPtr choices = AlarmStatusFunc::getStatusNames();
    if(choices.get()->size()!=8) {
        throw std::logic_error("number status choices not 8");
    }
    return Py_BuildValue(
        "(s,s,s,s,s,s,s,s)",
        (*choices)[0].c_str(),
        (*choices)[1].c_str(),
        (*choices)[2].c_str(),
        (*choices)[3].c_str(),
        (*choices)[4].c_str(),
        (*choices)[5].c_str(),
        (*choices)[6].c_str(),
        (*choices)[7].c_str());
}

static char _initDoc[] = "_init alarmPy.";
static char _destroyDoc[] = "_destroy alarmPy.";
static char _strDoc[] = "_str alarmPy.";
static char _getAlarmPyDoc[] = "_getAlarmPy.";
static char _getMessageDoc[] = "_getMessage alarmPy.";
static char _setMessageDoc[] = "_setMessage alarmPy.";
static char _getSeverityDoc[] = "_getSeverity alarmPy.";
static char _setSeverityDoc[] = "_setSeverity alarmPy.";
static char _getStatusDoc[] = "_getStatus alarmPy.";
static char _setStatusDoc[] = "_setStatus alarmPy.";
static char _getSeverityChoicesDoc[] = "_getSeverityChoices alarmPy.";
static char _getStatusChoicesDoc[] = "_getStatusChoices alarmPy.";

static PyMethodDef methods[] = {
    {"_init",_init,METH_VARARGS,_initDoc},
    {"_destroy",_destroy,METH_VARARGS,_destroyDoc},
    {"_str",_str,METH_VARARGS,_strDoc},
    {"_getAlarmPy",_getAlarmPy,METH_VARARGS,_getAlarmPyDoc},
    {"_getMessage",_getMessage,METH_VARARGS,_getMessageDoc},
    {"_setMessage",_setMessage,METH_VARARGS,_setMessageDoc},
    {"_getSeverity",_getSeverity,METH_VARARGS,_getSeverityDoc},
    {"_setSeverity",_setSeverity,METH_VARARGS,_setSeverityDoc},
    {"_getStatus",_getStatus,METH_VARARGS,_getStatusDoc},
    {"_setStatus",_setStatus,METH_VARARGS,_setStatusDoc},
    {"_getSeverityChoices",_getSeverityChoices,METH_VARARGS,_getSeverityChoicesDoc},
    {"_getStatusChoices",_getStatusChoices,METH_VARARGS,_getStatusChoicesDoc},
    {NULL,NULL,0,NULL}
};

} // namespace

PyMODINIT_FUNC initalarmPy(void)
{
    PyObject * m = Py_InitModule("alarmPy",methods);
    if(m==NULL) printf("initalarmPy failed\n");
}

