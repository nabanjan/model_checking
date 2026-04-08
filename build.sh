#!/bin/bash
mkdir -p build && clang++ -std=c++20 -g -o build/model_check model_check/model_check.cpp model_check/StdAfx.cpp
