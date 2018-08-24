#pragma once


/*
* @Note WICをつかったテクスチャローダーを作る
*/

#include <memory>
#include <string>


struct ID3D11Resource;
struct ID3D11ShaderResourceView;
struct ID3D11Device;
struct ID3D11DeviceContext;

class WICTextureLoaderCreate {
public:
	WICTextureLoaderCreate();
	virtual ~WICTextureLoaderCreate();



	/**
	* @brief テクスチャロード
	* @param filepath テクスチャまでのファイルパス
	* @param [out] texbuffer テクスチャ用バッファを返す
	* @param [out] texsrvview テクスチャ用ビューを返す
	*/
	void LoadTexture(ID3D11Device*& device, ID3D11DeviceContext*& comand, const std::wstring& filepath, ID3D11Resource** texbuffer, ID3D11ShaderResourceView** texsrvview);
private:
};
