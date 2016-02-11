#ifndef PYHELPER_H
#define PYHELPER_H

#include <stdexcept>
#include <memory>
#include <typeinfo>

#include <Python.h>

#include <pv/sharedPtr.h>

// Throw this to indicate that a python exception is active
struct python_exception : public std::runtime_error
{
    python_exception() : std::runtime_error("python") {}
    virtual ~python_exception() throw() {}
};

//! helper to indicate that the wrapped PyObject is a borrowed referenced for the caller
struct borrowed {
    PyObject *o;
    explicit borrowed(PyObject *o) :o(o) {}
};

//! Smart pointer for python objects
struct PyObj
{
    PyObject *obj;

    PyObj() :obj(NULL) {}

    //! Assumes caller already has a reference to this object
    //! if o==NULL then assume a python exception has been raised
    explicit PyObj(PyObject *o) :obj(o)
    {
        if(!obj)
            throw python_exception();
    }
    //! Assumes caller does not have a reference to this object
    //! if o==NULL then assume a python exception has been raised
    explicit PyObj(const borrowed& b) :obj(b.o)
    {
        if(!obj)
            throw python_exception();
        Py_INCREF(obj);
    }

    ~PyObj()
    {
        Py_XDECREF(obj);
    }

    //! Explicitly fetch the underlying PyObject*
    PyObject *get() {return obj;}
    //! Implicitly fetch the underlying PyObject*
    //operator PyObject*() { return obj; }

    //! Give up control of this reference (no decrement)
    PyObject *release() {
        PyObject *ret=obj;
        obj = NULL;
        return ret;
    }
    //! Switch object being pointed to.
    //! Any existing reference is decremented
    //! if o==NULL then simply clear the reference
    void reset(PyObject *o = 0)
    {
        Py_XDECREF(obj);
        obj = o;
    }

    //! if o==NULL then assume a python exception has been raised
    void reset(const borrowed& b)
    {
        if(!b.o)
            throw python_exception();
        Py_INCREF(b.o);
        Py_XDECREF(obj);
        obj = b.o;
    }

    PyObject& operator*() { return *obj; }
    PyObject* operator->() { return obj; }

private:
    PyObj(const PyObj&);
    PyObj& operator=(const PyObj&);
};

//! Scoped GIL locker
struct PyLockGIL
{
    PyGILState_STATE state;
    PyLockGIL() :state(PyGILState_Ensure()) {}
    ~PyLockGIL() {
        PyGILState_Release(state);
    }
private:
    PyLockGIL(const PyLockGIL&);
    PyLockGIL& operator=(const PyLockGIL&);
};

//! Scoped GIL unlocker
struct PyUnlockGIL
{
    PyThreadState *state;
    PyUnlockGIL() :state(PyEval_SaveThread()) {}
    ~PyUnlockGIL() { PyEval_RestoreThread(state); }
};

template<class T>
struct EnCapsule
{
    typedef T value_type;
    value_type val;

    EnCapsule(const value_type& v) :val(v) {}

    static
    PyObject* wrap(const T& v)
    {
        std::auto_ptr<EnCapsule> R(new EnCapsule(v));
        PyObj ret(PyCapsule_New(R.get(), typeid(EnCapsule).name(), &cleanup));
        R.release();
        return ret.release();
    }

    static
    value_type* unwrap(PyObject *cap)
    {
        void *raw = PyCapsule_GetPointer(cap, typeid(EnCapsule).name());
        if(!raw) {
            PyErr_Format(PyExc_RuntimeError, "wrong capsule type %s expect %s",
                         PyCapsule_GetName(cap), typeid(EnCapsule).name());
            return 0;
        }
        EnCapsule *ptr = static_cast<EnCapsule*>(raw);
        return &ptr->val;
    }

private:
    static
    void cleanup(PyObject *cap)
    {
        void *raw = PyCapsule_GetPointer(cap, typeid(EnCapsule).name());
        if(!raw) return;
        EnCapsule *ptr = static_cast<EnCapsule*>(raw);
        try{
            delete ptr;
        }catch(std::exception& e){
            PyErr_Format(PyExc_RuntimeError, "EnCapsule::cleanup(): %s", e.what());
        }
    }
};

#define EXECTOPY(klass, PYEXC) catch(klass& e) { PyErr_SetString(PyExc_##PYEXC, e.what()); }

#endif /* PYHELPER_H */
