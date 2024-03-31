#pragma once
#include <cstdio>
#include <stdlib.h>
#include <list>
#include "hhk.h"
#define error(...) do { std::printf(__VA_ARGS__); Debug_WaitKey(); exit(EXIT_FAILURE); } while (0)

extern std::list<void *> *dealloc_list;