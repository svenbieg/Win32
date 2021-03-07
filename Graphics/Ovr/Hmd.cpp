//=========
// Hmd.cpp
//=========

#include "pch.h"


//=======
// Using
//=======

#include "Desktop/Application.h"
#include "Runtime/Application.h"
#include "Hmd.h"

using namespace DirectX;
using namespace Runtime;
using namespace Windows::Foundation;


//===========
// Namespace
//===========

namespace Graphics {
	namespace Ovr {


//==========
// Settings
//==========

const VECTOR vFwd={ 0.f, 0.f, -1.f, 0.f };
const VECTOR vUp={ 0.f, 1.f, 0.f, 0.f };


//==================
// Con-/Destructors
//==================

Hmd::Hmd():
Tracking(true),
hModule(NULL),
pSession(nullptr)
{
ZeroMemory(&cDescription, sizeof(HMD_DESC));
ZeroMemory(&uId, sizeof(LUID));
#ifdef _X86_
hModule=LoadLibraryA("LibOVRRT32_1.dll");
#else
hModule=LoadLibraryA("LibOVRRT64_1.dll");
#endif
if(hModule==INVALID_HANDLE_VALUE)
	hModule=NULL;
ASSERT(hModule);
OVR_INITPARAMS initparams;
ZeroMemory(&initparams, sizeof(OVR_INITPARAMS));
initparams.uFlags=OVR_INITFLAG_REQUESTVERSION;
initparams.uMinorVersion=10;
auto pinit=(INITIALIZE)GetProcAddress(hModule, "ovr_Initialize");
ThrowIfFailed(pinit(&initparams));
auto pcreate=(CREATE_HMD)GetProcAddress(hModule, "ovr_Create");
ThrowIfFailed(pcreate(&pSession, &uId));
auto pgetdesc=(GET_HMD_DESC)GetProcAddress(hModule, "ovr_GetHmdDesc");
cDescription=pgetdesc(pSession);
hDeviceContext=new DeviceContext(uId);
Application::Current->Destroyed.Add(this, &Hmd::OnApplicationDestroyed);
CreateTextures();
CreateRenderContext();
}

Hmd::~Hmd()
{
StopRendering();
DoShutdown();
Scene=nullptr;
pSwapTextures[0]=nullptr;
pSwapTextures[1]=nullptr;
hDeviceContext=nullptr;
Application::Current->Destroyed.Remove(this);
}


//========
// Common
//========

VOID Hmd::Recenter()
{
auto precenter=(RECENTER_TRACKING_ORIGIN)GetProcAddress(hModule, "ovr_RecenterTrackingOrigin");
ThrowIfFailed(precenter(pSession));
}

VOID Hmd::StartRendering()
{
if(hRenderTask)
	return;
Handle<Hmd> hhmd=this;
hRenderTask=new Task([this](){ DoRender(); });
}

VOID Hmd::StopRendering()
{
if(hRenderTask)
	{
	hRenderTask->Abort();
	hRenderTask=nullptr;
	}
}


//================
// Common Private
//================

VOID Hmd::CreateRenderContext()
{
pGetEyePoses=(GET_EYE_POSES)GetProcAddress(hModule, "ovr_GetEyePoses");
pMatrixProjection=(MATRIX_PROJECTION)GetProcAddress(hModule, "ovrMatrix4f_Projection");
pSubmitFrame=(SUBMIT_FRAME)GetProcAddress(hModule, "ovr_SubmitFrame");
auto pgetrd=(GET_RENDER_DESC)GetProcAddress(hModule, "ovr_GetRenderDesc");
for(UINT ueye=0; ueye<2; ueye++)
	{
	pEyeRenderDescs[ueye]=pgetrd(pSession, ueye, cDescription.pDefaultEyeFov[ueye]);
	pHmdToEyes[ueye]=pEyeRenderDescs[ueye].vHmdToEyeViewOffset;
	OVR_MATRIX mxovr=pMatrixProjection(pEyeRenderDescs[ueye].cFov, 0.1f, 100.f, 0);
	pProjectionMatrices[ueye]={
		mxovr.M[0][0], mxovr.M[1][0], mxovr.M[2][0], mxovr.M[3][0],
		mxovr.M[0][1], mxovr.M[1][1], mxovr.M[2][1], mxovr.M[3][1],
		mxovr.M[0][2], mxovr.M[1][2], mxovr.M[2][2], mxovr.M[3][2],
		mxovr.M[0][3], mxovr.M[1][3], mxovr.M[2][3], mxovr.M[3][3]};
	}
}

VOID Hmd::CreateTextures()
{
auto pgetsize=(GET_FOV_TEXTURE_SIZE)GetProcAddress(hModule, "ovr_GetFovTextureSize");
::SIZE csize=pgetsize(pSession, 0, cDescription.pDefaultEyeFov[0], 1.f);
auto pdev=hDeviceContext->GetD3DDevice();
pSwapTextures[0]=new SwapTexture(pdev, hModule, pSession, csize.cx, csize.cy);
csize=pgetsize(pSession, 1, cDescription.pDefaultEyeFov[1], 1.f);
pSwapTextures[1]=new SwapTexture(pdev, hModule, pSession, csize.cx, csize.cy);
}

VOID Hmd::DoRender()
{
UINT64 uframe=0;
while(hRenderTask->GetStatus()==TaskStatus::Running)
	{
	DWORD dwstarttime=GetTickCount();
	hDeviceContext->Lock();
	try
		{
		POSE peyeposes[2];
		DOUBLE dsampletime=0;
		pGetEyePoses(pSession, uframe, true, pHmdToEyes, peyeposes, &dsampletime);
		for(UINT ueye=0; ueye<2; ueye++)
			{
			BeforeRendered(this, ueye);
			auto prtv=pSwapTextures[ueye]->GetRenderTargetView();
			auto pdsv=pSwapTextures[ueye]->GetDepthStencilView();
			hDeviceContext->SetTarget(prtv, pdsv, pSwapTextures[ueye]->GetViewport());
			hDeviceContext->Clear({ 0.f, 0.f, 0.f, 0.f });
			if(Scene&&Scene->Camera)
				{
				hDeviceContext->SetProjectionMatrix(XMMatrixTranspose(XMLoadFloat4x4(&pProjectionMatrices[ueye])));
				auto hcam=Scene->Camera;
				VECTOR vcampos=XMLoadFloat3(&hcam->GetPosition());
				VECTOR vcamrot=XMLoadFloat4(&hcam->GetRotation());
				VECTOR veyepos=XMLoadFloat3(Tracking? &peyeposes[ueye].vPosition: &pEyeRenderDescs[ueye].vHmdToEyeViewOffset);
				VECTOR vpos=XMVectorAdd(vcampos, XMVector3Rotate(veyepos, vcamrot));
				VECTOR veyerot=XMLoadFloat4(&peyeposes[ueye].vRotation);
				VECTOR vrot=Tracking? XMQuaternionMultiply(vcamrot, veyerot): vcamrot;
				VECTOR vfwd=XMVector3Rotate(vFwd, vrot);
				hDeviceContext->SetViewMatrix(XMMatrixTranspose(XMMatrixLookAtRH(vpos, XMVectorAdd(vpos, vfwd), XMVector3Rotate(vUp, vrot))));
				Scene->Draw(hDeviceContext);
				}
			pSwapTextures[ueye]->Commit();
			}
		LAYER l;
		ZeroMemory(&l, sizeof(LAYER));
		l.uType=1;
		for(UINT u=0; u<2; u++)
			{
			l.ppTextureSwapChains[u]=pSwapTextures[u]->GetSwapChain();
			CopyMemory(&l.pViewports[u], &pSwapTextures[u]->GetRect(), sizeof(RECT));
			}
		CopyMemory(l.pFovs, cDescription.pDefaultEyeFov, 2*sizeof(FOV));
		CopyMemory(l.pEyePoses, peyeposes, 2*sizeof(POSE));
		l.dSensorSampleTime=dsampletime;
		const LAYER* player=&l;
		pSubmitFrame(pSession, uframe, nullptr, &player, 1);
		uframe++;
		}
	catch(HRESULT hr)
		{
		hDeviceContext->Unlock();
		throw hr;
		}
	hDeviceContext->Unlock();
	}
}

VOID Hmd::DoShutdown()
{
if(!pSession)
	return;
auto pdestroy=(DESTROY_HMD)GetProcAddress(hModule, "ovr_Destroy");
pdestroy(pSession);
pSession=nullptr;
auto pshutdown=(SHUTDOWN)GetProcAddress(hModule, "ovr_Shutdown");
pshutdown();
}

VOID Hmd::OnApplicationDestroyed()
{
DoShutdown();
}

}}
