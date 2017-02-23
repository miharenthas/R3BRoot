# Create a library called "libMVPlanes" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(SYSTEM_INCLUDE_DIRECTORIES 
${SYSTEM_INCLUDE_DIRECTORIES}
${BASE_INCLUDE_DIRECTORIES}
)

set(INCLUDE_DIRECTORIES
${R3BROOT_SOURCE_DIR}/r3bbase
${R3BROOT_SOURCE_DIR}/r3bdata  
${R3BROOT_SOURCE_DIR}/mvplanes  )

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
)
 
link_directories( ${LINK_DIRECTORIES})

# this doesn't seem to work on the first go...
Set(LINKDEF MVPLinkDef.h)
Set(LIBRARY_NAME R3BMVPlanes) 
Set(DEPENDENCIES
    R3Bbase
    R3BData)

set(SRCS
R3BRattlePlane.cxx
R3BRPHit.cxx 
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

GENERATE_LIBRARY()
