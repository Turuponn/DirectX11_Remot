#pragma once

struct ID3D11Device;
struct ID3D11SamplerState;


class TextureSampler {
public:
	TextureSampler();
	virtual ~TextureSampler();

	/**
	@brief テクスチャサンプラの初期化設定
	@param[out] sampler  テクスチャサンプラーを返します
	*/
	void InitSampler(ID3D11Device*& device, ID3D11SamplerState** sampler);


private:
};