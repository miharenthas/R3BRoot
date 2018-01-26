# Create a library for the bodgelogger, it's a plugin needed to save and read
#an event header with the ascii generator custom
#should be irrelevant for everybody else.

Set(SYSTEM_INCLUDE_DIRECTORIES 
${SYSTEM_INCLUDE_DIRECTORIES}
${BASE_INCLUDE_DIRECTORIES}
)

set(INCLUDE_DIRECTORIES ${R3BROOT_SOURCE_DIR}/bodgelogger )

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
)
 
link_directories( ${LINK_DIRECTORIES})

Set(LINKDEF BodgeloggerLinkDef.h)
Set(LIBRARY_NAME bodgelogger) 


set(SRCS
r3b_ascii_blog.cc
r3b_ascii_bodgelogger.cc
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cc *.h HEADERS "${SRCS}")

GENERATE_LIBRARY()
