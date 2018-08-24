#pragma once



#include <string>
#include <memory>
#include "Mesh.h"
#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>
#include <assimp/Importer.hpp> // C ++インポータインタフェース
#include <assimp/scene.h> //出力データ構造体
#include <assimp/postprocess.h > //後処理フラグ 

//struct VERTEX {
//	float X, Y, Z;
//	DirectX::XMFLOAT2 texcoord;
//};
//
//struct Texture {
//	std::string type;
//	std::string path;
//	ID3D11ShaderResourceView *texture;
//};

class D3D11DeviceAndSwapChainAndContextManager;

class AssimpManager {
private:
	bool import(const std::string& PFILE, std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
	void ProcessNode(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, aiNode * node, const aiScene * scene);
	Mesh ProcessMesh(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, aiMaterial* mat, aiTextureType type, string typeName, const aiScene* scene);
	std::string determineTextureType(const aiScene * scene, aiMaterial * mat);
	ID3D11ShaderResourceView* getTextureFromModel(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, const aiScene * scene, int textureindex);
	int getTextureIndex(aiString * str);
	void SetDir(const std::string& filepath);//パスを入れる
	void SetupMesh(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
public:
	AssimpManager();
	virtual ~AssimpManager();


	void Close();
	void Draw(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
	void LoadFile(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, const std::string& filepath);

private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded;
	std::string textype;


	ID3D11Buffer* _vertexBuffer;
	ID3D11Buffer* _indexBuffer;
};
