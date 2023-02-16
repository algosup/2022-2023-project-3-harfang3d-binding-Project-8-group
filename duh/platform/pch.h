// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <codecvt>
#include <condition_variable>
#include <cstdint>
#include <fstream>
#include <functional>
#include <future>
#include <locale>
#include <map>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#if WIN32
#include <Windows.h> // do not reorder

#include <DbgHelp.h>
#include <WTypes.h>
#include <WinBase.h>
#undef max
#endif
