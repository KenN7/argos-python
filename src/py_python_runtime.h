#ifndef PY_PYTHON_RUNTIME_H
#define PY_PYTHON_RUNTIME_H

#include <Python.h>

namespace argos {

void EnsurePythonInitialized();

class CPyGILGuard {
  public:
    CPyGILGuard();
    ~CPyGILGuard();

  private:
    CPyGILGuard(const CPyGILGuard&);
    CPyGILGuard& operator=(const CPyGILGuard&);

    PyGILState_STATE m_tState;
};

} // namespace argos

#endif
