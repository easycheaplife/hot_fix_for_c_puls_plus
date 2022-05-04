#!/bin/bash
g++ -fPIC -shared app.cc patch.cc -o patch.so
g++ -g -rdynamic -o main app.cc main.cc ../hot_fix/hot_fix.h -L ../hot_fix -lhot_fix -ldl
