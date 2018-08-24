#pragma once


#include <d3d11.h>
#include <memory>
#include <string>
#include <DirectXMath.h>



class D3D11DeviceAndSwapChainAndContextManager;
class VertexShadaManager;
class PixelShadaManager;
class InputLayoutManager;
class FBXLoader;
class TextureSamplerManager;


class FBXLoaderManager {
private:
	//�T���v���쐬
	void CreateTextureSampler(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
	void CreateShada(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
public:
	FBXLoaderManager();
	virtual ~FBXLoaderManager();

	
	void LoadModel(const std::string& filepath, std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
	
	
	void Render(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
private:
	std::shared_ptr<VertexShadaManager> _vertexshada;
	std::shared_ptr<PixelShadaManager> _pixelshada;
	std::shared_ptr<InputLayoutManager> _inputlaout;
	std::shared_ptr<FBXLoader> _fbx;
	std::shared_ptr<TextureSamplerManager> _texsmpler;//�A�h���b�V���O���[�h�͕s�ρH


	ID3D11InputLayout* _laout;
	
};
