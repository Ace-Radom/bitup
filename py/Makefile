# build pyc files

PYTHON = python
PYTHON_FLAG = -m py_compile

ifeq (${PYTHON_FLAG},-m py_compile)

PYTHON_COMPILED_FILE_FE = pyc

else

PYTHON_COMPILED_FILE_FE = pyo

endif
# filename extension for python compiled file (.pyc or .pyo)

ifeq (${OS},Windows_NT)

COPY = copy
# copy file
RD = rmdir /s /q
# remove dir
DEL = del /q
# delete files
PATH_SEPARATOR = \\

endif

ifeq (${ARCH},Linux)

COPY = cp
# copy file
RD = rm -rf
# remove dir
PATH_SEPARATOR = /

endif

PYTHON_EDITION = cpython-310
# python edition

TARGET_NUM = 1

all:build

build:
	@${PYTHON} ${PYTHON_FLAG} hashfile.py
	@echo [1/${TARGET_NUM}] Compiling hashfile.py

	@${COPY} .${PATH_SEPARATOR}__pycache__${PATH_SEPARATOR}hashfile.${PYTHON_EDITION}.${PYTHON_COMPILED_FILE_FE} .${PATH_SEPARATOR}hashfile.${PYTHON_COMPILED_FILE_FE}

clean:
	@${RD} __pycache__
	@${DEL} *.${PYTHON_COMPILED_FILE_FE}