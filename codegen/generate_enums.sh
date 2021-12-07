#!

GLEXT_FILE_NAME=glext.h
DEFINES_FILE_NAME=defines.txt

HERE_PATH=${0%generate_enums.sh}
echo $HERE_PATH

cd $HERE_PATH
cat $GLEXT_FILE_NAME | grep "#define" > $DEFINES_FILE_NAME
lua generate_enums.lua $DEFINES_FILE_NAME
