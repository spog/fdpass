#
# The "fdpass" library build rules
#
# Copyright (C) 2015 Samo Pogacnik <samo_pogacnik@t-2.net>
# All rights reserved.
#
# This file is part of the "fdpass" software project.
# This file is provided under the terms of the BSD 3-Clause license,
# available in the LICENSE file of the "fdpass" software project.
#

# Create a library called "fdpass" which includes the source files "oredredstr.c, ...".
# The extension is already found. Any number of sources could be listed here.
add_library (fdpass SHARED fdpass.c) 

set_target_properties (fdpass PROPERTIES VERSION ${project_VERSION_STRING} SOVERSION ${project_VERSION_MAJOR})

install(TARGETS fdpass DESTINATION lib COMPONENT libs)
install(FILES ${project_SOURCE_DIR}/include/libfdpass/fdpass.h DESTINATION include/libfdpass COMPONENT headers)
