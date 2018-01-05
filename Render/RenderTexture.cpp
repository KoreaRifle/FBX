#include "../stdafx.h"
#include "RenderTexture.h"

RenderTexture::RenderTexture(UINT width, UINT height)
{
	HRESULT hr;

	D3DInfo info;
	D3D::GetInfo(&info);

	if (width < 1 || width >= info.screenWidth)
		this->width = info.screenWidth;
	else
		this->width = width;

	if (height < 1 || height >= info.screenHeight)
		this->height = info.screenHeight;
	else
		this->height = height;


	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = this->width;
	textureDesc.Height = this->height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

	hr = D3D::GetDevice()->CreateTexture2D(&textureDesc, NULL, &texture);
	assert(SUCCEEDED(hr));


	D3D11_RENDER_TARGET_VIEW_DESC renderDesc;
	ZeroMemory(&renderDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	renderDesc.Format = textureDesc.Format;
	renderDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderDesc.Texture2D.MipSlice = 0;

	hr = D3D::GetDevice()->CreateRenderTargetView(texture, &renderDesc, &renderView);
	assert(SUCCEEDED(hr));


	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	viewDesc.Format = textureDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;

	hr = D3D::GetDevice()->CreateShaderResourceView(texture, &viewDesc, &resourceView);
	assert(SUCCEEDED(hr));
}

RenderTexture::~RenderTexture()
{
	SAFE_RELEASE(resourceView);
	SAFE_RELEASE(renderView);
	SAFE_RELEASE(texture);
}

void RenderTexture::SetTarget()
{
	ID3D11DeviceContext* dc = D3D::GetDeviceContext();
	
	dc->OMSetRenderTargets(1, &renderView, DepthStencil::Get()->GetDepthView());
}

void RenderTexture::Clear(D3DXCOLOR & color)
{
	ID3D11DeviceContext* dc = D3D::GetDeviceContext();

	dc->ClearRenderTargetView(renderView, color);
	dc->ClearDepthStencilView
	(
		DepthStencil::Get()->GetDepthView()
		, D3D11_CLEAR_DEPTH
		, 1, 0
	);
}

void RenderTexture::SaveTexture(wstring outputFile)
{
	HRESULT hr = D3DX11SaveTextureToFile
	(
		D3D::GetDeviceContext()
		, texture
		, D3DX11_IFF_PNG
		, outputFile.c_str()
	);
	assert(SUCCEEDED(hr));
}
