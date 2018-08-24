#pragma once

#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>

#include <memory>

class D3D11DeviceAndSwapChainAndContextManager;
class WICTextureLoderManager;
class TextureSamplerManager;

struct OBJ_MESH
{
	int vertexnum;//���_��
	int faceindexnum;//���_�C���f�b�N�X��
	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer** ppIndexBuffer;
};


struct OBJ_MATERIAL{
	char szName[110];
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
	DWORD dwNumFace;//���̃}�e���A���ł���|���S����
	char szTextureName[110];
	ID3D11ShaderResourceView* pTexture;
};


struct OBJ_VERTEX
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
};

struct OBJ_INDEX {
	int fecenum;//1�}�e���A�����Ƃ̃C���f�b�N�X��
	int* indexdata;
};

class ObjLoader {
private:
	HRESULT LoadMaterialFromFile(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, OBJ_MATERIAL** ppMaterial);
public:
	ObjLoader();
	virtual ~ObjLoader();

	HRESULT InitStaticMesh(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, const char* FileName, OBJ_MESH** pMesh);
	
	void Render(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, OBJ_MESH* pMesh);
private:
	DWORD m_dwNumMaterial;
	OBJ_MATERIAL* m_pMaterial;
	std::shared_ptr<WICTextureLoderManager> _wictex;
	std::shared_ptr<TextureSamplerManager> _texsmpler;
};
