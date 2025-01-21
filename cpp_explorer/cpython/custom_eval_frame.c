/*******************************************************************************
 * An toy example to use PEP 523 to define custom frame evaluation procedure
 * NOTE: 1. python version: 3.12
 *       2. pytorch commit: 1664033e
 *       3. check https://docs.python.org/3.12/index.html for latest CPython API
 * Compile cmd: gcc -shared -o custom_eval_frame.so custom_eval_frame.c \
 *              $(python3-config --includes --ldflags)
 * Author: Chen Chen
 ******************************************************************************/
#include <stdio.h>

#include <Python.h>
#include <frameobject.h>
#include <internal/pycore_frame.h>

// 自定义帧求值函数
static PyObject* my_eval_frame(PyThreadState* tstate,
                               _PyInterpreterFrame* frame, int throw_flag) {
    if (!frame) {
        fprintf(stderr, "Error: frame is NULL\n");
        return NULL;
    }
    fprintf(stdout, "Hello\n");

    PyCodeObject* code = PyUnstable_InterpreterFrame_GetCode(frame);
    if (!code) {
        fprintf(stderr, "Error: failed to get code object from frame\n");
        return NULL;
    }

    fprintf(stdout, "Get code object\n");
    // 打印当前帧的代码对象名称
    const char* code_name = PyUnicode_AsUTF8(code->co_name);
    if (!code_name) {
        fprintf(stderr, "Error: failed to get code name\n");
        Py_DECREF(code);
        return NULL;
    }
    printf("Evaluating frame for code: %s\n", code_name);

    Py_DECREF(code);

    // 调用默认的帧求值函数
    return _PyEval_EvalFrameDefault(tstate, frame, throw_flag);
}

// 注册自定义帧求值函数
static int enable_custom_eval() {
    // 获取当前线程状态
    PyThreadState* tstate = PyThreadState_Get();
    if (!tstate) {
        fprintf(stderr, "Error: failed to get thread state\n");
        return -1;
    }

    // 设置自定义帧求值函数
    _PyInterpreterState_SetEvalFrameFunc(tstate->interp, my_eval_frame);

    return 0;
}

// Python 模块初始化函数
static PyObject* py_enable_custom_eval(PyObject* self, PyObject* args) {
    if (enable_custom_eval() == 0) {
        Py_RETURN_TRUE;
    }
    Py_RETURN_FALSE;
}

// 模块方法表
static PyMethodDef MyMethods[] = {{"enable_custom_eval", py_enable_custom_eval,
                                   METH_VARARGS,
                                   "Enable custom frame evaluation"},
                                  {NULL, NULL, 0, NULL}};

// 模块定义
static struct PyModuleDef custom_eval_frame = {
    PyModuleDef_HEAD_INIT, "custom_eval_frame", NULL, -1, MyMethods};

// 模块初始化函数
PyMODINIT_FUNC PyInit_custom_eval_frame(void) {
    return PyModule_Create(&custom_eval_frame);
}
