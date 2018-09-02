
#include <fbxsdk.h>
#include <DirectXMath.h>
#include <vector>
#include <d3d11.h>
#include <memory>
#include <map>


//�萔
struct FBXCBUFF {
	DirectX::XMFLOAT4 diffusecolor;
	DirectX::XMFLOAT4 ambientcolor;
	DirectX::XMFLOAT4 specularcolor;
};

// ��̒��_�����i�[����\����
struct VERTEX {
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
	int boneIndex[4];//�{�[���@�ԍ� TODO:���������̂͂����e�����Ă邩��
	float boneWeight[4];//�{�[���@�d��
};
//�}�e���A��
struct FBX_MATERIAL
{
	CHAR matname[110];
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
	CHAR textureName[110];
	int fecenum;//�}�e���A���|���S����(�}�e���A���A�^�b�`���ꂽ���b�V���͈͓��̃C���f�b�N�X���H)
};


//�{�[���\����
struct BONE
{
	DirectX::XMMATRIX mBindPose;//�����|�[�Y�i�����ƕς��Ȃ��j
	DirectX::XMMATRIX mNewPose;//���݂̃|�[�Y�i���̓s�x�ς��j

};
//�P���_�̋��L�@�ő�20�|���S���܂�
struct POLY_TABLE
{
	int PolyIndex[20];//�|���S���ԍ� 
	int Index123[20];//3�̒��_�̂����A���Ԗڂ�
	int NumRef;//�����Ă���|���S����

};

class D3D11DeviceAndSwapChainAndContextManager;
class WICTextureLoderManager;
class TextureSamplerManager;
class ConstantManager;
struct D3D11_MAPPED_SUBRESOURCE;

class FBXLoader {
private:
	//�X�L�j���O�p
	void LoadSkinAnim(FbxNode* meshNode);
	void InitSkinAnim();
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
	
	std::vector<std::shared_ptr<WICTextureLoderManager>> _textures;
	

	std::string _fbxfilepath;


	std::vector<ID3D11Buffer*> _vertexbuffers;
	std::vector<std::vector<ID3D11Buffer*>> _indexbuffers;//���b�V�����ɉ������}�e���A���p���_�C���f�b�N�X�e�[�u��
	std::vector<std::vector<FBX_MATERIAL>> _material;//�}�e���A���e�[�u��
	int _vertexidx;//�C���f�b�N�X��

	int _rigidbodyMeshnum;//1���f���@���̐�


	//�{�[��
	int m_iNumBone;
	BONE* m_BoneArray;
	FbxCluster** m_ppCluster;


	D3D11_MAPPED_SUBRESOURCE* _cbuffaddress;
	
	std::shared_ptr<ConstantManager> _constantbuffer;

private:
	bool _traiangleCoolF;//�O�p�`���x���t���O
};
