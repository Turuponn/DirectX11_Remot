#pragma once

#include <memory>
#include <vector>

class D3D11DeviceAndSwapChainAndContextManager;
struct OBJ_MESH;
struct OBJ_MATERIAL;
struct OBJ_VERTEX;
struct OBJ_INDEX;
struct ID3D11Buffer;
class WICTextureLoderManager;
class InputLayoutManager;
class VertexShadaManager;
class PixelShadaManager;
class TextureSamplerManager;
class IndexBufferManager;
class ObjLoader;


/*
データはここで保持
*/

class ObjLoaderManager {
private:
	void CreateShada(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
	void CreateVertex(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
	void CreateIndex(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
public:
	ObjLoaderManager();
	virtual ~ObjLoaderManager();


	void LoadModel(const char* filepath, std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
	void DrawModel(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, std::shared_ptr<TextureSamplerManager>& smpler);
private:

	int _materialnum;//マテリアル数
	OBJ_MESH* _mesh;

	OBJ_MATERIAL* _material;
	std::vector<OBJ_VERTEX> _vertexdata;
	std::vector<OBJ_INDEX> _indexdata;
	ID3D11Buffer* _vertexBuffer;
	std::shared_ptr<IndexBufferManager> _indexbuff;

	std::shared_ptr<WICTextureLoderManager> _wictexloder;
	std::shared_ptr<InputLayoutManager> _inputlayoutmanager;
	std::shared_ptr<VertexShadaManager> _vs;
	std::shared_ptr<PixelShadaManager> _ps;

	std::shared_ptr<ObjLoader> _objloader;

};
