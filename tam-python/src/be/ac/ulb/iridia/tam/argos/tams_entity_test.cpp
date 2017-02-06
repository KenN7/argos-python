#include <boost/python.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <bitset>

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/datatypes/color.h>



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

class TAMtest {

public:
    TAMtest(const std::string id)
    {
        try{
        Py_Initialize();
        main_module = boost::python::import("__main__");
        main_namespace = main_module.attr("__dict__");

        boost::python::exec("import sys", main_namespace);
        boost::python::exec("import os", main_namespace);
        boost::python::exec("sys.path.append(os.path.abspath('../common'))", main_namespace);
        boost::python::exec("sys.path.append(os.path.abspath('./'))", main_namespace);

        // Import the module "TAM_argos"
        tam_module = boost::python::import("TAM_argos");
        
        tam = CreateTAM(id);

        std::cout << "Created TAM:\n" << std::string(boost::python::extract<std::string>(tam.attr("__str__")())) << std::endl;        
        } catch(boost::python::error_already_set const &){
            std::string p_error_str = parse_python_exception();
            std::cout << "Error in Python: " << p_error_str << std::endl;
        }
    }

    std::string tam_to_string() const 
    {
        try{
            return std::string(boost::python::extract<std::string>(tam.attr("__str__")()));
        } catch(boost::python::error_already_set const &){
            std::string p_error_str = parse_python_exception();
            std::cout << "Error in Python: " << p_error_str << std::endl;
        }
    }

    argos::CColor get_led_color()
    {
        try {
            return argos::CColor(
                boost::python::extract<argos::UInt8>(tam.attr("get_led_color")().attr("get_red_channel_value")()),
                boost::python::extract<argos::UInt8>(tam.attr("get_led_color")().attr("get_green_channel_value")()),
                boost::python::extract<argos::UInt8>(tam.attr("get_led_color")().attr("get_blue_channel_value")())
            );
        } catch(boost::python::error_already_set const &){
            std::string p_error_str = parse_python_exception();
            std::cout << "Error in Python: " << p_error_str << std::endl;
        }
    }
    
private:
    boost::python::object main_module;
    boost::python::object main_namespace;

    boost::python::object tam_module;
    boost::python::object tam;

    boost::python::object CreateTAM(const std::string id) 
    {
        return tam_module.attr("TAM")(id);
    }
};

std::ostream& operator<<(std::ostream& o, const TAMtest& t) 
{
    return o << t.tam_to_string();
}

int main()
{
    int length = 3;
    std::vector<TAMtest> TAMvec;

    for(int i = 0; i < length; i++)
    {
        TAMvec.push_back(TAMtest("TAM" + std::to_string(i)));
    }

    std::cout << "\n--------------------\nLIST OF TAMs:\n\n";
    for(int i = 0; i < length; i++)
    {
        std::cout << TAMvec[i] << std::endl;
        std::cout << "red:" << std::bitset<8>(TAMvec[i].get_led_color().GetRed()) << std::endl;
        std::cout << "green:" << std::bitset<8>(TAMvec[i].get_led_color().GetGreen()) << std::endl;
        std::cout << "blue:" << std::bitset<8>(TAMvec[i].get_led_color().GetBlue()) << std::endl;
    }

    return 0;
}


