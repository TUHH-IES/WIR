#!/bin/bash

#find WIR -name "*.cc" -type f | grep -v tests | grep -v tcasmlex.cc | grep -v tcasmyacc.cc > cppcheck.list

/opt/local/cppcheck/2.13.0/bin/cppcheck --inline-suppr -j 6 --file-list=cppcheck.list --enable=warning,style,performance,information --suppress=missingIncludeSystem --suppress=*:tcasmyacc.hh --platform=unix64 --language=c++ --std=c++17 --xml --xml-version=2 \
-I ./WIR \
-I ./BUILD/WIR \
-I ./LIBUSEFUL \
-D__x86_64__ \
-DHAVE_CONFIG_H=1 2>&1 >/dev/null | tee cppcheck.xml
