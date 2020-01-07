#include <k4a/k4a.h>
#include <structmember.h>
#include <Python.h>

#include "types.h"

#ifndef __PY_K4A__TRANSFORMATION__
#define __PY_K4A__TRANSFORMATION__

PyObject* transformation_depth_image_to_color_camera(PyObject* self, PyObject* args);
PyObject* calibration_2d_to_3d(PyObject* self, PyObject* args);

#endif
