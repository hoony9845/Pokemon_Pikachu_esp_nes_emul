#
# Main component makefile.
#
# This Makefile can be left empty. By default, it will take the sources in the
# src/ directory, compile them and link them into lib(subdirectory_name).a
# in the build directory. This behaviour is entirely configurable,
# please read the ESP-IDF documents if you need to do this.

#COMPONENT_EMBED_FILES := image.jpg
ifdef CONFIG_LCD_FACE_1
COMPONENT_EMBED_FILES := face1.jpg
endif

ifdef CONFIG_LCD_FACE_2
COMPONENT_EMBED_FILES := face2.jpg
endif


