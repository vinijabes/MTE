#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Kinematics/Core/Log.h"

#include "Kinematics/Debug/Instrumentor.h"

#define MTE_PLATFORM_WINDOWS

#ifdef MTE_PLATFORM_WINDOWS
#include <Windows.h>
#endif