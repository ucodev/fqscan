#!/bin/bash

if [ -f .done ]; then
	echo "Already done. To redo, type 'clean' first."
	exit 1
fi

echo "Doing..."

echo "/usr/bin/g++" > .compiler

if [ -f `which clang++` ]; then
	echo `which clang++` > .compiler
elif [ -f `which g++` ]; then
	echo `which g++` > .compiler
else
	echo "No suitable C++ compiler found in your system."
	exit 1
fi

echo "/usr/bin/gcc" > .compiler_c

if [ -f `which clang++` ]; then
	echo `which clang` > .compiler_c
elif [ -f `which g++` ]; then
	echo `which g` > .compiler_c
else
	echo "No suitable C compiler found in your system."
	exit 1
fi


echo '/usr/local' > .install_dir

/usr/bin/make

if [ $? -eq 0 ]; then
	/usr/bin/touch .done
	echo "Done."
	exit 0
fi

echo "Error occured."

