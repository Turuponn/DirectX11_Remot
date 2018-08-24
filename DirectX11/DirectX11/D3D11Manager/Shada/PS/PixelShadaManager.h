#pragma once



#include <memory>
#include <tchar.h>

class PixelShada;
struct ID3D11PixelShader;
class D3D11DeviceAndSwapChainAndContextManager;


class PixelShadaManager {
public:
	PixelShadaManager();
	virtual ~PixelShadaManager();

	/**
	* @brief ピクセルシェーダ作成
	* @param filepath シェーダファイルまでのパス
	* @param funcname エントリポイント
	*/
	void CreatePSShada(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, const TCHAR* filepath, const char* funcname);

	/**
	* @brief ピクセルシェーダを返す
	*/
	ID3D11PixelShader*& GetPSShada();

private:
	ID3D11PixelShader* _pixelshada;
};
