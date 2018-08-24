#pragma once

#include <tchar.h>
struct ID3D11Device;
struct ID3D11PixelShader;

class PixelShada {
public:
	PixelShada();
	virtual ~PixelShada();

	/**
	@brief ピクセルシェーダ作成
	@param filename　シェーダファイル名までのパス
	@param funcname  関数名
	@param[out] pixelbuffer  ピクセルシェーダを返します
	*/
	void CreatePixelShada(const TCHAR* filenamepath, const char* funcname, ID3D11Device*& device, ID3D11PixelShader** pixelbuffer);

private:
};
