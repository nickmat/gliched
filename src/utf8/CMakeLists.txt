# utf8proc library

set(UTF8_SOURCES 
    utf8api.cpp
    utf8proc.c
)

set( UTF8_HEADERS
    ../../include/utf8/utf8api.h
    utf8proc.h
    utf8proc_data.h # Note, we have renamed this from utf8proc_data.c
)

add_library( utf8api ${UTF8_SOURCES} ${UTF8_HEADERS} )

