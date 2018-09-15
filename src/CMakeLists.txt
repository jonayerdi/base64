cmake_minimum_required (VERSION 3.8.1)

project (base64)

set(HEADERS
            base64.h
            base64_file.h
            )
set(SRCS
            base64.c
            base64_file.c
			base64_app.c
            )
			
add_executable(base64 ${SRCS} ${HEADERS})

set_property(TARGET base64 PROPERTY C_STANDARD 99)
