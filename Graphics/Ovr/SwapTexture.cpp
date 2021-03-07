//=================
// SwapTexture.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "SwapTexture.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Ovr {


//==================
// Con-/Destructors
//==================

SwapTexture::SwapTexture(D3D_DEVICE* pdev, HMODULE hmodule, OVR_SESSION* psession, UINT uwidth, UINT uheight):
hModule(hmodule),
pSession(psession),
pSwapChain(nullptr),
uCurrent(0)
{
ZeroMemory(&cViewport, sizeof(D3D_VIEWPORT));
cViewport.Height=(FLOAT)uheight;
cViewport.Width=(FLOAT)uwidth;
cViewport.MaxDepth=1.f;
ZeroMemory(&rcViewport, sizeof(RECT));
rcViewport.right=uwidth;
rcViewport.bottom=uheight;
auto pcreate=(CREATE_TEXTURE_SWAP_CHAIN)GetProcAddress(hModule, "ovr_CreateTextureSwapChainDX");
OVR_TEXTURE_DESC desc;
ZeroMemory(&desc, sizeof(OVR_TEXTURE_DESC));
desc.uFormat=OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
desc.iArraySize=1;
desc.iWidth=uwidth;
desc.iHeight=uheight;
desc.iMipLevels=1;
desc.iSampleCount=1;
desc.uMiscFlags=1;
desc.uBindFlags=1;
ThrowIfFailed(pcreate(pSession, pdev, &desc, &pSwapChain));
auto pgetcount=(GET_TEXTURE_SWAP_CHAIN_LENGTH)GetProcAddress(hModule, "ovr_GetTextureSwapChainLength");
UINT ucount=0;
ThrowIfFailed(pgetcount(pSession, pSwapChain, &ucount));
hDepthStencils=new TextureVector(ucount);
hDepthStencilViews=new DepthStencilViewVector(ucount);
hRenderTargetViews=new RenderTargetViewVector(ucount);
for(UINT u=0; u<ucount; u++)
	{
	auto pgetbuf=(GET_TEXTURE_SWAP_CHAIN_BUFFER)GetProcAddress(hModule, "ovr_GetTextureSwapChainBufferDX");
	ComPtr<ID3D11Texture2D> ptexture;
	ThrowIfFailed(pgetbuf(pSession, pSwapChain, u, IID_ID3D11Texture2D, (VOID**)ptexture.GetAddressOf()));
	D3D_RENDER_TARGET_VIEW_DESC rtvd;
	ZeroMemory(&rtvd, sizeof(D3D_RENDER_TARGET_VIEW_DESC));
	rtvd.Format=DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvd.ViewDimension=D3D11_RTV_DIMENSION_TEXTURE2D;
	ID3D11RenderTargetView* ptv=nullptr;
	ThrowIfFailed(pdev->CreateRenderTargetView(ptexture.Get(), &rtvd, &ptv));
	hRenderTargetViews->SetAt(u, ptv);
	DXGI_FORMAT format=DXGI_FORMAT_D32_FLOAT;
	D3D_TEXTURE_DESC td;
	ZeroMemory(&td, sizeof(D3D_TEXTURE_DESC));
	td.Width=uwidth;
	td.Height=uheight;
	td.MipLevels=1;
	td.ArraySize=1;
	td.Format=DXGI_FORMAT_D32_FLOAT;
	td.SampleDesc.Count=1;
	td.BindFlags=D3D11_BIND_DEPTH_STENCIL;
	D3D_TEXTURE* pds=nullptr;
	ThrowIfFailed(pdev->CreateTexture2D(&td, 0, &pds));
	hDepthStencils->SetAt(u, pds);
	D3D_DEPTH_STENCIL_VIEW* pdsv=nullptr;
	ThrowIfFailed(pdev->CreateDepthStencilView(pds, 0, &pdsv));
	hDepthStencilViews->SetAt(u, pdsv);
	}
}

SwapTexture::~SwapTexture()
{
auto pdestroy=(DESTROY_TEXTURE_SWAP_CHAIN)GetProcAddress(hModule, "ovr_DestroyTextureSwapChain");
pdestroy(pSession, pSwapChain);
}


//========
// Common
//========

VOID SwapTexture::Commit()
{
auto pcommit=(COMMIT_TEXTURE_SWAP_CHAIN)GetProcAddress(hModule, "ovr_CommitTextureSwapChain");
ThrowIfFailed(pcommit(pSession, pSwapChain));
auto pget=(GET_TEXTURE_SWAP_CHAIN_CURRENT_INDEX)GetProcAddress(hModule, "ovr_GetTextureSwapChainCurrentIndex");
ThrowIfFailed(pget(pSession, pSwapChain, &uCurrent));
}

}}
