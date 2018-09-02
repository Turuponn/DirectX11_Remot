#include "FBXLoader.h"
#include "D3D11Manager\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContextManager.h"
#include "D3D11Manager\Texture\WICTextureLoderManager.h"
#include "D3D11Manager\TextureSampler\TextureSamplerManager.h"
#include "D3D11Manager\Shada\Constant\ConstantManager.h"
#include <tchar.h>
using namespace std;
using namespace DirectX;
using namespace fbxsdk;
//Note: 本のサンプルを使えるようにしただけ




FBXLoader::FBXLoader() {
	m_pFbxManager = NULL;
	m_pFbxScene = NULL;
	m_pFbxImporter = NULL;
	_rigidbodyMeshnum = 0;
	_traiangleCoolF = false;
	
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
			if (_traiangleCoolF == false) {
				printf("FBX: モデルが三角形化メッシュになっていないのでロードが遅くなります。\n");
				_traiangleCoolF = true;
			}
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
	LoadVertexData(device);



	//定数系初期化
	//TODO: 色をインデックスと一緒にする必要があるため　マテリアルごとに必要
	std::shared_ptr<ConstantManager> constant(new ConstantManager());
	//constant->CreateConstant(device, sizeof(FBXCBUFF));
	//_cbuffaddress = new D3D11_MAPPED_SUBRESOURCE();
	//constant->ConstantMap(device, _cbuffaddress);
	//
	//memcpy_s(
	//	_cbuffaddress->pData,
	//	_cbuffaddress->RowPitch,
	//	(void*)(&_scenecameramat),//データ部
	//	sizeof(_scenecameramat)//サイズ部
	//);
	//constant->ConstantUnMap(device);
	
}

void FBXLoader::LoadVertexData(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {

	//BlenderのFBXは頂点インデックスベース？

	
	int meshcount = 0;
	_rigidbodyMeshnum = m_pFbxScene->GetRootNode()->GetChildCount();
	printf("剛体数: %d  \n", _rigidbodyMeshnum);
	// 頂点データの取り出し
	for (int i = 0; i < m_pFbxScene->GetRootNode()->GetChildCount(); i++) {//子までノードを巡る : 剛体が複数ある場合その数分まわるっぽい
		if (m_pFbxScene->GetRootNode()->GetChild(i)->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh) {
			std::vector<VERTEX> v;
			auto mesh = m_pFbxScene->GetRootNode()->GetChild(i)->GetMesh();
			auto meshnode = m_pFbxScene->GetRootNode()->GetChild(i);
			++meshcount;

			
			
			VertexDataImport(mesh, v);
			if (v.size() <= 0) {
				printf("FBX: empty mesh!   %d番目 \n", meshcount);
				return;
			}

			//スキニング用データロード
			//LoadSkinAnim(_meshNode);

			//頂点バッファ作成
			CreateVertexBuffer(mesh, device, v);
			


			
			//マテリアル情報取り出し
			/*_material.resize(meshcount);
			_indexbuffers.resize(meshcount);*/
			MaterialData(mesh, device, meshcount);
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
void FBXLoader::LoadSkinAnim(FbxNode* mesh) {

	// ----- Animation -----
	/*timeCount += FrameTime;
	if (timeCount > stop) timeCount = start;*/

	FbxSkin* skinDeformer = nullptr;
	skinDeformer = (FbxSkin *)mesh->GetMesh()->GetDeformer(0, FbxDeformer::eSkin);
	if (skinDeformer == nullptr) {
		printf("FBX:  Not Animation Loader\n");
		return;
	}
	if (mesh->GetMesh() == nullptr) {
		throw(1);
	}

	// 移動、回転、拡大のための行列を作成
	FbxMatrix globalPosition = mesh->EvaluateGlobalTransform(0);
	FbxVector4 t0 = mesh->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 r0 = mesh->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 s0 = mesh->GetGeometricScaling(FbxNode::eSourcePivot);
	FbxAMatrix geometryOffset = FbxAMatrix(t0, r0, s0);

	
	// 各頂点に掛けるための最終的な行列の配列
	FbxMatrix *clusterDeformation = new FbxMatrix[mesh->GetMesh()->GetControlPointsCount()];
	memset(clusterDeformation, 0, sizeof(FbxMatrix) * mesh->GetMesh()->GetControlPointsCount());



	int clusterCount = skinDeformer->GetClusterCount();
	// 各クラスタから各頂点に影響を与えるための行列作成
	for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++) {
		// クラスタ(ボーン)の取り出し
		FbxCluster *cluster = skinDeformer->GetCluster(clusterIndex);
		FbxMatrix vertexTransformMatrix;
		FbxAMatrix referenceGlobalInitPosition;
		FbxAMatrix clusterGlobalInitPosition;
		FbxMatrix clusterGlobalCurrentPosition;
		FbxMatrix clusterRelativeInitPosition;
		FbxMatrix clusterRelativeCurrentPositionInverse;
		cluster->GetTransformMatrix(referenceGlobalInitPosition);
		referenceGlobalInitPosition *= geometryOffset;
		cluster->GetTransformLinkMatrix(clusterGlobalInitPosition);
		clusterGlobalCurrentPosition = cluster->GetLink()->EvaluateGlobalTransform(0);
		clusterRelativeInitPosition = clusterGlobalInitPosition.Inverse() * referenceGlobalInitPosition;
		clusterRelativeCurrentPositionInverse = globalPosition.Inverse() * clusterGlobalCurrentPosition;
		vertexTransformMatrix = clusterRelativeCurrentPositionInverse * clusterRelativeInitPosition;
		// 上で作った行列に各頂点毎の影響度(重み)を掛けてそれぞれに加算
		for (int i = 0; i < cluster->GetControlPointIndicesCount(); i++) {
			int index = cluster->GetControlPointIndices()[i];
			double weight = cluster->GetControlPointWeights()[i];
			FbxMatrix influence = vertexTransformMatrix * weight;
			clusterDeformation[index] += influence;
		}
	}

	//// 最終的な頂点座標を計算しVERTEXに変換
	//for (int i = 0; i < mesh->GetControlPointsCount(); i++) {
	//	FbxVector4 outVertex = clusterDeformation[i].MultNormalize(mesh->GetControlPointAt(i));
	//	vertices2[i].Pos.x = (FLOAT)outVertex[0];
	//	vertices2[i].Pos.y = (FLOAT)outVertex[1];
	//	vertices2[i].Pos.z = (FLOAT)outVertex[2];
	//}


	////FBXから抽出すべき情報は、頂点ごとのボーンインデックス、頂点ごとのボーンウェイト、バインド行列、ポーズ行列　の4項目
	//int uvnum = meshNode->GetMesh()->GetTextureUVCount();
	//int facenum = meshNode->GetMesh()->GetPolygonCount();
	//auto vertexnum = meshNode->GetMesh()->GetControlPointsCount();

	//int i, k;
	//int iNumBone = 0;//ボーン数

	////デフォーマーを得る 
	//FbxDeformer* pDeformer = meshNode->GetMesh()->GetDeformer(0);
	//FbxSkin* pSkinInfo = static_cast<FbxSkin*>(pDeformer);
	////
	////ボーンを得る
	//iNumBone = pSkinInfo->GetClusterCount();
	//m_ppCluster = new  FbxCluster*[iNumBone];
	//for (i = 0; i < iNumBone; i++)
	//{
	//	m_ppCluster[i] = pSkinInfo->GetCluster(i);
	//}

	////通常の場合　（頂点数＞=UV数　pvVBが頂点インデックスベースの場合）
	//if (vertexnum >= uvnum)
	//{
	//	//それぞれのボーンに影響を受ける頂点を調べる　そこから逆に、頂点ベースでボーンインデックス・重みを整頓する
	//	for (i = 0; i < iNumBone; i++)
	//	{
	//		int iNumIndex = m_ppCluster[i]->GetControlPointIndicesCount();//このボーンに影響を受ける頂点数
	//		int* piIndex = m_ppCluster[i]->GetControlPointIndices();
	//		double* pdWeight = m_ppCluster[i]->GetControlPointWeights();
	//		//頂点側からインデックスをたどって、頂点サイドで整理する
	//		for (k = 0; k < iNumIndex; k++)
	//		{
	//			for (int m = 0; m < 4; m++)//FBXやCGソフトがボーン4本以内とは限らない。5本以上の場合は、重みの大きい順に4本に絞る 
	//			{
	//				if (pdWeight[k] > pvVB[piIndex[k]].boneIndex[m])
	//				{
	//					pvVB[piIndex[k]].boneIndex[m] = i;
	//					pvVB[piIndex[k]].boneWeight[m] = pdWeight[k];
	//					break;
	//				}
	//			}
	//		}
	//	}
	//}
	////UVインデックスベースの場合　（頂点数<UV数）
	//else
	//{
	//	int PolyIndex = 0;
	//	int UVIndex = 0;
	//	//それぞれのボーンに影響を受ける頂点を調べる　そこから逆に、頂点ベースでボーンインデックス・重みを整頓する
	//	for (i = 0; i < iNumBone; i++)
	//	{
	//		int iNumIndex = m_ppCluster[i]->GetControlPointIndicesCount();//このボーンに影響を受ける頂点数
	//		int* piIndex = m_ppCluster[i]->GetControlPointIndices();
	//		double* pdWeight = m_ppCluster[i]->GetControlPointWeights();
	//		//頂点側からインデックスをたどって、頂点サイドで整理する
	//		for (k = 0; k < iNumIndex; k++)
	//		{
	//			//その頂点を含んでいるポリゴンすべてに、このボーンとウェイトを適用
	//			for (int p = 0; p < PolyTable[piIndex[k]].NumRef; p++)
	//			{
	//				//頂点→属すポリゴン→そのポリゴンのUVインデックス　と逆引き
	//				PolyIndex = PolyTable[piIndex[k]].PolyIndex[p];
	//				UVIndex = meshNode->GetMesh()->GetTextureUVIndex(PolyIndex, PolyTable[piIndex[k]].Index123[p], FbxLayerElement::eTextureDiffuse);

	//				for (int m = 0; m < 4; m++)//FBXやCGソフトがボーン4本以内とは限らない。5本以上の場合は、重みの大きい順に4本に絞る 
	//				{
	//					if (pdWeight[k] > pvVB[UVIndex].boneWeight[m])
	//					{
	//						pvVB[UVIndex].boneIndex[m] = i;
	//						pvVB[UVIndex].boneWeight[m] = pdWeight[k];
	//						break;
	//					}
	//				}
	//			}

	//		}
	//	}
	//}

	//
	////ボーンを生成
	//// ジョイントの変換行列の取得
	//m_iNumBone = iNumBone;
	//m_BoneArray = new BONE[iNumBone];
	//std::vector<XMMATRIX> matrixs;

	//std::vector<float> matArry;//要素を分解して連続したメモリにアタッチする
	//for (int bonenumidx = 0; bonenumidx<m_iNumBone; bonenumidx++){
	//	FbxAMatrix mat;
	//	m_ppCluster[bonenumidx]->GetTransformLinkMatrix(mat);
	//	for (int row = 0; row<4; row++)
	//	{
	//		for (int col = 0; col<4; col++)
	//		{
	//			matArry.push_back(mat.Get(row, col));
	//		}
	//	}
	//}

	////行列にする
	//int j = 0;
	//for (int i = 0; i < m_iNumBone; i++) {
	//	XMMATRIX mat(
	//		matArry[0], matArry[1], matArry[2], matArry[3],
	//		matArry[4], matArry[5] * -1, matArry[6], matArry[7],
	//		matArry[8], matArry[9], matArry[10], matArry[11],
	//		matArry[12], matArry[13], matArry[14], matArry[15]
	//	);
	//	matrixs.push_back(mat);
	//		
	//}
	//


	
}

	


void FBXLoader::InitSkinAnim() {

}
void FBXLoader::MaterialData(FbxMesh*& meshdata,std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, int meshcount) {
	
	//マテリアル読み込み
	FbxNode* pNode = meshdata->GetNode();
	std::vector<FBX_MATERIAL> material;
	std::vector<ID3D11Buffer*> indexbuffer;
	int facenum = meshdata->GetPolygonCount();
	int m_dwNumMaterial = pNode->GetMaterialCount();
	
	
	

	material.resize(m_dwNumMaterial);//1メッシュあたりの使われているマテリアル数
	indexbuffer.resize(m_dwNumMaterial);




	for (int i = 0; i < m_dwNumMaterial; i++)
	{
		//フォンモデルを想定
		FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);
		FbxSurfacePhong* pPhong = (FbxSurfacePhong*)pMaterial;

		//環境光
		material[i].ambient.x = static_cast<float>(pPhong->Ambient.Get()[0]);
		material[i].ambient.y = static_cast<float>(pPhong->Ambient.Get()[1]);
		material[i].ambient.z = static_cast<float>(pPhong->Ambient.Get()[2]);
		//拡散反射光
		material[i].diffuse.x = static_cast<float>(pPhong->Diffuse.Get()[0]);
		material[i].diffuse.y = static_cast<float>(pPhong->Diffuse.Get()[1]);
		material[i].diffuse.z = static_cast<float>(pPhong->Diffuse.Get()[2]);
		//鏡面反射光
		material[i].specular.x = static_cast<float>(pPhong->Specular.Get()[0]);
		material[i].specular.y = static_cast<float>(pPhong->Specular.Get()[1]);
		material[i].specular.z = static_cast<float>(pPhong->Specular.Get()[2]);
		//テクスチャー（ディフューズテクスチャーのみ）
		auto lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);




		int layeredTextureCount = lProperty.GetSrcObjectCount();//複数のテクスチャを使っているか
		if (layeredTextureCount > 1) {
			for (int j = 0; layeredTextureCount > j; j++) {
				FbxLayeredTexture* layeredTexture = lProperty.GetSrcObject<FbxLayeredTexture>(j);
				if (layeredTexture == nullptr) {
					printf("Error:\n");
					continue;
				}
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
		CreateIndexBuffer(meshdata, device, i, material, meshcount, indexbuffer);
	}
	//テーブルサイズ更新
	_material.push_back(material);
	_indexbuffers.push_back(indexbuffer);
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
		vertexdata.resize(m_dwNumUV);
	}
	else {
		vertexdata.resize(m_dwNumVert);
	}


	for (DWORD i = 0; i < facenum; i++) {

		int iStartIndex = meshdata->GetPolygonVertexIndex(i);
		int* piIndex = meshdata->GetPolygonVertices();//（頂点インデックス）読み込み
		int iIndex0 = piIndex[iStartIndex];
		int iIndex1 = piIndex[iStartIndex + 1];
		int iIndex2 = piIndex[iStartIndex + 2];
	

	
		//頂点座標
		VertexData(meshdata, i, iIndex0, iIndex1, iIndex2, vertexdata);
		//法線
		NormalData(meshdata, i, iIndex0, iIndex1, iIndex2, vertexdata);
		//UV
		UVData(meshdata, i, iIndex0, iIndex1, iIndex2, vertexdata);
		
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

void FBXLoader::CreateIndexBuffer(FbxMesh*& meshdata, std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device,int matIdx, std::vector<FBX_MATERIAL>& material,int meshcount, std::vector<ID3D11Buffer*>& idxbuffers) {

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



	material[matIdx].fecenum = meshdata->GetPolygonVertexCount();
	idxbuffers[matIdx] = idxbuff;
	

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


	//インデックスとマテリアルは別で保存すべきか？
	//やはりインデックスを違うメッシュのインデックスを使って居たっぽい

	//メッシュにしては数が多すぎる
	for (int meshidx = 0; meshidx < _vertexbuffers.size(); meshidx++) {



		//バーテックスバッファーをセット
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		device->GetDeviceContext()->IASetVertexBuffers(0, 1, &_vertexbuffers[meshidx], &stride, &offset);


		//マテリアルループ
		for (int materialidx = 0; materialidx < _material[meshidx].size(); materialidx++)
		{
			device->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			//使用されていないマテリアル対策
			if (_material[meshidx][materialidx].fecenum == 0)
			{
				continue;
			}
			//インデックスバッファーをセット
			stride = sizeof(int);
			offset = 0;
			device->GetDeviceContext()->IASetIndexBuffer(_indexbuffers[meshidx][materialidx], DXGI_FORMAT_R32_UINT, 0);
			if (_textures.size() > 0) {

				device->GetDeviceContext()->PSSetSamplers(0, 1, &texsmpler->GetTextureSampler());
				device->GetDeviceContext()->PSSetShaderResources(0, 1, &_textures[materialidx]->GetTexView());

			}

			//Draw
			device->GetDeviceContext()->DrawIndexed(_material[meshidx][materialidx].fecenum, 0, 0);

		}
	}

}