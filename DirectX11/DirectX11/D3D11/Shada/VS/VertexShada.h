#pragma once

#include <tchar.h>

struct ID3D11Device;
struct ID3D11VertexShader;

struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;


class VertexShada {
public:
	VertexShada();
	virtual ~VertexShada();

	/**
	@brief ���_�V�F�[�_�쐬
	@param filename�@�V�F�[�_�t�@�C�����܂ł̃p�X
	@param funcname  �֐���
	@param[out] vertexbuffer  ���_�V�F�[�_��Ԃ��܂�
	@param [out] compileshada �R���p�C���V�F�[�_��Ԃ��܂�
	*/
	void CreateVertexShada(const TCHAR* filenamepath, const char* funcname, ID3D11Device*& device, ID3D11VertexShader** vertexbuffer, ID3DBlob** compileshada);

private:
};
