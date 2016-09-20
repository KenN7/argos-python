# argos-python

## How to use
copy all files in the plugin folder of argos simulator
then build.

### Building instructions
```bash
cd argos-python
cmake .
make
```
### Controller
Now you use the python_controller in your .argos configuration like so :
```xml
<python_controller id="whatever" library="/path/to/compiled/libpy_controller_interface.so">
```

don't forget to also add :
```xml
<params script="/path/to/script.py" />
```
at the end of controller section

### python script
based on the test.py file, use the embedded functions (controlstep, init, destroy, reset...)
the global object "robot" is the one where there should be all the methods for the robot.

For now only the steering actuator works so it's kind of useless ;) ^^
