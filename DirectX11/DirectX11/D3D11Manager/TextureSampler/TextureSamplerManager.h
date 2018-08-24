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
	* @brief テクスチャをサンプリングするサンプラを作成する
	*/
	void CreateTextureSampler(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);

	/**
	* @return サンプラを返す
	*/
	ID3D11SamplerState*& GetTextureSampler();


private:
	ID3D11SamplerState* _texsampler;
};
