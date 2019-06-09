#pragma once



struct ID3D11Device;
struct ID3D11Buffer;

class ConstantBufferCreate {
public:
	ConstantBufferCreate();
	virtual ~ConstantBufferCreate();

	/**
	@brief �萔�o�b�t�@�쐬
	@param constantsize�@�萔�T�C�Y
	@param [out] constantbuffer �萔�o�b�t�@��Ԃ��܂�
	*/
	void CreateConstantBuffer(ID3D11Device*& device, const size_t& constantsize, ID3D11Buffer** constantbuffer);


};
