#pragma once

#include <tchar.h>
struct ID3D11Device;
struct ID3D11PixelShader;

class PixelShada {
public:
	PixelShada();
	virtual ~PixelShada();

	/**
	@brief �s�N�Z���V�F�[�_�쐬
	@param filename�@�V�F�[�_�t�@�C�����܂ł̃p�X
	@param funcname  �֐���
	@param[out] pixelbuffer  �s�N�Z���V�F�[�_��Ԃ��܂�
	*/
	void CreatePixelShada(const TCHAR* filenamepath, const char* funcname, ID3D11Device*& device, ID3D11PixelShader** pixelbuffer);

private:
};
