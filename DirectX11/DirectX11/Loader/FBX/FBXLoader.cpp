#include "FBXLoader.h"
#include "D3D11Manager\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContextManager.h"
#include "D3D11Manager\Texture\WICTextureLoderManager.h"
#include "D3D11Manager\TextureSampler\TextureSamplerManager.h"
#include <tchar.h>
using namespace std;
using namespace DirectX;




FBXLoader::FBXLoader() {
	m_pFbxManager = NULL;
	m_pFbxScene = NULL;
	m_pFbxImporter = NULL;
	_rigidbodyMeshnum = 0;
	
	
	_vertexidx = 0;
}
FBXLoader::~FBXLoader() {
	Releae();
}


//	FBXデータ解放。 
void FBXLoader::Releae()
{
	if (m_pFbxImporter)
	{
		m_pFbxImporter->Destroy();
		m_pFbxImporter = NULL;
	}

	if (m_pFbxScene)
	{
		m_pFbxScene->Destroy();
		m_pFbxScene = NULL;
	}

	if (m_pFbxManager)
	{
		m_pFbxManager->Destroy();
		m_pFbxManager = NULL;
	}
} 
void FBXLoader::TriangulateRecursive(FbxNode* _pNode, FbxScene* _pScene)
{
	//	ポリゴンの三角形化。
	FbxNodeAttribute* pNodeAttribute = _pNode->GetNodeAttribute();

	if (pNodeAttribute)
	{
		FbxNodeAttribute::EType type = pNodeAttribute->GetAttributeType();
		if (type == FbxNodeAttribute::eMesh ||
			type == FbxNodeAttribute::eNurbs ||
			type == FbxNodeAttribute::eNurbsSurface ||
			type == FbxNodeAttribute::ePatch)
		{
			FbxGeometryConverter lConverter(_pNode->GetFbxManager());
			lConverter.Triangulate(_pScene, true);
		}
	}

	const int lChildCount = _pNode->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
		// 子ノードを探索。 
		TriangulateRecursive(_pNode->GetChild(lChildIndex), _pScene);
	}
}

//	FBX オブジェクトの 初期化。 
bool FBXLoader::InitializeSdkObjects()
{
	// マネージャ作成。 
	m_pFbxManager = FbxManager::Create();
	if (!m_pFbxManager)
	{
		FBXSDK_printf("Error: Unable to create FBX Manager!\n");
		return false;
	}
	else
	{
		FBXSDK_printf("Autodesk FBX SDK version %s\n", m_pFbxManager->GetVersion());
	}

	// IOSettings オブジェクト作成。 このオブジェクトは全てのインポート/エクスポートの設定を保持します。 
	FbxIOSettings* pIOS = FbxIOSettings::Create(m_pFbxManager, IOSROOT);
	m_pFbxManager->SetIOSettings(pIOS);

	// 実行可能なディレクトリ(オプション)からプラグインをロードします。 
	FbxString lPath = FbxGetApplicationDirectory();
	m_pFbxManager->LoadPluginsDirectory(lPath.Buffer());

	// シーン作成。 
	m_pFbxScene = FbxScene::Create(m_pFbxManager, "Scene");
	if (!m_pFbxScene)
	{
		FBXSDK_printf("Error: Unable to create FBX scene!\n");
		return false;
	}
	return true;
}

//	FBXファイルの読み込み 
bool FBXLoader::Load(const std::string& _pName, std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device)
{
	_fbxfilepath = _pName;
	printf("\n");
	int sFileFormat = -1;

	// FBX SDK オブジェクトの初期化。 
	InitializeSdkObjects();

	// インポータ作成。 
	m_pFbxImporter = FbxImporter::Create(m_pFbxManager, "Importer");
	if (!m_pFbxImporter)
	{
		FBXSDK_printf("Error: Unable to create FBX importer!\n");
		return false;
	}

	// ファイルを指定したインポート(リーダー)ファイル形式で検出します。 
	if (!m_pFbxManager->GetIOPluginRegistry()->DetectReaderFileFormat(_pName.c_str(), sFileFormat))
	{
		// 検出できないファイル形式です。 FbxImporter::eFBX_BINARY 形式でトライします。 
		sFileFormat = m_pFbxManager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX binary (*.fbx)");
	}

	// FBXファイルを読み込む。 
	if (!m_pFbxImporter->Initialize(_pName.c_str(), sFileFormat))
	{
		FBXSDK_printf("Error: Unable to create FBX initialize!\n");
		return false;
	}

	// FBX ファイルのバージョン取得。 
	int major, minor, revision;
	m_pFbxImporter->GetFileVersion(major, minor, revision);
	FBXSDK_printf("FBX VERSION %d %d %d\n", major, minor, revision);

	// 読み込んだFBXファイルからシーンデータを取り出す。 
	if (!m_pFbxImporter->Import(m_pFbxScene))
	{
		FBXSDK_printf("Error: Unable to create FBX import!\n");
		return false;
	}

	// 軸の設定。 
	FbxAxisSystem OurAxisSystem = FbxAxisSystem::DirectX;
	
	// DirectX系に変換。 
	FbxAxisSystem SceneAxisSystem = m_pFbxScene->GetGlobalSettings().GetAxisSystem();
	if (SceneAxisSystem != OurAxisSystem)
	{
		FbxAxisSystem::DirectX.ConvertScene(m_pFbxScene);
	}

	// 単位系の統一。 
	FbxSystemUnit SceneSystemUnit = m_pFbxScene->GetGlobalSettings().GetSystemUnit();
	if (SceneSystemUnit.GetScaleFactor() != 1.0f)
	{
		// センチメーター単位にコンバートする。 
		FbxSystemUnit::cm.ConvertScene(m_pFbxScene);
	}

	// 三角形化
	TriangulateRecursive(m_pFbxScene->GetRootNode(), m_pFbxScene);


	//頂点データ作成
	CreateVertexBuffer(device);

	return true;
}

void FBXLoader::CreateVertexBuffer(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {

	//Note: ASCIIになってないバイナリじゃないやつはおかしくなるかも
	//Note: ファイル内にカメラやライトがのこっているとそれもメッシュとしてとるかもしれない
	//サンプルの戦車はたぶん使えない　バージョンのせい？　古い？

	
	_rigidbodyMeshnum = m_pFbxScene->GetRootNode()->GetChildCount();
	printf("剛体数: %d  \n", _rigidbodyMeshnum);
	// 頂点データの取り出し
	for (int i = 0; i < m_pFbxScene->GetRootNode()->GetChildCount(); i++) {//子までノードを巡る : 剛体が複数ある場合その数分まわるっぽい
		if (m_pFbxScene->GetRootNode()->GetChild(i)->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh) {
			std::vector<VERTEX> v;
			auto _mesh = m_pFbxScene->GetRootNode()->GetChild(i)->GetMesh();

			
			
			VertexDataImport(_mesh, v);
			//マテリアル情報取り出し
			MaterialData(_mesh, device);
			//頂点バッファ作成
			CreateVertexBuffer(_mesh,device, v);



		}
	}
	
}
void FBXLoader::LoadTexture(const std::string& filepath, std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {
	std::shared_ptr<WICTextureLoderManager> texmanager(new WICTextureLoderManager());
	std::wstring path(filepath.begin(), filepath.end());
	texmanager->LoadTexture(device, path);
	_textures.push_back(texmanager);
}
const std::string FBXLoader::TexturePath(const std::string& filepath) {
	std::string workpath = filepath;
	auto lastpath = filepath.substr(0, filepath.find_last_of('\\') + 1);//最後に現れる指定文字までの場所
	workpath.erase(0, lastpath.size());//中身を編集

	//パス化する
	auto lastpathfbx = _fbxfilepath.substr(0, _fbxfilepath.find_last_of('/') + 1);
	if (lastpathfbx.size() < 1) {
		printf("fbx指定ファイルパスが無効\n");
		throw(1);
	}
	workpath = lastpathfbx + workpath;
	printf("LoadTexture: %s\n", workpath.c_str());
	return workpath;
}

void FBXLoader::MaterialData(FbxMesh*& meshdata,std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {
	//マテリアル読み込み
	FbxNode* pNode = meshdata->GetNode();
	int m_dwNumMaterial = pNode->GetMaterialCount();
	int facenum = meshdata->GetPolygonCount();
	_material.resize(m_dwNumMaterial);



	for (int i = 0; i < m_dwNumMaterial; i++)
	{
		//フォンモデルを想定
		FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);
		FbxSurfacePhong* pPhong = (FbxSurfacePhong*)pMaterial;

		//環境光
		_material[i].ambient.x = static_cast<float>(pPhong->Ambient.Get()[0]);
		_material[i].ambient.y = static_cast<float>(pPhong->Ambient.Get()[1]);
		_material[i].ambient.z = static_cast<float>(pPhong->Ambient.Get()[2]);
		//拡散反射光
		_material[i].diffuse.x = static_cast<float>(pPhong->Diffuse.Get()[0]);
		_material[i].diffuse.y = static_cast<float>(pPhong->Diffuse.Get()[1]);
		_material[i].diffuse.z = static_cast<float>(pPhong->Diffuse.Get()[2]);
		//鏡面反射光
		_material[i].specular.x = static_cast<float>(pPhong->Specular.Get()[0]);
		_material[i].specular.y = static_cast<float>(pPhong->Specular.Get()[1]);
		_material[i].specular.z = static_cast<float>(pPhong->Specular.Get()[2]);
		//テクスチャー（ディフューズテクスチャーのみ）
		auto lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);




		int layeredTextureCount = lProperty.GetSrcObjectCount();//複数のテクスチャを使っているか
		if (layeredTextureCount > 1) {
			for (int j = 0; layeredTextureCount > j; j++) {
				FbxLayeredTexture* layeredTexture = lProperty.GetSrcObject<FbxLayeredTexture>(j);
				int textureCount = layeredTexture->GetSrcObjectCount() - lProperty.GetSrcObjectCount();//レイヤー数を取得
				for (int k = 0; textureCount > k; k++) {//レイヤー数だけ繰り返す
					FbxFileTexture* texture = lProperty.GetSrcObject<FbxFileTexture>(k);
					if (texture) {
						std::string textureName = texture->GetRelativeFileName();
						std::string UVSetName = texture->UVSet.Get().Buffer();

					}
				}
			}
			continue;
		}
		int fileTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();//テクスチャ数を取得
		if (fileTextureCount > 0) {
			for (int j = 0; fileTextureCount > j; j++) {
				FbxFileTexture* texture = lProperty.GetSrcObject<FbxFileTexture>(j);
				if (texture) {
					std::string texfilepath = texture->GetRelativeFileName();//テクスチャ名ではなくテクスチャファイル名を取得する
					LoadTexture(TexturePath(texfilepath), device);
					//UVセット名も保持：TODO:　よくわからないので後回し
					//std::string UVSetName = texture->UVSet.Get().Buffer();	
				}
			}
		}

		//マテリアルごとのインデックスバッファ作成
		CreateIndexBuffer(meshdata, device, i);
	}
}


void FBXLoader::CreateVertexBuffer(FbxMesh*& meshdata, std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, std::vector<VERTEX>& vertexdata) {
	ID3D11Buffer* vertexbuffer = nullptr;
	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex = {};
	bd_vertex.ByteWidth = sizeof(VERTEX) * meshdata->GetControlPointsCount();
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex = {};
	data_vertex.pSysMem = &vertexdata[0];
	device->GetDevice()->CreateBuffer(&bd_vertex, &data_vertex, &vertexbuffer);
	_vertexbuffers.push_back(vertexbuffer);
}
void FBXLoader::VertexDataImport(FbxMesh*& meshdata, std::vector<VERTEX>& vertexdata) {

	int m_dwNumUV = meshdata->GetTextureUVCount();
	int facenum = meshdata->GetPolygonCount();

	auto m_dwNumVert = meshdata->GetControlPointsCount();
	if (m_dwNumVert < m_dwNumUV) {
		vertexdata.resize(m_dwNumUV); // 頂点数
	}
	else {
		vertexdata.resize(m_dwNumVert); // 頂点数
	}


	for (DWORD i = 0; i < facenum; i++) {

		int iStartIndex = meshdata->GetPolygonVertexIndex(i);
		int* piIndex = meshdata->GetPolygonVertices();//（頂点インデックス）読み込み
		int iIndex0 = piIndex[iStartIndex];
		int iIndex1 = piIndex[iStartIndex + 1];
		int iIndex2 = piIndex[iStartIndex + 2];
		if (m_dwNumVert<m_dwNumUV)//UV数のほうが多い場合は本来の（頂点ベース）インデックスを利用しない。UVインデックスを基準にする
		{
			iIndex0 = meshdata->GetTextureUVIndex(i, 0, FbxLayerElement::eTextureDiffuse);
			iIndex1 = meshdata->GetTextureUVIndex(i, 1, FbxLayerElement::eTextureDiffuse);
			iIndex2 = meshdata->GetTextureUVIndex(i, 2, FbxLayerElement::eTextureDiffuse);
		}

	
		//頂点座標
		VertexData(meshdata, i, iIndex0, iIndex1, iIndex2, vertexdata);
		//法線
		NormalData(meshdata, i, iIndex0, iIndex1, iIndex2, vertexdata);
		//UV
		UVData(meshdata, i, iIndex0, iIndex1, iIndex2, vertexdata);
		
	}

	//TODO:　バイナリなら必要ないとおもうんだけど
	FbxLayerElementUV* uv = meshdata->GetLayer(0)->GetUVs();
	if (m_dwNumUV && uv->GetMappingMode() == FbxLayerElement::eByControlPoint)
	{
		FbxLayerElementUV* pUV = meshdata->GetLayer(0)->GetUVs();
		for (DWORD k = 0; k<m_dwNumUV; k++)
		{
			vertexdata[k].uv.x = static_cast<float>(pUV->GetDirectArray().GetAt(k)[0]);
			vertexdata[k].uv.y = 1.0f - static_cast<float>(pUV->GetDirectArray().GetAt(k)[1]);
		}
	}
	
}

void FBXLoader::UVData(FbxMesh*& meshdata, int faceidx, int idx0, int idx1, int idx2, std::vector<VERTEX>& vertexdata) {
	int m_dwNumUV = meshdata->GetTextureUVCount();
	int UVindex = meshdata->GetTextureUVIndex(faceidx, 0, FbxLayerElement::eTextureDiffuse);
	FbxLayerElementUV* pUV = 0;
	FbxLayerElementUV* uv = meshdata->GetLayer(0)->GetUVs();

	if (m_dwNumUV && uv->GetMappingMode() == FbxLayerElement::eByPolygonVertex)
	{
		
		UVindex = meshdata->GetTextureUVIndex(faceidx, 0, FbxLayerElement::eTextureDiffuse);
		pUV = meshdata->GetLayer(0)->GetUVs();
		vertexdata[idx0].uv.x = static_cast<float>(pUV->GetDirectArray().GetAt(UVindex)[0]);
		vertexdata[idx0].uv.y = 1.0f - static_cast<float>(pUV->GetDirectArray().GetAt(UVindex)[1]);

		UVindex = meshdata->GetTextureUVIndex(faceidx, 1, FbxLayerElement::eTextureDiffuse);
		
		vertexdata[idx1].uv.x = static_cast<float>(pUV->GetDirectArray().GetAt(UVindex)[0]);
		vertexdata[idx1].uv.y = 1.0f - static_cast<float>(pUV->GetDirectArray().GetAt(UVindex)[1]);

		UVindex = meshdata->GetTextureUVIndex(faceidx, 2, FbxLayerElement::eTextureDiffuse);
		vertexdata[idx2].uv.x = static_cast<float>(pUV->GetDirectArray().GetAt(UVindex)[0]);
		vertexdata[idx2].uv.y = 1.0f - static_cast<float>(pUV->GetDirectArray().GetAt(UVindex)[1]);
	}
	

}
void FBXLoader::VertexData(FbxMesh*& meshdata, int faceidx, int idx0, int idx1, int idx2, std::vector<VERTEX>& vertexdata) {
	int index = meshdata->GetPolygonVertex(faceidx, 0);
	XMFLOAT3 pos = {
		static_cast<float>(-meshdata->GetControlPointAt(index)[0]),//FBXは右手座標系 DirectXは左手系
		static_cast<float>(meshdata->GetControlPointAt(index)[1]),
		static_cast<float>(meshdata->GetControlPointAt(index)[2]),
	};
	vertexdata[idx0].pos = pos;

	int index2 = meshdata->GetPolygonVertex(faceidx, 1);
	XMFLOAT3 pos2 = {
		static_cast<float>(-meshdata->GetControlPointAt(index2)[0]),
		static_cast<float>(meshdata->GetControlPointAt(index2)[1]),
		static_cast<float>(meshdata->GetControlPointAt(index2)[2]),
	};
	vertexdata[idx1].pos = pos2;

	int index3 = meshdata->GetPolygonVertex(faceidx, 2);
	XMFLOAT3 pos3 = {
		static_cast<float>(-meshdata->GetControlPointAt(index3)[0]),
		static_cast<float>(meshdata->GetControlPointAt(index3)[1]),
		static_cast<float>(meshdata->GetControlPointAt(index3)[2]),
	};
	vertexdata[idx2].pos = pos3;
}

void FBXLoader::CreateIndexBuffer(FbxMesh*& meshdata, std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device,int matIdx) {

	ID3D11Buffer* idxbuff = nullptr;
	D3D11_BUFFER_DESC bd_index;
	bd_index.ByteWidth = sizeof(int) * meshdata->GetPolygonVertexCount();
	bd_index.Usage = D3D11_USAGE_DEFAULT;
	bd_index.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd_index.CPUAccessFlags = 0;
	bd_index.MiscFlags = 0;
	bd_index.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_index;
	data_index.pSysMem = meshdata->GetPolygonVertices();
	device->GetDevice()->CreateBuffer(&bd_index, &data_index, &idxbuff);

	_material[matIdx].dwNumFace = meshdata->GetPolygonVertexCount();
	_indexbuffs.push_back(idxbuff);

}
void FBXLoader::NormalData(FbxMesh*& meshdata,int faceidx,int idx0, int idx1, int idx2, std::vector<VERTEX>& vertexdata) {


	
	int normalLayerCount = meshdata->GetElementNormalCount();//法線セット数
	for (int nec = 0; nec < normalLayerCount; nec++) {
		auto Normal = meshdata->GetElementNormal(nec);    //メッシュから法線セットを取得
		auto NormalMapping = Normal->GetMappingMode();        //マッピングモードを取得
		auto NormalReference = Normal->GetReferenceMode();    //リファレンスモードを取得


		int normalNum = Normal->GetDirectArray().GetCount();//法線数
		
		if (NormalMapping == FbxGeometryElement::eByControlPoint && NormalReference == FbxGeometryElement::eDirect) {//インデックスバッファで最適化されたデータ
			continue;
		}
		if (NormalMapping == FbxGeometryElement::eByPolygonVertex && NormalReference == FbxGeometryElement::eIndexToDirect) {//独自インデックスバッファ
			continue;
		}

		if (NormalMapping == FbxGeometryElement::eByPolygonVertex && NormalReference == FbxGeometryElement::eDirect) {//生データのまま入っている
			fbxsdk::FbxVector4 normalvec;
			meshdata->GetPolygonVertexNormal(faceidx, 0, normalvec);
			vertexdata[idx0].normal.x = -normalvec[0];//FBXは右手座標系
			vertexdata[idx0].normal.y = normalvec[1];
			vertexdata[idx0].normal.z = normalvec[2];

			meshdata->GetPolygonVertexNormal(faceidx, 1, normalvec);
			vertexdata[idx1].normal.x = -normalvec[0];
			vertexdata[idx1].normal.y = normalvec[1];
			vertexdata[idx1].normal.z = normalvec[2];

			meshdata->GetPolygonVertexNormal(faceidx, 2, normalvec);
			vertexdata[idx2].normal.x = -normalvec[0];
			vertexdata[idx2].normal.y = normalvec[1];
			vertexdata[idx2].normal.z = normalvec[2];
			continue;
		}
	}
}


void FBXLoader::Render(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, std::shared_ptr<TextureSamplerManager>& texsmpler) {


	for (int vertexidx = 0; vertexidx < _vertexbuffers.size(); vertexidx++) {
		//バーテックスバッファーをセット
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		device->GetDeviceContext()->IASetVertexBuffers(0, 1, &_vertexbuffers[vertexidx], &stride, &offset);


		//マテリアルループ
		for (int i = 0; i < _material.size(); i++)
		{
			//使用されていないマテリアル対策
			if (_material[i].dwNumFace == 0)
			{
				continue;
			}
			//インデックスバッファーをセット
			stride = sizeof(int);
			offset = 0;
			device->GetDeviceContext()->IASetIndexBuffer(_indexbuffs[i], DXGI_FORMAT_R32_UINT, 0);
			device->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			device->GetDeviceContext()->PSSetSamplers(0, 1, &texsmpler->GetTextureSampler());
			device->GetDeviceContext()->PSSetShaderResources(0, 1, &_textures[i]->GetTexView());
			//Draw
			device->GetDeviceContext()->DrawIndexed(_material[i].dwNumFace, 0, 0);

		}
	}

}