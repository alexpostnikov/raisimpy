/**
 * Python wrappers for raisim.object.terrain using pybind11.
 *
 * Copyright (c) 2019, kangd and jhwangbo (original C++), Brian Delhaisse <briandelhaisse@gmail.com> (Python wrappers)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>     // automatic conversion between std::vector, std::list, etc to Python list/tuples/dict
#include <pybind11/eigen.h>   // automatic conversion between Eigen data types to Numpy data types

#include "raisim/math.hpp"   // contains the definitions of Vec, Mat, etc.
#include "raisim/object/terrain/Ground.hpp"
#include "raisim/object/terrain/HeightMap.hpp"

#include "ode/collision.h"
#include "ode/ode.h"
#include "ode/extras/collision_kernel.h"
// Important note: for the above include ("ode/src/collision_kernel.h"), you have to add a `extras` folder in the
// `$LOCAL_BUILD/include/ode/` which should contain the following header files:
// array.h, collision_kernel.h, common.h, error.h, objects.h, odeou.h, odetls.h, threading_base.h, and typedefs.h.
// These header files can be found in the `ode/src` folder (like here: https://github.com/thomasmarsh/ODE/tree/master/ode/src)
//
// Why do we need to do that? The reason is that for `raisim::Mesh`, the authors of RaiSim use the `dSpaceID` variable
// type which has been forward declared in `ode/common.h` (but not defined there) as such:
//
// struct dxSpace;
// typedef struct dxSpace *dSpaceID;
//
// Thus for `dSpaceID` we need the definition of `dxSpace`, and this one is defined in `ode/src/collision_kernel.h` (in
// the `src` folder and not in the `include` folder!!). Pybind11 is looking for that definition, if you don't include
// it, pybind11 will complain and raise errors.


namespace py = pybind11;
using namespace raisim;


void init_terrain(py::module &m) {

    /***********/
    /* Terrain */
    /***********/
    py::class_<raisim::TerrainProperties>(m, "TerrainProperties", "Raisim terrain properties")
        .def(py::init<>(), "Initialize the terrain properties")
        .def(py::init<double, double, double, double, size_t, size_t, size_t, double, double, double, std::uint32_t>(),
        "Initialize the terrain properties.\n\n"
        "Args:\n"
        "    frequency (float): frequency.\n"
        "    x_size (float): the size in the x direction.\n"
        "    y_size (float): the size in the y direction.\n"
        "    z_scale (float): z scale.\n"
        "    x_samples (int): the number of samples in x.\n"
	    "    y_samples (int): the number of samples in y.\n"
	    "    fractal_octaves (int): the number of fractal octaves.\n"
        "    fractal_lacunarity (float): the lacunarity of fractals.\n"
        "    fractal_gain (float): fractal gain.\n"
        "    step_size (float): the step size.\n"
        "    seed (int): the random seed.",
        py::arg("frequency"), py::arg("x_size"), py::arg("y_size"), py::arg("z_scale"), py::arg("x_samples"),
        py::arg("y_samples"), py::arg("fractal_octaves"), py::arg("fractal_lacunarity"), py::arg("fractal_gain"),
        py::arg("step_size"), py::arg("seed"))

        .def_readwrite("frequency", &raisim::TerrainProperties::frequency)
        .def_readwrite("x_size", &raisim::TerrainProperties::xSize)
        .def_readwrite("y_size", &raisim::TerrainProperties::ySize)
        .def_readwrite("z_scale", &raisim::TerrainProperties::zScale)
        .def_readwrite("x_samples", &raisim::TerrainProperties::xSamples)
        .def_readwrite("y_samples", &raisim::TerrainProperties::ySamples)
        .def_readwrite("fractal_octaves", &raisim::TerrainProperties::fractalOctaves)
        .def_readwrite("fractal_lacunarity", &raisim::TerrainProperties::fractalLacunarity)
        .def_readwrite("fractal_gain", &raisim::TerrainProperties::fractalGain)
        .def_readwrite("step_size", &raisim::TerrainProperties::stepSize)
        .def_readwrite("seed", &raisim::TerrainProperties::seed);


    /**********/
	/* Ground */
	/**********/
	py::class_<raisim::Ground, raisim::SingleBodyObject>(m, "Ground", "Raisim Ground.")
	    .def(py::init<double>(),
	    "Initialize a ground instance.\n\n"
	    "Args:\n"
	    "    height (float): height of the ground.",
	    py::arg("height"))
	    .def("get_height", &raisim::Ground::getHeight, R"mydelimiter(
	    Get the ground's height.

	    Returns:
	        float: height of the ground.
	    )mydelimiter");


    /*************/
    /* HeightMap */
    /*************/
    py::class_<raisim::HeightMap, raisim::SingleBodyObject>(m, "HeightMap", "Raisim HeightMap.")
        .def(py::init<double, double, const std::string&>(),
	    "Initialize a HeightMap instance.\n\n"
	    "Args:\n"
	    "    x_center (float): the x center of the heightmap in the world.\n"
        "    y_center (float): the y center of the heightmap in the world.\n"
	    "    filename (str): raisim heightmap filename.",
	    py::arg("x_center"), py::arg("y_center"), py::arg("filename"))

	    .def(py::init<double, double, const std::string&, double, double, double, double>(),
	    "Initialize a HeightMap instance.\n\n"
	    "Args:\n"
	    "    x_center (float): the x center of the heightmap in the world.\n"
        "    y_center (float): the y center of the heightmap in the world.\n"
	    "    filename (str): filename to the PNG.\n"
        "    x_size (float): the size in the x direction.\n"
        "    y_size (float): the size in the y direction.\n"
	    "    height_scale (float): the height scale.\n"
	    "    height_offset (float): the height offset.",
	    py::arg("x_center"), py::arg("y_center"), py::arg("filename"), py::arg("x_size"), py::arg("y_size"),
	    py::arg("height_scale"), py::arg("height_offset"))

        .def(py::init<int, int, double, double, double, double, const std::vector<double> &>(),
        "Initialize a HeightMap instance.\n\n"
	    "Args:\n"
	    "    x_samples (int): the number of samples in x.\n"
	    "    y_samples (int): the number of samples in y.\n"
        "    x_scale (float): the scale in the x direction.\n"
        "    y_scale (float): the scale in the y direction.\n"
        "    x_center (float): the x center of the heightmap in the world.\n"
        "    y_center (float): the y center of the heightmap in the world.\n"
        "    heights (list[float]): list of desired heights.",
	    py::arg("x_samples"), py::arg("y_samples"), py::arg("x_scale"), py::arg("y_scale"), py::arg("x_center"),
	    py::arg("y_center"), py::arg("heights"))

	    .def(py::init<double, double, const raisim::TerrainProperties &>(),
	    "Initialize a HeightMap instance.\n\n"
	    "Args:\n"
        "    x_center (float): the x center of the heightmap in the world.\n"
        "    y_center (float): the y center of the heightmap in the world.\n"
        "    terrain_properties (TerrainProperties): the terrain properties.\n",
	    py::arg("x_center"), py::arg("y_center"), py::arg("terrain_properties"))


        .def("init", &raisim::HeightMap::init, R"mydelimiter(
	    Initialize the heightmap.

	    Args:
	        x_samples (int): the number of samples in x.
	        y_samples (int): the number of samples in y.
            x_scale (float): the scale in the x direction.
            y_scale (float): the scale in the y direction.
            x_center (float): the x center of the heightmap in the world.
            y_center (float): the y center of the heightmap in the world.
	    )mydelimiter",
	    py::arg("x_samples"), py::arg("y_samples"), py::arg("x_scale"), py::arg("y_scale"), py::arg("x_center"),
	    py::arg("y_center"))


        .def("get_height_map", &raisim::HeightMap::getHeightMap, R"mydelimiter(
	    Get the height map.

	    Returns:
	        list[float]: height map.
	    )mydelimiter")


	    .def("get_height", &raisim::HeightMap::getHeight, R"mydelimiter(
	    Get the height at the given location.

	    Args:
            x (float): x position.
            y (float): y position.
	    )mydelimiter",
	    py::arg("x"), py::arg("y"))


        .def("destroy_collision_bodies", &raisim::HeightMap::destroyCollisionBodies, R"mydelimiter(
	    Destroy the collision bodies of the specified collision space.

	    Args:
            id_ (dSpaceID): collision space id.
	    )mydelimiter",
	    py::arg("id_"))


	    .def("get_x_samples", &raisim::HeightMap::getXSamples, R"mydelimiter(
	    Get the x samples.

	    Returns:
            int: the number of x samples.
	    )mydelimiter")


	    .def("get_y_samples", &raisim::HeightMap::getYSamples, R"mydelimiter(
	    Get the y samples.

	    Returns:
            int: the number of y samples.
	    )mydelimiter")


	    .def("get_x_size", &raisim::HeightMap::getXSize, R"mydelimiter(
	    Get the x size.

	    Returns:
            float: the size in x.
	    )mydelimiter")


	    .def("get_y_size", &raisim::HeightMap::getYSize, R"mydelimiter(
	    Get the y size.

	    Returns:
            float: the size in y.
	    )mydelimiter")


	    .def("get_x_center", &raisim::HeightMap::getCenterX, R"mydelimiter(
	    Get the x center.

	    Returns:
            float: the x center.
	    )mydelimiter")


	    .def("get_y_center", &raisim::HeightMap::getCenterY, R"mydelimiter(
	    Get the y center.

	    Returns:
            float: the y center.
	    )mydelimiter")


	    .def("get_height_vector", &raisim::HeightMap::getHeightVector, R"mydelimiter(
	    Get the height vector.

	    Returns:
            list[float]: the height vector.
	    )mydelimiter");

}