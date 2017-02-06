#include <boost/python.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


// Taken from http://thejosephturner.com/blog/post/embedding-python-in-c-applications-with-boostpython-part-2/
std::string parse_python_exception()
{
    namespace py = boost::python;

    PyObject *type_ptr = NULL, *value_ptr = NULL, *traceback_ptr = NULL;
    // Fetch the python error
    PyErr_Fetch(&type_ptr, &value_ptr, &traceback_ptr);
    // Set some fallback values
    std::string ret("Unfetchable Python error");

    // Try to obtain the type of error (e.g. SyntaxError)
    // check that there is actually a valid pointer to the type data.
    if(type_ptr != NULL){
        py::handle<> h_type(type_ptr);
        py::str type_pstr(h_type);
        py::extract<std::string> e_type_pstr(type_pstr);
        if(e_type_pstr.check())
            ret = e_type_pstr();
        else
            ret = "Unknown exception type";
    }

    // Find the value that caused the error
    if(value_ptr != NULL){
        py::handle<> h_val(value_ptr);
        py::str a(h_val);
        py::extract<std::string> returned(a);
        if(returned.check())
            ret +=  ": " + returned();
        else
            ret += std::string(": Unparseable Python error: ");
    }

    // Find where the error occured
    if(traceback_ptr != NULL){
        py::handle<> h_tb(traceback_ptr);
        py::object tb(py::import("traceback"));
        py::object fmt_tb(tb.attr("format_tb"));
        py::object tb_list(fmt_tb(h_tb));
        py::object tb_str(py::str("\n").join(tb_list));
        py::extract<std::string> returned(tb_str);
        if(returned.check())
            ret += ": " + returned();
        else
            ret += std::string(": Unparseable Python traceback");
    }
    return ret;
}

int main()
{
    namespace py = boost::python;
    
    try{
        Py_Initialize();
        py::object main_module = py::import("__main__");
        py::object main_namespace = main_module.attr("__dict__");

        py::exec("import sys", main_namespace);
        py::exec("import os", main_namespace);
        py::exec("sys.path.append(os.path.abspath('../common'))", main_namespace);
        py::exec("sys.path.append(os.path.abspath('./'))", main_namespace);

        // py::exec("from TAM import *", main_namespace);

        // Import the module "test_cpp"
        py::object test_module = py::import("test_cpp");
        // Save into "func_test" a function of the module "test_cpp"
        py::object func_test = test_module.attr("func_test")();
        // Instantiate an object of class "Test", and store it into "test_obj""
        py::object test_obj = test_module.attr("Test")();
        // Call a function of "test_obj", and pass the parameter "2" to it.
        std::cout << py::extract<double>(test_obj.attr("param_test_class")(2)) << std::endl;
        // Modify an object attribute
        test_obj.attr("a") = 100;
        std::cout << py::extract<double>(test_obj.attr("param_test_class")(2)) << std::endl;

        // Save a class method as variable 
        // NOTE: the method will access the class attribute of the object to which it refers, so be careful!
        py::object method_1 = test_obj.attr("param_test_class");
        std::cout << py::extract<double>(method_1(2)) << std::endl;



    } catch(boost::python::error_already_set const &){
        std::string p_error_str = parse_python_exception();
        std::cout << "Error in Python: " << p_error_str << std::endl;
    }
}