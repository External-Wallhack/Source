#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h> 
#include <d3d9.h>
#include <d3dx9.h>
#include <stdint.h>
#include <Dwmapi.h>
#include <vector>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <TlHelp32.h>
#include <WinInet.h>
#include <stddef.h>

using namespace std;
//
//#include "vmprotectsdk.h"
//#pragma comment(lib, "vmprotectsdk32.lib")

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "Draw.h"
#include "Memory.h"
#include "Matrix.h"
#include "SAMP.h"
#include "Protect.h"
#include "License.h"

#define SAMP_INFO  0x21A0F8
#define SAMP_POOLS 0x3CD

#define ENABLE_LICENSING 1
