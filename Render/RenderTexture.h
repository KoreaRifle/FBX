#pragma once

class RenderTexture
{
public:
	RenderTexture(UINT width = 0, UINT height = 0);
	~RenderTexture();

	ID3D11ShaderResourceView* GetResourceView()
	{
		return resourceView;
	}

	void SetTarget();
	void Clear(D3DXCOLOR& color = D3DXCOLOR(1, 1, 1, 1));

	void SaveTexture(wstring outputFile);

private:
	UINT width, height;

	ID3D11Texture2D* texture;
	ID3D11RenderTargetView* renderView;
	ID3D11ShaderResourceView* resourceView;
};