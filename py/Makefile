# build pyc files

# this makefile is for building all python modules which are used for other C++ programs
# including hash functions, guis, etc.
# this makefile is only tested under Windows, not sure if the cross-platform check works under Linux

# this makefile provides make and clean function
# for clean function it will delete __pycache__ folder, which is created during the compilation, and also all .pyc / .pyo files under this folder
# that means: all compiled python files will be deleted (but that's how it should work isn't it)

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
# under Windows

ifeq (${ARCH},Linux)
COPY = cp
# copy file
RD = rm -rf
# remove dir
DEL = rm
# delete files
PATH_SEPARATOR = /
endif
# under Linux

PYTHON_EDITION = cpython-310
# python edition

TARGET_NUM = 2

HASHFILE = hashfile
GUI = gui

all:build

build:
	@${PYTHON} ${PYTHON_FLAG} ${HASHFILE}.py
	@echo [ 1/${TARGET_NUM}] Compiling python target: ${HASHFILE}
	@${PYTHON} ${PYTHON_FLAG} ${GUI}.py
	@echo [ 2/${TARGET_NUM}] Compiling python target: ${GUI}

	@${COPY} __pycache__${PATH_SEPARATOR}${HASHFILE}.${PYTHON_EDITION}.${PYTHON_COMPILED_FILE_FE} ${HASHFILE}.${PYTHON_COMPILED_FILE_FE}
	@${COPY} __pycache__${PATH_SEPARATOR}${GUI}.${PYTHON_EDITION}.${PYTHON_COMPILED_FILE_FE} ${GUI}.${PYTHON_COMPILED_FILE_FE}

clean:
	@${RD} __pycache__
	@${DEL} *.${PYTHON_COMPILED_FILE_FE}

help:
	@echo The following are the valid targets for this Makefile:
	@echo ... all:   compile all targets (the default if no target is provided)
	@echo ... build: same as all
	@echo ... clean: delete all compiled python files
	@echo ... help:  show this page