#include <Python.h>

#include "shmctl.h"
#include "node.h"

PyMODINIT_FUNC PyInit_node ();
static struct PyModuleDef traf_node_module;
static PyMethodDef traf_node_methods[];
static PyObject *_send_d(PyObject *, PyObject *);
static PyObject *_recv_d(PyObject *, PyObject *);
static PyObject *_start_server(PyObject *);
static PyObject *_stop_server(PyObject *);

PyMODINIT_FUNC PyInit_traf_node() 
{
    return PyModule_Create (&traf_node_module);
}

static struct PyModuleDef traf_node_module = {
    PyModuleDef_HEAD_INIT, 
    "traf_node",
    "c library for claculation",
    -1,
    traf_node_methods
};

static PyMethodDef traf_node_methods[] = {
    {"send_d", (PyCFunction) _send_d, METH_VARARGS, "Send a data to specific adjacent node by node index"},
    {"recv_d", (PyCFunction) _recv_d, METH_VARARGS, "Recv a data from specific adjacent node by node index"},
    {"start_server", (PyCFunction) _start_server, METH_NOARGS, "Start background server"},
    {"stop_server", (PyCFunction) _stop_server, METH_NOARGS, "Stop background server"},
    {NULL, NULL, 0, NULL} 
};

static PyObject *_send_d(PyObject *self, PyObject *args)
{
    int n;
    int ret;
    int len;
    struct contant d;

    if (!PyArg_ParseTuple (args, "is#", &n, &(d.json), len))
        return Py_BuildValue ("i", -1);

    if (len > 200) {
        fprintf (stderr, "Input length too long");
        eixt (-1);
    }

    ret = send_d (n, d);

    return Py_BuildValue ("i", ret);
}

static PyObject *_recv_d(PyObject *self, PyObject *args)
{
    int n;
    struct contant d;
    if (!PyArg_ParseTuple (args, "i", &n))
        return Py_BuildValue ("i", -1);

    d = recv_d (n);

    return Py_BuildValue ("s", d.json);
}

static PyObject *_start_server(PyObject *self)
{
    int ret;

    ret = start_server ();

    return Py_BuildValue ("i", ret);
}

static PyObject *_stop_server(PyObject *self)
{
    int ret;

    ret = stop_server ();

    return Py_BuildValue ("i", ret);
}
