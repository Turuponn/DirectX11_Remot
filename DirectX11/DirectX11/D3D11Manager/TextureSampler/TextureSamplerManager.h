#pragma once


#include <memory>

class TextureSampler;
struct ID3D11SamplerState;
class D3D11DeviceAndSwapChainAndContextManager;

class TextureSamplerManager {
private:
	
	
public:
	TextureSamplerManager();
	virtual ~TextureSamplerManager();


	/**
	* @brief �e�N�X�`�����T���v�����O����T���v�����쐬����
	*/
	void CreateTextureSampler(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);

	/**
	* @return �T���v����Ԃ�
	*/
	ID3D11SamplerState*& GetTextureSampler();


private:
	ID3D11SamplerState* _texsampler;
};
