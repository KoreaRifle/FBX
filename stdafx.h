#pragma once

#include <Windows.h>
#include <assert.h>
#include <time.h>
#pragma comment(lib, "winmm.lib")

#include <string>
#include <vector>
#include <list>
#include <map>
#include <functional>
#include <fstream>
#include <algorithm>
using namespace std;

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#define FBXSDK_SHARED
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk.lib")

using namespace fbxsdk;

#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p)=NULL; } }
#define SAFE_DELETE(p) { if (p) { delete (p); (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p); (p)=NULL; } }

#include "AntTweakBar.h"
#pragma comment(lib, "AntTweakBar.lib")

#include "./System/D3D.h"
#include "./System/Keyboard.h"
#include "./System/Mouse.h"
#include "./System/Frames.h"
#include "./System/TweakBar.h"

#include "./Render/Texture.h"
#include "./Render/Rasterizer.h"
#include "./Render/Sampler.h"
#include "./Render/DepthStencil.h"
#include "./Render/Blender.h"
#include "./Render/VertexLayout.h"
#include "./Render/UserInterface.h"

#include "./Camera/Camera.h"

#include "./Utility/String.h"
#include "./Utility/BinaryFile.h"
#include "./Utility/Mesh.h"

#define CAMERA Camera::Get()
#define KEYBOARD Keyboard::Get()
#define MOUSE Mouse::Get()