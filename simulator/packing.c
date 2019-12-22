#include <Python.h>

#include <stdio.h>

#include "sim.h"

PyMODINIT_FUNC PyInit_simu();
static struct PyModuleDef simu_module;
static PyMethodDef simu_methods[];

static PyObject *_sim_start(PyObject *self);
static PyObject *_sim_stop(PyObject *self);
static PyObject *_set_fs_func(PyObject *self, PyObject *args);
static PyObject *_sim_update(PyObject *self);
static PyObject *_chg_sig(PyObject *self, PyObject *args);
static PyObject *_get_sig(PyObject *self, PyObject *args);
static PyObject *_get_cont(PyObject *self, PyObject *args);
static PyObject *_show_sm(PyObject *self);
/*
int sim_start(void);
int stop_sim(void);
int sim_update(void);
int chg_sig(int ,int);
int get_sig(int);
int get_cont(int, int);
*/

PyMODINIT_FUNC PyInit_simu() 
{
    return PyModule_Create (&simu_module);
}

static struct PyModuleDef simu_module = {
    PyModuleDef_HEAD_INIT, 
    "simu",
    "traffic simultor",
    -1,
    simu_methods
};

static PyMethodDef simu_methods[] = {
    {"sim_start", (PyCFunction) _sim_start, METH_NOARGS, "start simulater"},
    {"sim_stop", (PyCFunction) _sim_stop, METH_NOARGS, "stop simulater"},
    {"set_fs_func", (PyCFunction) _set_fs_func, METH_VARARGS, "set node phase update function"},
    {"sim_update", (PyCFunction) _sim_update, METH_NOARGS, "simulater update"},
    {"chg_sig", (PyCFunction) _chg_sig, METH_VARARGS, "change signal"},
    {"get_sig", (PyCFunction) _get_sig, METH_VARARGS, "get signal"},
    {"get_cont", (PyCFunction) _get_cont, METH_VARARGS, "get contant on edge"},
    {"show_sm", (PyCFunction) _show_sm, METH_NOARGS, "show simulator"},
    {NULL, NULL, 0, NULL} 
};

static PyObject *_sim_start(PyObject *self)
{
    int ret;
    
    ret = sim_start ();

    return Py_BuildValue ("i", ret);
}

static PyObject *_sim_stop(PyObject *self)
{
    int ret;
    
    ret = sim_stop ();

    return Py_BuildValue ("i", ret);
}

static PyObject *_set_fs_func(PyObject *self, PyObject *args)
{
    int ret;
    int n;
    int s;
    int f;

    if (!PyArg_ParseTuple (args, "iii", &n, &s, &f))
        return Py_BuildValue ("i", -1);
    
    ret = set_fs_func (n, s, f);

    return Py_BuildValue ("i", ret);
}

static PyObject *_sim_update(PyObject *self)
{
    int ret;
    
    ret = sim_update ();

    return Py_BuildValue ("i", ret);
}

static PyObject *_chg_sig(PyObject *self, PyObject *args)
{
    int ret;
    int n;
    int s;

    if (!PyArg_ParseTuple (args, "ii", &n, &s))
        return Py_BuildValue ("i", -1);
    
    ret = chg_sig (n, s);

    return Py_BuildValue ("i", ret);
}

static PyObject *_get_sig(PyObject *self, PyObject *args)
{
    int ret;
    int n;

    if (!PyArg_ParseTuple (args, "i", &n))
        return Py_BuildValue ("i", -1);

    ret = get_sig (n);

    return Py_BuildValue ("i", ret);
}

static PyObject *_get_cont(PyObject *self, PyObject *args)
{
    int ret;
    int n;
    int e;

    if (!PyArg_ParseTuple (args, "ii", &n, &e))
        return Py_BuildValue ("i", -1);

    ret = get_cont (n, e);

    return Py_BuildValue ("i", ret);
}

static PyObject *_show_sm(PyObject *self)
{
    sim_show_sm ();

    return Py_BuildValue ("i", 0);
}
