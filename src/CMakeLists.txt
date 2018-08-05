cmake_minimum_required (VERSION 3.8.1)

project (base64)
set(HEADERS
            base64.h
            )
set(SRCS
            base64.c
			base64_app.c
            )
add_executable(base64 ${SRCS} ${HEADERS})
#install(TARGETS base64 DESTINATION bin)
