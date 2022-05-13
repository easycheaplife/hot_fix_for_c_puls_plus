#!/bin/bash
g++ -fPIC -shared app.cc -o libapp.so
g++ -fPIC -shared patch.cc -o patch.so
g++ -g -rdynamic -o main ../hot_fix/hot_fix.h app.h main.cc -L ../hot_fix -L. -lapp -lhot_fix -ldl
