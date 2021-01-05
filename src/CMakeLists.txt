file(GLOB CCM_Files RELATIVE "CCM" "*.cpp")
file(GLOB COMMON_Files RELATIVE "COMMON" "*.cpp")
file(GLOB IDP_Files RELATIVE "IDP" "*.cpp")
file(GLOB PRE_Files RELATIVE "PRE" "*.cpp")
file(GLOB SYSTEM_Files RELATIVE "SYSTEM" "*.cpp")

##### Executable file and Linker Setup #####
add_executable(model main.cpp ${COMMON_Files} ${CCM_Files} ${IDP_Files} ${PRE_Files} ${SYSTEM_Files} COMMON/Debug.h)
target_link_libraries(model systemc.so)