//==========
// LibOvr.h
//==========

#pragma once


//=======
// Using
//=======

#include "Graphics/DirectX/Version.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Ovr {


//================
// Initialization
//================

#define OVR_INITFLAG_REQUESTVERSION 4

typedef struct ALIGN(8)
{
UINT uFlags;
UINT uMinorVersion;
VOID* pCallback;
VOID* pParam;
UINT uTimeout;
IF64(UINT uReserved);
}OVR_INITPARAMS;


//=========
// Session
//=========

typedef VOID OVR_SESSION;


//========
// Matrix
//========

typedef struct
{
FLOAT M[4][4];
}OVR_MATRIX;


//===============
// Field of View
//===============

typedef struct
{
FLOAT fUpTan;
FLOAT fDownTan;
FLOAT fLeftTan;
FLOAT fRightTan;
}FOV;


//=============
// Description
//=============

typedef struct
{
UINT uType;
IF64(UINT uReserved0;)
CHAR pProductName[64];
CHAR pManufacturer[64];
SHORT uVendorId;
SHORT uProductId;
CHAR pSerialNumber[24];
SHORT uFirmwareMajor;
SHORT uFirmwareMinor;
UINT uAvailableHmdCaps;
UINT uDefaultHmdCaps;
UINT uAvailableTrackingCaps;
UINT uDefaultTrackingCaps;
FOV pDefaultEyeFov[2];
FOV pMaxEyeFov[2];
INT iResX;
INT iResY;
FLOAT fDisplayRefreshRate;
IF64(UINT uReserved1);
}HMD_DESC;


//==========
// Textures
//==========

#define OVR_FORMAT_R8G8B8A8_UNORM_SRGB 5

typedef struct
{
UINT uType;
UINT uFormat;
INT iArraySize;
INT iWidth;
INT iHeight;
INT iMipLevels;
INT iSampleCount;
BOOL bStaticImage;
UINT uMiscFlags;
UINT uBindFlags;
}OVR_TEXTURE_DESC;

typedef VOID OVR_TEXTURE_SWAP_CHAIN;

/*typedef struct
{
UINT uRenderApi;
UINT uWidth;
UINT uHeight;
IF64(UINT uReserved;)
ID3D11Texture2D* pTexture;
ID3D11ShaderResourceView* pShaderResourceView;
UINT_PTR pReserved[6];
}OVR_TEXTURE;

typedef struct
{
TEXTURE* pTextures;
UINT uCount;
UINT uCurrent;
}TEXTURE_SET;*/


//=================
// Eye Description
//=================

typedef struct
{
UINT uEye;
FOV cFov;
::RECT DistortedViewport;
FLOAT2 vPixelsPerTanAngleAtCenter;
FLOAT3 vHmdToEyeViewOffset;
}EYE_RENDER_DESC;


//======
// Pose
//======

typedef struct
{
FLOAT4 vRotation;
FLOAT3 vPosition;
}POSE;

typedef struct
{
POSE cPose;
FLOAT3 vAngularVelocity;
FLOAT3 vLinearVelocity;
FLOAT3 vAngularAcceleration;
FLOAT3 vLinearAcceleration;
UINT uReserved;
DOUBLE dTime;
}POSE_STATE;


//=========
// Sensors
//=========

typedef struct
{
FLOAT3 vAccelerometer;
FLOAT3 vGyrometer;
FLOAT3 vMagnetometer;
FLOAT fTemperature;
FLOAT fTime;
}SENSOR_DATA;


//=================
// Tracking Status
//=================

typedef struct
{
POSE_STATE cHeadPose;
POSE cCameraPose;
POSE cLeveledCameraPose;
POSE_STATE pHandPoses[2];
SENSOR_DATA RawSensorData;
UINT uStatusFlags;
UINT pHandStatusFlags[2];
UINT uLastCameraFrameCounter;
UINT uReserved;
}TRACKING_STATUS;


//=======
// Layer
//=======

typedef struct
{
UINT uType;
UINT uFlags;
OVR_TEXTURE_SWAP_CHAIN* ppTextureSwapChains[2];
::RECT pViewports[2];
FOV pFovs[2];
POSE pEyePoses[2];
DOUBLE dSensorSampleTime;
}LAYER;


//===========
// Interface
//===========

typedef VOID (__cdecl *CALC_EYE_POSES)(POSE Head, FLOAT3* HmdToEyes, POSE* EyePoses);
typedef INT (__cdecl *COMMIT_TEXTURE_SWAP_CHAIN)(OVR_SESSION* Session, OVR_TEXTURE_SWAP_CHAIN* SwapChain);
typedef INT (__cdecl *CREATE_HMD)(OVR_SESSION** Session, LUID* Id);
typedef INT (__cdecl *CREATE_TEXTURE_SWAP_CHAIN)(OVR_SESSION* Session, ID3D11Device* Device, const OVR_TEXTURE_DESC* Description, OVR_TEXTURE_SWAP_CHAIN** SwapChain);
typedef VOID (__cdecl *DESTROY_HMD)(OVR_SESSION* Session);
typedef VOID (__cdecl *DESTROY_TEXTURE_SWAP_CHAIN)(OVR_SESSION* Session, OVR_TEXTURE_SWAP_CHAIN* SwapChain);
typedef VOID (__cdecl *GET_EYE_POSES)(OVR_SESSION* Session, UINT64 Frame, BOOL LatencyMarker, FLOAT3* HmdToEyes, POSE* EyePoses, DOUBLE* SampleTime);
typedef ::SIZE (__cdecl *GET_FOV_TEXTURE_SIZE)(OVR_SESSION* Session, UINT Eye, FOV Fov, FLOAT PixelsPerDisplayPixel);
typedef DOUBLE (__cdecl *GET_FRAME_TIME)(OVR_SESSION* Session, UINT64 Frame);
typedef HMD_DESC (__cdecl *GET_HMD_DESC)(OVR_SESSION* Session);
typedef EYE_RENDER_DESC (__cdecl *GET_RENDER_DESC)(OVR_SESSION* Session, UINT Eye, FOV Fov);
typedef INT (__cdecl *GET_TEXTURE_SWAP_CHAIN_BUFFER)(OVR_SESSION* Session, OVR_TEXTURE_SWAP_CHAIN* SwapChain, INT Id, IID Iid, VOID** Object);
typedef INT (__cdecl *GET_TEXTURE_SWAP_CHAIN_CURRENT_INDEX)(OVR_SESSION* Session, OVR_TEXTURE_SWAP_CHAIN* SwapChain, UINT* Index);
typedef INT (__cdecl *GET_TEXTURE_SWAP_CHAIN_LENGTH)(OVR_SESSION* Session, OVR_TEXTURE_SWAP_CHAIN* SwapChain, UINT* Count);
typedef DOUBLE (__cdecl *GET_TIME)();
typedef TRACKING_STATUS (__cdecl *GET_TRACKING_STATE)(OVR_SESSION* Session, DOUBLE FrameTime, BOOL LatencyMarker);
typedef INT (__cdecl *INITIALIZE)(OVR_INITPARAMS* Parameters);
typedef OVR_MATRIX (__cdecl *MATRIX_PROJECTION)(FOV Fov, FLOAT Near, FLOAT Far, UINT Flags);
typedef INT (__cdecl *RECENTER_TRACKING_ORIGIN)(OVR_SESSION* Session);
typedef BOOL (__cdecl *SHUTDOWN)();
typedef INT (__cdecl *SUBMIT_FRAME)(OVR_SESSION* Session, UINT64 Frame, VOID* ViewScale, const LAYER** Layers, UINT LayerCount);

}}