#!/bin/sh

g++ list_main.cpp list.cpp file_using.cpp -Wall -Wextra -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr -o a.out
