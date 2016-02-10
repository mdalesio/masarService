/* gatherV3DataPy.cpp */
/*
 *Copyright - See the COPYRIGHT that is included with this distribution.
 *EPICS pvServiceCPP is distributed subject to a Software License Agreement found
 *in file LICENSE that is included with this distribution.
 */
/* Author:  Marty Kraimer Date: 2011.12 */

#undef _POSIX_C_SOURCE
#undef _XOPEN_SOURCE
#include <Python.h>

#include <pv/gatherV3Data.h>
#include <pv/pyhelper.h>
#include <stdexcept>

namespace epics { namespace masar {

using namespace epics::pvData;
using namespace epics::nt;
using namespace std;
typedef std::tr1::shared_ptr<NTMultiChannel> NTMultiChannelPtr;

class GatherV3DataPyPvt {
public:
    GatherV3DataPyPvt(
        shared_vector<const std::string> const & channelNames);
    ~GatherV3DataPyPvt();
public:
    GatherV3DataPtr gatherV3Data;
    PVStructurePtr pvStructure;
};

GatherV3DataPyPvt::GatherV3DataPyPvt(
    shared_vector<const std::string> const & channelNames)
: gatherV3Data(GatherV3Data::create(channelNames))
{
}


GatherV3DataPyPvt::~GatherV3DataPyPvt()
{
}

static PyObject * _init(PyObject *willBeNull, PyObject *args)
{
try{
    PyObject *pytuple = 0;
    if(!PyArg_ParseTuple(args,"O!:gatherV3DataPy",
        &PyTuple_Type, &pytuple))
    {
        return NULL;
    }
    Py_ssize_t num = PyTuple_GET_SIZE(pytuple);
    shared_vector<string> names(num);
    for(Py_ssize_t i=0; i<num; i++) {
        PyObject *pyobject = PyTuple_GetItem(pytuple,i);
        if(pyobject==NULL) {
            PyErr_SetString(PyExc_SyntaxError,
               "a channelName is null");
            return NULL;
        }
        char *sval =  PyString_AsString(pyobject);
        if(sval==NULL) {
            return NULL;
        }
        names[i] = sval;
    }
    shared_vector<const string> name(freeze(names));
    GatherV3DataPyPvt *pvt = new GatherV3DataPyPvt(name);
    return PyCapsule_New(pvt,"gatherV3DataPvt",0);
}EXECTOPY(std::exception, RuntimeError)
    return NULL;
}


static PyObject * _destroy(PyObject *willBeNull, PyObject *args)
{
try{
    PyObject *pcapsule = 0;
    if(!PyArg_ParseTuple(args,"O!:gatherV3DataPy",
        &PyCapsule_Type, &pcapsule))
    {
        return NULL;
    }
    void *pvoid = PyCapsule_GetPointer(pcapsule,"gatherV3DataPvt");
    if(pvoid==0) {
        PyErr_SetString(PyExc_SyntaxError,
           "first arg must be return from _init");
        return NULL;
    }
    std::auto_ptr<GatherV3DataPyPvt> pvt(static_cast<GatherV3DataPyPvt *>(pvoid));
    GatherV3DataPtr const & gatherV3Data = pvt->gatherV3Data;
    {
        PyUnlockGIL U;
        gatherV3Data->destroy();
    }
    Py_RETURN_NONE;
}EXECTOPY(std::exception, RuntimeError)
    return NULL;
}

static PyObject * _connect(PyObject *willBeNull, PyObject *args)
{
try {
    PyObject *pcapsule = 0;
    double timeout = 1.0;
    if(!PyArg_ParseTuple(args,"O!d:gatherV3DataPy",
        &PyCapsule_Type, &pcapsule,
        &timeout))
    {
        return NULL;
    }
    void *pvoid = PyCapsule_GetPointer(pcapsule,"gatherV3DataPvt");
    if(pvoid==0) {
        PyErr_SetString(PyExc_SyntaxError,
           "first arg must be return from _init");
        return NULL;
    }
    GatherV3DataPyPvt *pvt = static_cast<GatherV3DataPyPvt *>(pvoid);
    GatherV3DataPtr const & gatherV3Data = pvt->gatherV3Data;
    bool result = false;
    {
        PyUnlockGIL U;
        result = gatherV3Data->connect(timeout);
    }
    if(result) {
         Py_RETURN_TRUE;
    }
    Py_RETURN_FALSE;
}EXECTOPY(std::exception, RuntimeError)
    return NULL;
}

static PyObject * _get(PyObject *willBeNull, PyObject *args)
{
try {
    PyObject *pcapsule = 0;
    if(!PyArg_ParseTuple(args,"O!:gatherV3DataPy",
        &PyCapsule_Type, &pcapsule))
    {
        return NULL;
    }
    void *pvoid = PyCapsule_GetPointer(pcapsule,"gatherV3DataPvt");
    if(pvoid==0) {
        PyErr_SetString(PyExc_SyntaxError,
           "arg must be return from _init");
        return NULL;
    }
    GatherV3DataPyPvt *pvt = static_cast<GatherV3DataPyPvt *>(pvoid);
    GatherV3DataPtr const & gatherV3Data = pvt->gatherV3Data;
    bool result = true;
    {
        PyUnlockGIL U;
        result = gatherV3Data->get();
    }
    if(result) {
        Py_RETURN_TRUE;
    }
    Py_RETURN_FALSE;
}EXECTOPY(std::exception, RuntimeError)
    return NULL;
}

static PyObject * _put(PyObject *willBeNull, PyObject *args)
{
try {
    PyObject *pcapsule = 0;
    if(!PyArg_ParseTuple(args,"O!:gatherV3DataPy",
        &PyCapsule_Type, &pcapsule))
    {
        return NULL;
    }
    void *pvoid = PyCapsule_GetPointer(pcapsule,"gatherV3DataPvt");
    if(pvoid==0) {
        PyErr_SetString(PyExc_SyntaxError,
           "arg must be return from _init");
        return NULL;
    }
    GatherV3DataPyPvt *pvt = static_cast<GatherV3DataPyPvt *>(pvoid);
    GatherV3DataPtr const & gatherV3Data = pvt->gatherV3Data;
    bool result = true;
    {
        PyUnlockGIL U;
        result = gatherV3Data->put();
    }
    if(result) {
        Py_RETURN_TRUE;
    }
    Py_RETURN_FALSE;
}EXECTOPY(std::exception, RuntimeError)
    return NULL;
}

static PyObject * _getMessage(PyObject *willBeNull, PyObject *args)
{
try {
    PyObject *pcapsule = 0;
    if(!PyArg_ParseTuple(args,"O!:gatherV3DataPy",
        &PyCapsule_Type, &pcapsule))
    {
        PyErr_SetString(PyExc_SyntaxError,
           "Bad argument. Expected (pvt)");
        return NULL;
    }
    void *pvoid = PyCapsule_GetPointer(pcapsule,"gatherV3DataPvt");
    if(pvoid==0) {
        PyErr_SetString(PyExc_SyntaxError,
           "first arg must be return from _init");
        return NULL;
    }
    GatherV3DataPyPvt *pvt = static_cast<GatherV3DataPyPvt *>(pvoid);
    string message = pvt->gatherV3Data->getMessage();
    return Py_BuildValue("s",message.c_str());
}EXECTOPY(std::exception, RuntimeError)
    return NULL;
}

static PyObject * _getPVStructure(PyObject *willBeNull, PyObject *args)
{
try {
    PyObject *pcapsule = 0;
    if(!PyArg_ParseTuple(args,"O!:gatherV3DataPy",
        &PyCapsule_Type, &pcapsule))
    {
        return NULL;
    }
    void *pvoid = PyCapsule_GetPointer(pcapsule,"gatherV3DataPvt");
    if(pvoid==0) {
        PyErr_SetString(PyExc_SyntaxError,
           "first arg must be return from _init");
        return NULL;
    }
    GatherV3DataPyPvt *pvt = static_cast<GatherV3DataPyPvt *>(pvoid);
    pvt->pvStructure = pvt->gatherV3Data->getNTMultiChannel()->getPVStructure();
    return PyCapsule_New(&pvt->pvStructure,"pvStructure",0);
}EXECTOPY(std::exception, RuntimeError)
    return NULL;
}

static char _initDoc[] = "_init gatherV3DataPy.";
static char _destroyDoc[] = "_destroy gatherV3DataPy.";
static char _connectDoc[] = "_connect.";
static char _getDoc[] = "_get.";
static char _putDoc[] = "_put.";
static char _getMessageDoc[] = "_getMessage.";
static char _getPVStructureDoc[] = "_getPVStructure.";

static PyMethodDef methods[] = {
    {"_init",_init,METH_VARARGS,_initDoc},
    {"_destroy",_destroy,METH_VARARGS,_destroyDoc},
    {"_connect",_connect,METH_VARARGS,_connectDoc},
    {"_get",_get,METH_VARARGS,_getDoc},
    {"_put",_put,METH_VARARGS,_putDoc},
    {"_getMessage",_getMessage,METH_VARARGS,_getMessageDoc},
    {"_getPVStructure",_getPVStructure,METH_VARARGS,_getPVStructureDoc},
    {NULL,NULL,0,NULL}
};

PyMODINIT_FUNC initgatherV3DataPy(void)
{
    PyObject * m = Py_InitModule("gatherV3DataPy",methods);
    if(m==NULL) printf("initgatherV3DataPy failed\n");
}

}}
