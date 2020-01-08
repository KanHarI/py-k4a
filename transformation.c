#include <Python.h>
#include <structmember.h>
#include <k4a/k4a.h>

#include "types.h"

#include <stdio.h>

PyObject* transformation_depth_image_to_color_camera(PyObject* self, PyObject* args)
{
	DeviceObject *obj;
	DeviceConfiguration *config;
	TransformationObject *transObj;
	ImageObject* imgObj;

	PyArg_ParseTuple(args, "OOOO", &obj, &config, &transObj, &imgObj);

	k4a_calibration_t calibration;

    k4a_result_t result = k4a_device_get_calibration(
        obj->device,
        config->depth_mode,
        config->color_resolution,
        &calibration
    );

    if (result != K4A_RESULT_SUCCEEDED)
    {
        return Py_None;
    }

    ImageObject* dstImgObj = newImageObject();
    result = k4a_image_create(
    	K4A_IMAGE_FORMAT_DEPTH16,
    	calibration.color_camera_calibration.resolution_width,
    	calibration.color_camera_calibration.resolution_height,
    	2*calibration.color_camera_calibration.resolution_width, /* 2 Bytes per pixel */
    	&dstImgObj->image
    );

    if (result != K4A_RESULT_SUCCEEDED)
    {
    	return Py_None;
    }

    result = k4a_transformation_depth_image_to_color_camera(
    	transObj->transformation,
    	dstImgObj->image,
    	imgObj->image
    );

    if (result != K4A_RESULT_SUCCEEDED)
    {
    	k4a_image_release(dstImgObj->image);
    	return Py_None;
    }

    return (PyObject*) dstImgObj;
}

PyObject* calibration_2d_to_3d(PyObject* self, PyObject* args)
{
	DeviceObject *obj;
	DeviceConfiguration *config;
	k4a_float2_t pixel_loc;
	float depth;

    printf("Line 68!");

	PyArg_ParseTuple("OOfff", &obj, &config, &pixel_loc.xy.x, &pixel_loc.xy.y, &depth);

    printf("Line 72!");

	k4a_calibration_t calibration;

    k4a_result_t result = k4a_device_get_calibration(
        obj->device,
        config->depth_mode,
        config->color_resolution,
        &calibration
    );

    printf("Line 83!");

    if (result != K4A_RESULT_SUCCEEDED)
    {
        return Py_None;
    }

    printf("Line 90!");

    k4a_float3_t _3d_loc;
    int is_valid;

    printf("Line 97!");

    result = k4a_calibration_2d_to_3d(
    	&calibration,
    	&pixel_loc,
    	depth,
    	K4A_CALIBRATION_TYPE_COLOR,
    	K4A_CALIBRATION_TYPE_COLOR,
    	&_3d_loc,
    	&is_valid
    );

    printf("Line 107!");

    if (result != K4A_RESULT_SUCCEEDED || !is_valid)
    {
    	return Py_None;
    }

    printf("Line 114!");

	return Py_BuildValue("fff", _3d_loc.xyz.x, _3d_loc.xyz.y, _3d_loc.xyz.z);
}
