
#include <fbxsdk.h>
#include <DirectXMath.h>
#include <vector>
#include <d3d11.h>
#include <memory>
#include <map>

// ��̒��_�����i�[����\����
struct VERTEX {
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
};
//�}�e���A��
struct FBX_MATERIAL
{
	CHAR matname[110];
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
	CHAR textureName[110];
	DWORD dwNumFace;//���̃}�e���A���ł���|���S����
};

class D3D11DeviceAndSwapChainAndContextManager;
class WICTextureLoderManager;
class TextureSamplerManager;

class FBXLoader {
private:

	void LoadTexture(const std::string& filepath, std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);

private:

	void CreateVertexBuffer(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
	void NormalData(FbxMesh*& meshdata, int faceidx, int idx0, int idx1, int idx2, std::vector<VERTEX>& vertexdata);
	void VertexData(FbxMesh*& meshdata, int faceidx, int idx0, int idx1, int idx2, std::vector<VERTEX>& vertexdata);
	void UVData(FbxMesh*& meshdata, int faceidx, int idx0, int idx1, int idx2, std::vector<VERTEX>& vertexdata);
	void MaterialData(FbxMesh*& meshdata, std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
	const std::string TexturePath(const std::string& filepath);



	void VertexDataImport(FbxMesh*& meshdata, std::vector<VERTEX>& vertexdata);
	void CreateIndexBuffer(FbxMesh*& meshdata, std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, int matIdx);
	void CreateVertexBuffer(FbxMesh*& meshdata, std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, std::vector<VERTEX>& vertexdata);
private:
	// 
	//! �|���S���̎O�p�`���B 
	// 
	void TriangulateRecursive(FbxNode* _pNode, FbxScene* _pScene);

	// 
	//! FBX �I�u�W�F�N�g�� �������B 
	// 
	bool InitializeSdkObjects();

public:
	FBXLoader();
	virtual ~FBXLoader();


	bool Load(const std::string& _pName, std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
	// ����B 
	void Releae();



	void Render(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, std::shared_ptr<TextureSamplerManager>& texsmpler);

private:
	// FBX SDK �I�u�W�F�N�g�B 
	FbxManager*		m_pFbxManager;
	FbxScene*		m_pFbxScene;
	FbxImporter*		m_pFbxImporter;
	std::vector<VERTEX> _vertexdata;
	std::vector<FBX_MATERIAL> _material;
	std::vector<std::shared_ptr<WICTextureLoderManager>> _textures;
	

	std::string _fbxfilepath;


	std::vector<ID3D11Buffer*> _vertexbuffers;
	std::vector<ID3D11Buffer*> _indexbuffs;

	int _vertexidx;//�C���f�b�N�X��

	int _rigidbodyMeshnum;//1���f���@���̐�
private:
};
