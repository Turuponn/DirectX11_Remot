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
	* @brief �s�N�Z���V�F�[�_�쐬
	* @param filepath �V�F�[�_�t�@�C���܂ł̃p�X
	* @param funcname �G���g���|�C���g
	*/
	void CreatePSShada(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, const TCHAR* filepath, const char* funcname);

	/**
	* @brief �s�N�Z���V�F�[�_��Ԃ�
	*/
	ID3D11PixelShader*& GetPSShada();

private:
	ID3D11PixelShader* _pixelshada;
};
