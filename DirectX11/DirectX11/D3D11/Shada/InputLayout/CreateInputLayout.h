#pragma once

struct ID3D11Device;
struct D3D11_INPUT_ELEMENT_DESC;

struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;



struct ID3D11InputLayout;

class CreateInputLayout {
public:
	CreateInputLayout();
	virtual ~CreateInputLayout();

	/**
	@brief ���_�C���v�b�g���C�A�E�g�쐬
	@param inputlaout
	@param numelement inputlaout�̗v�f��
	@param vertex_compileshada  ���_�R���p�C���ς݂̃u���u�����܂�
	@param [out] vertex_inputlaout ���_�C���v�b�g���C�A�E�g��Ԃ��܂�
	*/
	void Create(ID3D11Device*& device, const D3D11_INPUT_ELEMENT_DESC* inputlaout, const unsigned int& numelement, ID3DBlob*& vertex_compileshada, ID3D11InputLayout** vertex_inputlaout);

private:

};
