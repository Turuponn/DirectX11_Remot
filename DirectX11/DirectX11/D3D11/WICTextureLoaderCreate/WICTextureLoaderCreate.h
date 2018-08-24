#pragma once


/*
* @Note WIC���������e�N�X�`�����[�_�[�����
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
	* @brief �e�N�X�`�����[�h
	* @param filepath �e�N�X�`���܂ł̃t�@�C���p�X
	* @param [out] texbuffer �e�N�X�`���p�o�b�t�@��Ԃ�
	* @param [out] texsrvview �e�N�X�`���p�r���[��Ԃ�
	*/
	void LoadTexture(ID3D11Device*& device, ID3D11DeviceContext*& comand, const std::wstring& filepath, ID3D11Resource** texbuffer, ID3D11ShaderResourceView** texsrvview);
private:
};
