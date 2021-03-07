//=======
// Hmd.h
//=======

#pragma once


//=======
// Using
//=======

#include "Graphics/DirectX/DeviceContext.h"
#include "Graphics/Scene.h"
#include "LibOvr.h"
#include "SwapTexture.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Ovr {


//=====
// Hmd
//=====

class Hmd: public Object
{
private:
	// Using
	using DeviceContext=Graphics::DirectX::DeviceContext;

public:
	// Friends
	friend SwapTexture;

	// Con-/Destructors
	Hmd();
	~Hmd();

	// Common
	Event<Hmd, UINT> BeforeRendered;
	HMD_DESC const& GetDescription()const { return cDescription; }
	D3D_DEVICE* GetD3DDevice() { hDeviceContext->GetD3DDevice(); }
	Handle<DeviceContext> GetDeviceContext() { return hDeviceContext; }
	LUID const& GetId() { return uId; }
	VOID Recenter();
	Handle<Graphics::Scene> Scene;
	OVR_SESSION* GetSession()const { return pSession; }
	VOID StartRendering();
	VOID StopRendering();
	BOOL Tracking;

private:
	// Common
	VOID CreateRenderContext();
	VOID CreateTextures();
	VOID DoRender();
	VOID DoShutdown();
	VOID OnApplicationDestroyed();
	HMD_DESC cDescription;
	Handle<DeviceContext> hDeviceContext;
	HMODULE hModule;
	Handle<Task> hRenderTask;
	EYE_RENDER_DESC pEyeRenderDescs[2];
	GET_EYE_POSES pGetEyePoses;
	FLOAT3 pHmdToEyes[2];
	MATRIX_PROJECTION pMatrixProjection;
	FLOAT4X4 pProjectionMatrices[2];
	OVR_SESSION* pSession;
	SUBMIT_FRAME pSubmitFrame;
	Handle<SwapTexture> pSwapTextures[2];
	LUID uId;
};

}}
