
#include <fbxsdk.h>
#include <DirectXMath.h>
#include <vector>
#include <d3d11.h>
#include <memory>
#include <map>


//定数
struct FBXCBUFF {
	DirectX::XMFLOAT4 diffusecolor;
	DirectX::XMFLOAT4 ambientcolor;
	DirectX::XMFLOAT4 specularcolor;
};

struct VERTEX {
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
};
struct FBX_MATERIAL
{
	CHAR matname[110];
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
	CHAR textureName[110];
	int fecenum;
};

class D3D11DeviceAndSwapChainAndContextManager;
class WICTextureLoderManager;
class TextureSamplerManager;
class ConstantManager;
struct D3D11_MAPPED_SUBRESOURCE;

class FBXLoader {
private:


private:

	void LoadTexture(const std::string& filepath, std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);

private:

	void LoadVertexData(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
	void NormalData(FbxMesh*& meshdata, int faceidx, int idx0, int idx1, int idx2, std::vector<VERTEX>& vertexdata);
	void VertexData(FbxMesh*& meshdata, int faceidx, int idx0, int idx1, int idx2, std::vector<VERTEX>& vertexdata);
	void UVData(FbxMesh*& meshdata, int faceidx, int idx0, int idx1, int idx2, std::vector<VERTEX>& vertexdata);
	void MaterialData(FbxMesh*& meshdata, std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, int meshcount);
	const std::string TexturePath(const std::string& filepath);



	void VertexDataImport(FbxMesh*& meshdata, std::vector<VERTEX>& vertexdata);
	void CreateIndexBuffer(FbxMesh*& meshdata, std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, int matIdx, std::vector<FBX_MATERIAL>& material, int meshcount, std::vector<ID3D11Buffer*>& idxbuffers);
	void CreateVertexBuffer(FbxMesh*& meshdata, std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, std::vector<VERTEX>& vertexdata);
private:
	void TriangulateRecursive(FbxNode* _pNode, FbxScene* _pScene);
	bool InitializeSdkObjects();
public:
	FBXLoader();
	virtual ~FBXLoader();
	bool Load(const std::string& _pName, std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
	void Releae();
	void Render(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, std::shared_ptr<TextureSamplerManager>& texsmpler);

private:
	FbxManager*		m_pFbxManager;
	FbxScene*		m_pFbxScene;
	FbxImporter*		m_pFbxImporter;
	std::vector<VERTEX> _vertexdata;
	
	std::vector<std::shared_ptr<WICTextureLoderManager>> _textures;
	

	std::string _fbxfilepath;


	std::vector<ID3D11Buffer*> _vertexbuffers;
	std::vector<std::vector<ID3D11Buffer*>> _indexbuffers;
	std::vector<std::vector<FBX_MATERIAL>> _material;//マテリアルテーブル
	int _vertexidx;

	int _rigidbodyMeshnum;

	D3D11_MAPPED_SUBRESOURCE* _cbuffaddress;
	
	std::shared_ptr<ConstantManager> _constantbuffer;

private:
	bool _traiangleCoolF;
};
