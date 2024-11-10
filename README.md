# CMake Setup

Ensure you have CMake downloaded

After copying github repo:
1. mkdir build
2. cd build
3. cmake .. (reads CMakeLists.txt in repo to generate build files)
4. make
5. Then within build, you can run ./Client or ./Server to run executables

Whenever you need to add an external library, edit the CMakeLists.txt file (ask chatgbt what the specific line is). 

Example of linking math library to both files within CMakeLists.txt:
``` cmake
target_link_libraries(Client m) # Example for linking math library (-lm)
target_link_libraries(Server m) # Example for linking math library (-lm)
```
