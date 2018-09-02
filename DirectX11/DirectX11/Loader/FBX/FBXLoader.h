
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

// 一つの頂点情報を格納する構造体
struct VERTEX {
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
	int boneIndex[4];//ボーン　番号 TODO:おかしいのはここ影響してるかも
	float boneWeight[4];//ボーン　重み
};
//マテリアル
struct FBX_MATERIAL
{
	CHAR matname[110];
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
	CHAR textureName[110];
	int fecenum;//マテリアルポリゴン数(マテリアルアタッチされたメッシュ範囲内のインデックス数？)
};


//ボーン構造体
struct BONE
{
	DirectX::XMMATRIX mBindPose;//初期ポーズ（ずっと変わらない）
	DirectX::XMMATRIX mNewPose;//現在のポーズ（その都度変わる）

};
//１頂点の共有　最大20ポリゴンまで
struct POLY_TABLE
{
	int PolyIndex[20];//ポリゴン番号 
	int Index123[20];//3つの頂点のうち、何番目か
	int NumRef;//属しているポリゴン数

};

class D3D11DeviceAndSwapChainAndContextManager;
class WICTextureLoderManager;
class TextureSamplerManager;
class ConstantManager;
struct D3D11_MAPPED_SUBRESOURCE;

class FBXLoader {
private:
	//スキニング用
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
	//! ポリゴンの三角形化。 
	// 
	void TriangulateRecursive(FbxNode* _pNode, FbxScene* _pScene);

	// 
	//! FBX オブジェクトの 初期化。 
	// 
	bool InitializeSdkObjects();

public:
	FBXLoader();
	virtual ~FBXLoader();


	bool Load(const std::string& _pName, std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
	// 解放。 
	void Releae();



	void Render(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, std::shared_ptr<TextureSamplerManager>& texsmpler);

private:
	// FBX SDK オブジェクト。 
	FbxManager*		m_pFbxManager;
	FbxScene*		m_pFbxScene;
	FbxImporter*		m_pFbxImporter;
	std::vector<VERTEX> _vertexdata;
	
	std::vector<std::shared_ptr<WICTextureLoderManager>> _textures;
	

	std::string _fbxfilepath;


	std::vector<ID3D11Buffer*> _vertexbuffers;
	std::vector<std::vector<ID3D11Buffer*>> _indexbuffers;//メッシュ数に応じたマテリアル用頂点インデックステーブル
	std::vector<std::vector<FBX_MATERIAL>> _material;//マテリアルテーブル
	int _vertexidx;//インデックス数

	int _rigidbodyMeshnum;//1モデル　剛体数


	//ボーン
	int m_iNumBone;
	BONE* m_BoneArray;
	FbxCluster** m_ppCluster;


	D3D11_MAPPED_SUBRESOURCE* _cbuffaddress;
	
	std::shared_ptr<ConstantManager> _constantbuffer;

private:
	bool _traiangleCoolF;//三角形化警告フラグ
};
