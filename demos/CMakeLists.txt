#
# The demo programs build rules
#
# Copyright (C) 2015 Samo Pogacnik <samo_pogacnik@t-2.net>
# All rights reserved.
#
# This file is part of the DAEMONIZE software project.
# This file is provided under the terms of the BSD 3-Clause license,
# available in the LICENSE file of the "daemonize" software project.
#

# Add executable called "hello_daemonize" that is built from the source files  
# "hello_daemonize.c" and "hello_daemonize.h".  
add_executable (hello_fdpass hello_fdpass.c)    
        
# Make sure the linker can find the Hello library once it is built.  
link_directories (${project_BINARY_DIR}/fdpass)

# Link the executable to the daemonize library.  
target_link_libraries (hello_fdpass fdpass)

install(TARGETS hello_fdpass RUNTIME DESTINATION bin COMPONENT demos)

