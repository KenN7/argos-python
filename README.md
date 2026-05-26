# ARGoS Python Wrapper

This repository lets you write [ARGoS3](https://github.com/ilpincy/argos3) controllers, loop functions, and Qt user functions in Python, and then run experiments without recompiling C++.

The wrapper is implemented in C++ and uses Boost.Python to bridge ARGoS interfaces into Python objects. 

Note: This branch is distinct because it does not use Python subinterpreters. Scripts run in the main interpreter, improving compatibility with Python packages such as NumPy, ROS 2 Python libraries and many others.

## Dependencies

- ARGoS3 installed on the system
- Optional: e-puck plugins for the e-puck examples

This branch has been developed on Ubuntu 22.04 with GCC 11 and Python 3.10.

## Build

```sh
mkdir -p build
cd build
cmake ..
make
```

The build creates three shared libraries:

- `build/libpy_controller_interface.so`
- `build/libpy_loop_function_interface.so`
- `build/libpy_qtuser_function_interface.so`

## Using The Wrapper

A Python controller is declared in the `<controllers>` section of an `.argos`
file:

```xml
<python_controller id="my_controller"
                   library="../../build/libpy_controller_interface.so">
  <params script="../controllers/my_controller.py" />
</python_controller>
```

A Python loop function is declared at the top level:

```xml
<loop_functions library="../../build/libpy_loop_function_interface.so"
                label="py_loop_function">
  <params script="../loop_functions/my_loop_function.py" />
</loop_functions>
```

A Python Qt user function is declared inside `<qt-opengl>`:

```xml
<qt-opengl>
  <user_functions library="../../build/libpy_qtuser_function_interface.so"
                  label="py_qtuser_function">
    <params script="../loop_functions/my_qt_user.py" />
  </user_functions>
</qt-opengl>
```

## Examples

The `examples` folder contains a small subset of the `argos3-examples` experiments adapted to Python. 

To run examples:

```sh
cd examples/experiments
argos3 -c diffusion_1.argos
```

Included examples:

- `diffusion_1.argos` and `diffusion_10.argos`: foot-bot diffusion.
- `epuck_avoidance.argos`: e-puck obstacle avoidance.
- `flocking.argos`: foot-bot light-following flocking.
- `foraging.argos`: e-puck foraging with Python controller, loop function, and Qt user function.
- `gripping.argos`: foot-bot gripper demo.
- `synchronization.argos`: LED/camera synchronization.

Python controllers live in `examples/controllers`. The foraging loop and Qt user
functions live in `examples/loop_functions/foraging`.

Note: If ARGoS is launched from a Snap-packaged editor terminal and Qt fails with a
`/snap/core20/.../libpthread.so.0` symbol lookup error, run ARGoS from a normal
system terminal or with a clean environment.
