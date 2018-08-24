#pragma once

#include <memory>
#include <string>

class WICTextureLoaderCreate;
struct ID3D11Resource;
struct ID3D11ShaderResourceView;
class D3D11DeviceAndSwapChainAndContextManager;


class WICTextureLoderManager {
public:
	WICTextureLoderManager();
	virtual ~WICTextureLoderManager();

	/**
	* @brief テクスチャロード
	* @param filepath ファイルまでのパス
	*/
	void LoadTexture(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, const std::wstring& filepath);
	/**
	* @return テクスチャ用のSRVビューを返す
	*/
	ID3D11ShaderResourceView*& GetTexView();
	
	ID3D11Resource*& GetTexbuff();

private:
	ID3D11Resource* _texbuff;
	ID3D11ShaderResourceView* _texSrvView;
	
};

