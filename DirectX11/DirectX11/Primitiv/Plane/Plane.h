#pragma once

#include <memory>
#include "Primitiv\PrimitivProperty.h"
#include <vector>

class VertexBufferManager;
class VertexShadaManager;
class PixelShadaManager;
class D3D11DeviceAndSwapChainAndContextManager;
class InputLayoutManager;
class ConstantManager;
class WICTextureLoderManager;
class TextureSamplerManager;

class Plane {
private:
	void CreateVertex(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
	void CreateShada(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
	void InitTexture(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
	void LoadTexture(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
public:
	Plane();
	virtual ~Plane();




	/**
	@brief 床作成
	*/
	void CreatePlane(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
	/**
	* @brief 床をレンダー
	*/
	void DrawModel(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, std::shared_ptr<ConstantManager>& cameraCManager);


	std::shared_ptr<TextureSamplerManager>& GetSmpler();

private:
	std::shared_ptr<VertexBufferManager> _vertexbuffermanager;
	//!shada
	std::shared_ptr<VertexShadaManager> _vsshada;
	std::shared_ptr<PixelShadaManager> _psshada;
	std::shared_ptr<InputLayoutManager> _inputlayoutManager;
	std::shared_ptr<WICTextureLoderManager> _textureManager;
	std::shared_ptr<TextureSamplerManager> _texSamplerManager;
	


	std::vector<PrimitivVertex> _primitiv_vertexList;//頂点リスト


};