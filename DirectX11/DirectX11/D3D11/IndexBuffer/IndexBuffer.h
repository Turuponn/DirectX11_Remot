#pragma once



/************************************************************
* File Name	: IndexBuffer.h
* Description	: �C���f�b�N�X�o�b�t�@�쐬�N���X
* Date		:	2018-7-12.
* Author	:	�ߍ�
****************************************************/

struct ID3D11Device;
struct ID3D11Buffer;

class IndexBuffer {
public:

	IndexBuffer();
	virtual ~IndexBuffer();

	/// <summary>
	/// �C���f�b�N�X�o�b�t�@���쐬����
	/// </summary>
	/// <param name="device"></param> 
	/// <param name="buffsize"> 1�C���f�b�N�X�ɑ΂���o�b�t�@�̃T�C�Y </param>
	/// <param name="fecedata"> 1�C���f�b�N�X�ɑ΂���f�[�^�T�C�Y </param>
	/// <param name="indexbuffer"> [out] �C���f�b�N�X�o�b�t�@��Ԃ�</param>
	void CreateIndexBuffer(ID3D11Device*& device, const unsigned int buffsize, void* fecedata, ID3D11Buffer** indexbuffer);


private:

};