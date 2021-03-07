//===============
// SwapTexture.h
//===============

#pragma once


//=======
// Using
//=======

#include <wrl.h>
#include "Collections\Vector.h"
#include "LibOvr.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Ovr {


//==============
// Swap-Texture
//==============

class SwapTexture: public Object
{
private:
	// Using
	using TextureVector=Collections::Vector<Microsoft::WRL::ComPtr<D3D_TEXTURE>>;
	using DepthStencilViewVector=Collections::Vector<Microsoft::WRL::ComPtr<D3D_DEPTH_STENCIL_VIEW>>;
	using RenderTargetViewVector=Collections::Vector<Microsoft::WRL::ComPtr<D3D_RENDER_TARGET_VIEW>>;

public:
	// Con-/Destructors
	SwapTexture(D3D_DEVICE* Device, HMODULE Module, OVR_SESSION* Session, UINT Width, UINT Height);
	~SwapTexture();

	// Common
	VOID Commit();
	UINT GetCurrent()const { return uCurrent; }
	D3D_DEPTH_STENCIL_VIEW* GetDepthStencilView()const { return hDepthStencilViews->GetAt(uCurrent).Get(); }
	RECT const& GetRect() { return rcViewport; }
	D3D_RENDER_TARGET_VIEW* GetRenderTargetView()const { return hRenderTargetViews->GetAt(uCurrent).Get(); }
	OVR_TEXTURE_SWAP_CHAIN* GetSwapChain()const { return pSwapChain; }
	D3D_VIEWPORT* GetViewport() { return &cViewport; }

private:
	// Common
	Handle<TextureVector> hDepthStencils;
	Handle<DepthStencilViewVector> hDepthStencilViews;
	Handle<RenderTargetViewVector> hRenderTargetViews;
	D3D_VIEWPORT cViewport;
	HMODULE hModule;
	OVR_SESSION* pSession;
	OVR_TEXTURE_SWAP_CHAIN* pSwapChain;
	RECT rcViewport;
	UINT uCurrent;
};

}}
