#include "py_python_runtime.h"

namespace argos {

void EnsurePythonInitialized() {
    if (!Py_IsInitialized()) {
        Py_Initialize();
        PyEval_SaveThread();
    }
}

CPyGILGuard::CPyGILGuard() {
    EnsurePythonInitialized();
    m_tState = PyGILState_Ensure();
}

CPyGILGuard::~CPyGILGuard() {
    PyGILState_Release(m_tState);
}

} // namespace argos
