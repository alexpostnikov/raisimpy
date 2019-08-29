/**
 * Python wrappers for raisim.World using pybind11.
 *
 * Copyright (c) 2019, kangd (original C++), Brian Delhaisse <briandelhaisse@gmail.com> (Python wrappers)
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

#include <iostream>

#include "raisim/math.hpp"   // contains the definitions of Vec, Mat, etc.
#include "raisim/World.hpp"
#include "raisim/RaisimServer.hpp"

#include "converter.hpp"  // contains code that allows to convert between the Vec, Mat to numpy arrays.

namespace py = pybind11;
using namespace raisim;


void init_world(py::module &m) {

    /*********/
	/* World */
	/*********/
	py::class_<raisim::World>(m, "World", "Raisim world.", py::dynamic_attr()) // enable dynamic attributes for C++ class in Python
	    .def(py::init<>(), "Initialize the World.")
	    .def(py::init<const std::string &>(), "Initialize the World from the given config file.", py::arg("configFile"))


	    .def("set_time_step", &raisim::World::setTimeStep, R"mydelimiter(
	    Set the given time step `dt` in the simulator.

	    Args:
	        dt (float): time step to be set in the simulator.
	    )mydelimiter",
	    py::arg("dt"))


	    .def("get_time_step", &raisim::World::getTimeStep, R"mydelimiter(
	    Get the current time step that has been set in the simulator.

	    Returns:
	        float: time step.
	    )mydelimiter")


	    .def("add_sphere", &raisim::World::addSphere, R"mydelimiter(
	    Add dynamically a sphere into the world.

	    Args:
	        radius (float): radius of the sphere.
	        mass (float): mass of the sphere.
	        material (str): material to be applied to the sphere.
	        collision_group (unsigned long): collision group.
	        collision_mask (unsigned long): collision mask.

	    Returns:
	        Sphere: the sphere instance.
	    )mydelimiter",
	    py::arg("radius"), py::arg("mass"), py::arg("material") = "default", py::arg("collision_group") = 1,
	    py::arg("collision_mask") = CollisionGroup(-1),
	    py::return_value_policy::reference_internal)


	    .def("add_box", &raisim::World::addBox, R"mydelimiter(
	    Add dynamically a box into the world.

	    Args:
	        x (float): length along the x axis.
	        y (float): length along the y axis.
	        z (float): length along the z axis.
	        mass (float): mass of the box.
	        material (str): material to be applied to the box.
	        collision_group (unsigned long): collision group.
	        collision_mask (unsigned long): collision mask.

	    Returns:
	        Box: the box instance.
	    )mydelimiter",
	    py::arg("x"), py::arg("y"), py::arg("z"), py::arg("mass"), py::arg("material") = "default",
	    py::arg("collision_group") = 1, py::arg("collision_mask") = CollisionGroup(-1),
	    py::return_value_policy::reference_internal)


        .def("add_cylinder", &raisim::World::addCylinder, R"mydelimiter(
	    Add dynamically a cylinder into the world.

	    Args:
	        radius (float): radius of the cylinder.
	        height (float): height of the cylinder.
	        mass (float): mass of the cylinder.
	        material (str): material to be applied to the cylinder.
	        collision_group (unsigned long): collision group.
	        collision_mask (unsigned long): collision mask.

	    Returns:
	        Cylinder: the cylinder instance.
	    )mydelimiter",
	    py::arg("radius"), py::arg("height"), py::arg("mass"), py::arg("material") = "default",
	    py::arg("collision_group") = 1, py::arg("collision_mask") = CollisionGroup(-1),
	    py::return_value_policy::reference_internal)


	    .def("add_cone", &raisim::World::addCone, R"mydelimiter(
	    Add dynamically a cone into the world.

	    Args:
	        radius (float): radius of the cone.
	        height (float): height of the cone.
	        mass (float): mass of the cone.
	        material (str): material to be applied to the cone.
	        collision_group (unsigned long): collision group.
	        collision_mask (unsigned long): collision mask.

	    Returns:
	        Cone: the cone instance.
	    )mydelimiter",
	    py::arg("radius"), py::arg("height"), py::arg("mass"), py::arg("material") = "default",
	    py::arg("collision_group") = 1, py::arg("collision_mask") = CollisionGroup(-1),
	    py::return_value_policy::reference_internal)


	    .def("add_capsule", &raisim::World::addCapsule, R"mydelimiter(
	    Add dynamically a capsule into the world.

	    Args:
	        radius (float): radius of the capsule.
	        height (float): height of the capsule.
	        mass (float): mass of the capsule.
	        material (str): material to be applied to the capsule.
	        collision_group (unsigned long): collision group.
	        collision_mask (unsigned long): collision mask.

	    Returns:
	        Capsule: the capsule instance.
	    )mydelimiter",
	    py::arg("radius"), py::arg("height"), py::arg("mass"), py::arg("material") = "default",
	    py::arg("collision_group") = 1, py::arg("collision_mask") = CollisionGroup(-1),
	    py::return_value_policy::reference_internal)


	    .def("add_ground", &raisim::World::addGround, R"mydelimiter(
	    Add dynamically a ground into the world.

	    Args:
	        height (float): height of the ground.
	        material (str): material to be applied to the ground.
	        collision_mask (unsigned long): collision mask.

	    Returns:
	        Ground: the ground instance.
	    )mydelimiter",
	    py::arg("height") = 0., py::arg("material") = "default", py::arg("collision_mask") = CollisionGroup(-1),
	    py::return_value_policy::reference_internal)


        .def("add_heightmap", py::overload_cast<int, int, double, double, double, double, const std::vector<double> &,
            const std::string &, CollisionGroup, CollisionGroup>(&raisim::World::addHeightMap), R"mydelimiter(
	    Add a heightmap into the world.

	    Args:
	        x_samples (int): the number of samples in x.
	        y_samples (int): the number of samples in y.
            x_scale (float): the scale in the x direction.
            y_scale (float): the scale in the y direction.
            x_center (float): the x center of the heightmap in the world.
            y_center (float): the y center of the heightmap in the world.
            heights (list[float]): list of desired heights.
            material (str): material.
            group (CollisionGroup): collision group.
            mask (CollisionGroup): collision mask.

	    Returns:
	        HeightMap: the heightmap instance.
	    )mydelimiter",
	    py::arg("x_samples"), py::arg("y_samples"), py::arg("x_scale"), py::arg("y_scale"), py::arg("x_center"),
	    py::arg("y_center"), py::arg("heights"), py::arg("material") = "default", py::arg("collision_group") = 1,
	    py::arg("collision_mask") = CollisionGroup(-1),
	    py::return_value_policy::reference_internal)


        .def("add_heightmap", py::overload_cast<const std::string &, double, double, const std::string &,
            CollisionGroup, CollisionGroup>(&raisim::World::addHeightMap), R"mydelimiter(
	    Add a heightmap into the world.

	    Args:
	        filename (str): raisim heightmap filename.
            x_center (float): the x center of the heightmap in the world.
            y_center (float): the y center of the heightmap in the world.
            material (str): material.
            group (CollisionGroup): collision group.
            mask (CollisionGroup): collision mask.

	    Returns:
	        HeightMap: the heightmap instance.
	    )mydelimiter",
	    py::arg("filename"), py::arg("x_center"), py::arg("y_center"), py::arg("material") = "default",
	    py::arg("collision_group") = 1, py::arg("collision_mask") = CollisionGroup(-1),
	    py::return_value_policy::reference_internal)


        .def("add_heightmap", py::overload_cast<const std::string &, double, double, double, double, double, double,
            const std::string &, CollisionGroup, CollisionGroup>(&raisim::World::addHeightMap), R"mydelimiter(
	    Add a heightmap into the world.

	    Args:
	        filename (str): filename to the PNG.
	        x_center (float): the x center of the heightmap in the world.
            y_center (float): the y center of the heightmap in the world.
            x_size (float): the size in the x direction.
            y_size (float): the size in the y direction.
	        height_scale (float): the height scale.
	        height_offset (float): the height offset.
            material (str): material.
            group (CollisionGroup): collision group.
            mask (CollisionGroup): collision mask.

	    Returns:
	        HeightMap: the heightmap instance.
	    )mydelimiter",
	    py::arg("filename"), py::arg("x_center"), py::arg("y_center"), py::arg("x_size"), py::arg("y_size"),
	    py::arg("height_scale"), py::arg("height_offset"), py::arg("material") = "default",
	    py::arg("collision_group") = 1, py::arg("collision_mask") = CollisionGroup(-1),
	    py::return_value_policy::reference_internal)


        .def("add_heightmap", py::overload_cast<double, double, raisim::TerrainProperties&, const std::string &,
            CollisionGroup, CollisionGroup>(&raisim::World::addHeightMap), R"mydelimiter(
	    Add a heightmap into the world.

	    Args:
            x_center (float): the x center of the heightmap in the world.
            y_center (float): the y center of the heightmap in the world.
            terrain_properties (TerrainProperties): the terrain properties.
            material (str): material.
            group (CollisionGroup): collision group.
            mask (CollisionGroup): collision mask.

	    Returns:
	        HeightMap: the heightmap instance.
	    )mydelimiter",
	    py::arg("x_center"), py::arg("y_center"), py::arg("terrain_properties"),
	    py::arg("material") = "default", py::arg("collision_group") = 1, py::arg("collision_mask") = CollisionGroup(-1),
	    py::return_value_policy::reference_internal)


        .def("add_articulated_system", &raisim::World::addArticulatedSystem, R"mydelimiter(
	    Add an articulated system in the world.

	    Args:
            urdf_path (str): path to the URDF file.
            res_path (str): path to the resource directory. Leave it empty ('') if it is the urdf file directory.
            joint_order (list[str]): joint order.
            group (CollisionGroup): collision group.
            mask (CollisionGroup): collision mask.
            options (ArticulatedSystemOption): articulated system options.

	    Returns:
	        ArticulatedSystem: the articulated system instance.
	    )mydelimiter",
	    py::arg("urdf_path"), py::arg("res_path") = "", py::arg("joint_order") = std::vector<std::string>(), py::arg("collision_group") = 1,
	    py::arg("collision_mask") = CollisionGroup(-1), py::arg("options") = raisim::ArticulatedSystemOption(),
	    py::return_value_policy::reference_internal)


        .def("add_compound", [](raisim::World &self, const std::vector<raisim::Compound::CompoundObjectChild> &children,
                        double mass, py::array_t<double> inertia, CollisionGroup group=1,
                        CollisionGroup mask = CollisionGroup(-1)) {
            // convert np.array to Mat<3,3>
            Mat<3, 3> I = convert_np_to_mat<3, 3>(inertia);

            // return compound object
            return self.addCompound(children, mass, I, group, mask);
        }, R"mydelimiter(
	    Add a compound body in the world.

	    Args:
            children (list[CompoundObjectChild]): list of child object instance.
            mass (float): mass of the compound object.
            inertia (np.array[float[3,3]]): inertia matrix of the object.
            group (CollisionGroup): collision group.
            mask (CollisionGroup): collision mask.

	    Returns:
	        Compound: the compound body instance.
	    )mydelimiter",
	    py::arg("children"), py::arg("mass") = "", py::arg("inertia"), py::arg("collision_group") = 1,
	    py::arg("collision_mask") = CollisionGroup(-1),
	    py::return_value_policy::reference_internal)


        .def("add_mesh", [](raisim::World &self, const std::string & file_name,
                                double mass, py::array_t<double> inertia, py::array_t<double> com,
                                const std::string & material = "default", CollisionGroup group=1,
                                CollisionGroup mask = CollisionGroup(-1)) {
               // convert np.array to raisim matrices
               Mat<3, 3> I;
               I = convert_np_to_mat<3, 3>(inertia);
               Vec<3> COM;
               COM = convert_np_to_vec<3>(com);

               // return compound object
               return self.addMesh(file_name, mass, I, COM, material, group, mask);
             }, R"mydelimiter(
	    Add a mesh in the world.

	    Args:
            file_name (str): full path of the mesh file.
            mass (float): mass of the compound object.
            inertia (np.array[float[3,3]]): inertia matrix of the object.
            com (np.array[float[3,1]]): the location of the center of mass
            material (str): material
            group (CollisionGroup): collision group.
            mask (CollisionGroup): collision mask.

	    Returns:
	        Mesh: the mesh instance.
	    )mydelimiter",
             py::arg("file_name"), py::arg("mass"), py::arg("inertia"), py::arg("com"), py::arg("material") = "default",
             py::arg("collision_group") = 1, py::arg("collision_mask") = CollisionGroup(-1),
             py::return_value_policy::reference_internal)


        .def("add_stiff_wire", [](raisim::World &self, raisim::Object &object1, size_t local_idx1,
                py::array_t<double> pos_body1, raisim::Object &object2, size_t local_idx2,
                py::array_t<double> pos_body2, double length) {

            // convert the arrays to Vec<3>
            raisim::Vec<3> pos1 = convert_np_to_vec<3>(pos_body1);
            raisim::Vec<3> pos2 = convert_np_to_vec<3>(pos_body2);

            // return the stiff wire instance.
            return self.addStiffWire(&object1, local_idx1, pos1, &object2, local_idx2, pos2, length);
        }, R"mydelimiter(
	    Add a stiff wire constraint between two bodies in the world.

	    Args:
            object1 (Object): first object/body instance.
	        local_idx1 (int): local index of the first object/body.
	        pos_body1 (np.array[float[3]]): position of the constraint on the first body.
            object2 (Object): second object/body instance.
	        local_idx2 (int): local index of the second object/body.
	        pos_body2 (np.array[float[3]]): position of the constraint on the second body.
            length (float): length of the wire constraint.

	    Returns:
	        StiffWire: the stiff wire constraint instance.
	    )mydelimiter",
	    py::arg("object1"), py::arg("local_idx1"), py::arg("pos_body1"), py::arg("object2"), py::arg("local_idx2"),
            py::arg("pos_body2"), py::arg("length"),
            py::return_value_policy::reference_internal)


        .def("add_compliant_wire", [](raisim::World &self, raisim::Object &object1, size_t local_idx1,
                py::array_t<double> pos_body1, raisim::Object &object2, size_t local_idx2,
                py::array_t<double> pos_body2, double length, double stiffness) {

            // convert the arrays to Vec<3>
            raisim::Vec<3> pos1 = convert_np_to_vec<3>(pos_body1);
            raisim::Vec<3> pos2 = convert_np_to_vec<3>(pos_body2);

            // return the compliant wire instance.
            return self.addCompliantWire(&object1, local_idx1, pos1, &object2, local_idx2, pos2, length, stiffness);
        }, R"mydelimiter(
	    Add a compliant wire constraint between two bodies in the world.

	    Args:
            object1 (Object): first object/body instance.
	        local_idx1 (int): local index of the first object/body.
	        pos_body1 (np.array[float[3]]): position of the constraint on the first body.
            object2 (Object): second object/body instance.
	        local_idx2 (int): local index of the second object/body.
	        pos_body2 (np.array[float[3]]): position of the constraint on the second body.
            length (float): length of the wire constraint.
            stiffness (float): stiffness of the wire.

	    Returns:
	        CompliantWire: the compliant wire constraint instance.
	    )mydelimiter",
	    py::arg("object1"), py::arg("local_idx1"), py::arg("pos_body1"), py::arg("object2"), py::arg("local_idx2"),
            py::arg("pos_body2"), py::arg("length"), py::arg("stiffness"),
            py::return_value_policy::reference_internal)


        .def("get_object", &raisim::World::getObject, R"mydelimiter(
	    Get the specified object instance from its unique name.

	    Args:
            name (str): unique name of the object instance we want to get.

	    Returns:
	        Object, None: the specified object instance. None, if it didn't find the object.
	    )mydelimiter",
	    py::arg("name"),
	    py::return_value_policy::reference_internal)


	    .def("get_constraint", &raisim::World::getConstraint, R"mydelimiter(
	    Get the specified constraint instance from its unique name.

	    Args:
            name (str): unique name of the constraint instance we want to get.

	    Returns:
	        Constraints, None: the specified constraint instance. None, if it didn't find the constraint.
	    )mydelimiter",
	    py::arg("name"),
	    py::return_value_policy::reference_internal)


	    .def("get_wire", &raisim::World::getWire, R"mydelimiter(
	    Get the specified wire instance from its unique name.

	    Args:
            name (str): unique name of the wire instance we want to get.

	    Returns:
	        Constraints: the specified wire instance. None, if it didn't find the wire.
	    )mydelimiter",
	    py::arg("name"),
	    py::return_value_policy::reference_internal)


        .def("get_configuration_number", &raisim::World::getConfigurationNumber, R"mydelimiter(
	    Get the number of elements that are in the world. The returned number is updated everytime that we add or
	    remove an object from the world.

	    Returns:
	        int: the number of objects in the world.
	    )mydelimiter")


	    .def("remove_object", py::overload_cast<raisim::Object*>(&raisim::World::removeObject), R"mydelimiter(
	    Remove dynamically an object from the world.

	    Args:
	        obj (Object): the object to be removed from the world.
	    )mydelimiter",
	    py::arg("obj"))


	    .def("remove_object", py::overload_cast<raisim::StiffWire*>(&raisim::World::removeObject), R"mydelimiter(
	    Remove dynamically a stiff wire from the world.

	    Args:
	        wire (StiffWire): the stiff wire to be removed from the world.
	    )mydelimiter",
	    py::arg("wire"))


	    .def("remove_object", py::overload_cast<raisim::CompliantWire*>(&raisim::World::removeObject), R"mydelimiter(
	    Remove dynamically a compliant wire from the world.

	    Args:
	        wire (CompliantWire): the compliant wire to be removed from the world.
	    )mydelimiter",
	    py::arg("wire"))


	    .def("integrate", &raisim::World::integrate, "this function is simply calling both `integrate1()` and `integrate2()` one-by-one.")


	    .def("integrate1", &raisim::World::integrate1, R"mydelimiter(
        It performs:
        1. deletion contacts from previous time step
        2. collision detection
        3. register contacts to each body
        4. calls `preContactSolverUpdate1()` of each object
        )mydelimiter")


        .def("integrate2", &raisim::World::integrate2, R"mydelimiter(
        It performs
        1. calls `preContactSolverUpdate2()` of each body
        2. run collision solver
        3. calls `integrate` method of each object
        )mydelimiter")


        // TODO: improve the doc for the below method
        .def("get_contact_problems", &raisim::World::getContactProblem, R"mydelimiter(
        Return the list of contacts.
        )mydelimiter")


        .def("get_object_list", &raisim::World::getObjList, R"mydelimiter(
        Return the list of object instances that are in the world.

        Returns:
            list[Object]: list of object instances.
        )mydelimiter")


        .def("update_material_property", &raisim::World::updateMaterialProp, R"mydelimiter(
        Update material property.

        Args:
            prop (MaterialManager): material manager property instance.
        )mydelimiter",
        py::arg("prop"))


        .def("set_material_pair_properties", &raisim::World::setMaterialPairProp, R"mydelimiter(
        Set material pair properties.

        Args:
            material1 (str): first material.
            material2 (str): second material.
            friction (float): coefficient of friction.
            restitution (float): coefficient of restitution.
            threshold (float): restitution threshold.
        )mydelimiter",
        py::arg("material1"), py::arg("material2"), py::arg("friction"), py::arg("restitution"), py::arg("threshold"))


        .def("set_default_material", &raisim::World::setDefaultMaterial, R"mydelimiter(
        Set the default material.

        Args:
            friction (float): coefficient of friction.
            restitution (float): coefficient of restitution.
            threshold (float): restitution threshold.
        )mydelimiter",
        py::arg("friction"), py::arg("restitution"), py::arg("threshold"))


        .def("get_gravity", [](raisim::World &world) {
            Vec<3> gravity = world.getGravity();
            return convert_vec_to_np(gravity);
        }, R"mydelimiter(
        Get the gravity vector from the world.

        Returns:
            np.array[float[3]]: gravity vector.
        )mydelimiter")


        .def("set_gravity", [](raisim::World &world, py::array_t<double> array) {
            raisim::Vec<3> gravity = convert_np_to_vec<3>(array);
            world.setGravity(gravity);
        }, R"mydelimiter(
        Set the gravity vector in the world.

        Args:
            np.array[float[3]]: gravity vector.
        )mydelimiter", py::arg("gravity"))


        .def("set_erp", &raisim::World::setERP, "Set the error reduction parameter (ERP).", py::arg("erp"), py::arg("erp2")=0)


        .def("set_contact_solver_parameters", &raisim::World::setContactSolverParam, R"mydelimiter(
        Set contact solver parameters.

        Args:
            alpha_init (float): alpha init.
            alpha_min (float): alpha minimum.
            alpha_decay (float): alpha decay.
            max_iters (float): maximum number of iterations.
            threshold (float): threshold.
        )mydelimiter",
        py::arg("alpha_init"), py::arg("alpha_min"), py::arg("alpha_decay"), py::arg("max_iters"), py::arg("threshold"))


        .def("get_world_time", &raisim::World::getWorldTime, R"mydelimiter(
        Return the total integrated time (which is updated at every `integrate2()`` call).

        Returns:
            float: world time.
        )mydelimiter")


        .def("set_world_time", &raisim::World::setWorldTime, R"mydelimiter(
        Set the world time.

        Args:
            time (float): world time
        )mydelimiter", py::arg("time"))


        .def("get_contact_solver", py::overload_cast<>(&raisim::World::getContactSolver), R"mydelimiter(
        Return the bisection contact solver used.

        Returns:
            BisectionContactSolver: contact solver.
        )mydelimiter");

}