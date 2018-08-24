#pragma once

struct ID3D11Device;
struct ID3D11SamplerState;


class TextureSampler {
public:
	TextureSampler();
	virtual ~TextureSampler();

	/**
	@brief �e�N�X�`���T���v���̏������ݒ�
	@param[out] sampler  �e�N�X�`���T���v���[��Ԃ��܂�
	*/
	void InitSampler(ID3D11Device*& device, ID3D11SamplerState** sampler);


private:
};