#!/bin/bash

# Define the variable ABSL_LIBS by finding all Abseil shared libraries
# in /usr/local/lib, extracting their base names, removing 'lib' prefix and '.so' suffix,
# sorting them uniquely, and then prepending '-l' to each.
ABSL_LIBS=$(find /usr/local/lib -maxdepth 1 -name 'libabsl_*.so' -exec basename {} \; | sed -e 's/^lib//' -e 's/\.so.*//' | sort -u )

# Compile and link the C++ source files (main.cc and test.pb.cc)
# using g++, linking against libprotobuf, the discovered Abseil libraries,
# libpthread, and libutf8_range.
g++ main.cc test.pb.cc -lprotobuf $ABSL_LIBS -lpthread -lutf8_range