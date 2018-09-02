#include "FBXLoader.h"
#include "D3D11Manager\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContextManager.h"
#include "D3D11Manager\Texture\WICTextureLoderManager.h"
#include "D3D11Manager\TextureSampler\TextureSamplerManager.h"
#include "D3D11Manager\Shada\Constant\ConstantManager.h"
#include <tchar.h>
using namespace std;
using namespace DirectX;
using namespace fbxsdk;
//Note: �{�̃T���v�����g����悤�ɂ�������




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


//	FBX�f�[�^����B 
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
	//	�|���S���̎O�p�`���B
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
				printf("FBX: ���f�����O�p�`�����b�V���ɂȂ��Ă��Ȃ��̂Ń��[�h���x���Ȃ�܂��B\n");
				_traiangleCoolF = true;
			}
			lConverter.Triangulate(_pScene, true);
		}
	}

	const int lChildCount = _pNode->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
		// �q�m�[�h��T���B 
		TriangulateRecursive(_pNode->GetChild(lChildIndex), _pScene);
	}
}

//	FBX �I�u�W�F�N�g�� �������B 
bool FBXLoader::InitializeSdkObjects()
{
	// �}�l�[�W���쐬�B 
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

	// IOSettings �I�u�W�F�N�g�쐬�B ���̃I�u�W�F�N�g�͑S�ẴC���|�[�g/�G�N�X�|�[�g�̐ݒ��ێ����܂��B 
	FbxIOSettings* pIOS = FbxIOSettings::Create(m_pFbxManager, IOSROOT);
	m_pFbxManager->SetIOSettings(pIOS);

	// ���s�\�ȃf�B���N�g��(�I�v�V����)����v���O�C�������[�h���܂��B 
	FbxString lPath = FbxGetApplicationDirectory();
	m_pFbxManager->LoadPluginsDirectory(lPath.Buffer());

	// �V�[���쐬�B 
	m_pFbxScene = FbxScene::Create(m_pFbxManager, "Scene");
	if (!m_pFbxScene)
	{
		FBXSDK_printf("Error: Unable to create FBX scene!\n");
		return false;
	}
	return true;
}

//	FBX�t�@�C���̓ǂݍ��� 
bool FBXLoader::Load(const std::string& _pName, std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device)
{
	_fbxfilepath = _pName;
	printf("\n");
	int sFileFormat = -1;

	// FBX SDK �I�u�W�F�N�g�̏������B 
	InitializeSdkObjects();

	// �C���|�[�^�쐬�B 
	m_pFbxImporter = FbxImporter::Create(m_pFbxManager, "Importer");
	if (!m_pFbxImporter)
	{
		FBXSDK_printf("Error: Unable to create FBX importer!\n");
		return false;
	}

	// �t�@�C�����w�肵���C���|�[�g(���[�_�[)�t�@�C���`���Ō��o���܂��B 
	if (!m_pFbxManager->GetIOPluginRegistry()->DetectReaderFileFormat(_pName.c_str(), sFileFormat))
	{
		// ���o�ł��Ȃ��t�@�C���`���ł��B FbxImporter::eFBX_BINARY �`���Ńg���C���܂��B 
		sFileFormat = m_pFbxManager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX binary (*.fbx)");
	}

	// FBX�t�@�C����ǂݍ��ށB 
	if (!m_pFbxImporter->Initialize(_pName.c_str(), sFileFormat))
	{
		FBXSDK_printf("Error: Unable to create FBX initialize!\n");
		return false;
	}

	// FBX �t�@�C���̃o�[�W�����擾�B 
	int major, minor, revision;
	m_pFbxImporter->GetFileVersion(major, minor, revision);
	FBXSDK_printf("FBX VERSION %d %d %d\n", major, minor, revision);

	// �ǂݍ���FBX�t�@�C������V�[���f�[�^�����o���B 
	if (!m_pFbxImporter->Import(m_pFbxScene))
	{
		FBXSDK_printf("Error: Unable to create FBX import!\n");
		return false;
	}

	// ���̐ݒ�B 
	FbxAxisSystem OurAxisSystem = FbxAxisSystem::DirectX;

	// DirectX�n�ɕϊ��B 
	FbxAxisSystem SceneAxisSystem = m_pFbxScene->GetGlobalSettings().GetAxisSystem();
	if (SceneAxisSystem != OurAxisSystem)
	{
		FbxAxisSystem::DirectX.ConvertScene(m_pFbxScene);
	}

	// �P�ʌn�̓���B 
	FbxSystemUnit SceneSystemUnit = m_pFbxScene->GetGlobalSettings().GetSystemUnit();
	if (SceneSystemUnit.GetScaleFactor() != 1.0f)
	{
		// �Z���`���[�^�[�P�ʂɃR���o�[�g����B 
		FbxSystemUnit::cm.ConvertScene(m_pFbxScene);
	}

	// �O�p�`��
	TriangulateRecursive(m_pFbxScene->GetRootNode(), m_pFbxScene);


	//���_�f�[�^�쐬
	LoadVertexData(device);



	//�萔�n������
	//TODO: �F���C���f�b�N�X�ƈꏏ�ɂ���K�v�����邽�߁@�}�e���A�����ƂɕK�v
	std::shared_ptr<ConstantManager> constant(new ConstantManager());
	//constant->CreateConstant(device, sizeof(FBXCBUFF));
	//_cbuffaddress = new D3D11_MAPPED_SUBRESOURCE();
	//constant->ConstantMap(device, _cbuffaddress);
	//
	//memcpy_s(
	//	_cbuffaddress->pData,
	//	_cbuffaddress->RowPitch,
	//	(void*)(&_scenecameramat),//�f�[�^��
	//	sizeof(_scenecameramat)//�T�C�Y��
	//);
	//constant->ConstantUnMap(device);
	
}

void FBXLoader::LoadVertexData(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {

	//Blender��FBX�͒��_�C���f�b�N�X�x�[�X�H

	
	int meshcount = 0;
	_rigidbodyMeshnum = m_pFbxScene->GetRootNode()->GetChildCount();
	printf("���̐�: %d  \n", _rigidbodyMeshnum);
	// ���_�f�[�^�̎��o��
	for (int i = 0; i < m_pFbxScene->GetRootNode()->GetChildCount(); i++) {//�q�܂Ńm�[�h������ : ���̂���������ꍇ���̐����܂����ۂ�
		if (m_pFbxScene->GetRootNode()->GetChild(i)->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh) {
			std::vector<VERTEX> v;
			auto mesh = m_pFbxScene->GetRootNode()->GetChild(i)->GetMesh();
			auto meshnode = m_pFbxScene->GetRootNode()->GetChild(i);
			++meshcount;

			
			
			VertexDataImport(mesh, v);
			if (v.size() <= 0) {
				printf("FBX: empty mesh!   %d�Ԗ� \n", meshcount);
				return;
			}

			//�X�L�j���O�p�f�[�^���[�h
			//LoadSkinAnim(_meshNode);

			//���_�o�b�t�@�쐬
			CreateVertexBuffer(mesh, device, v);
			


			
			//�}�e���A�������o��
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
	auto lastpath = filepath.substr(0, filepath.find_last_of('\\') + 1);//�Ō�Ɍ����w�蕶���܂ł̏ꏊ
	workpath.erase(0, lastpath.size());//���g��ҏW

	//�p�X������
	auto lastpathfbx = _fbxfilepath.substr(0, _fbxfilepath.find_last_of('/') + 1);
	if (lastpathfbx.size() < 1) {
		printf("fbx�w��t�@�C���p�X������\n");
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

	// �ړ��A��]�A�g��̂��߂̍s����쐬
	FbxMatrix globalPosition = mesh->EvaluateGlobalTransform(0);
	FbxVector4 t0 = mesh->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 r0 = mesh->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 s0 = mesh->GetGeometricScaling(FbxNode::eSourcePivot);
	FbxAMatrix geometryOffset = FbxAMatrix(t0, r0, s0);

	
	// �e���_�Ɋ|���邽�߂̍ŏI�I�ȍs��̔z��
	FbxMatrix *clusterDeformation = new FbxMatrix[mesh->GetMesh()->GetControlPointsCount()];
	memset(clusterDeformation, 0, sizeof(FbxMatrix) * mesh->GetMesh()->GetControlPointsCount());



	int clusterCount = skinDeformer->GetClusterCount();
	// �e�N���X�^����e���_�ɉe����^���邽�߂̍s��쐬
	for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++) {
		// �N���X�^(�{�[��)�̎��o��
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
		// ��ō�����s��Ɋe���_���̉e���x(�d��)���|���Ă��ꂼ��ɉ��Z
		for (int i = 0; i < cluster->GetControlPointIndicesCount(); i++) {
			int index = cluster->GetControlPointIndices()[i];
			double weight = cluster->GetControlPointWeights()[i];
			FbxMatrix influence = vertexTransformMatrix * weight;
			clusterDeformation[index] += influence;
		}
	}

	//// �ŏI�I�Ȓ��_���W���v�Z��VERTEX�ɕϊ�
	//for (int i = 0; i < mesh->GetControlPointsCount(); i++) {
	//	FbxVector4 outVertex = clusterDeformation[i].MultNormalize(mesh->GetControlPointAt(i));
	//	vertices2[i].Pos.x = (FLOAT)outVertex[0];
	//	vertices2[i].Pos.y = (FLOAT)outVertex[1];
	//	vertices2[i].Pos.z = (FLOAT)outVertex[2];
	//}


	////FBX���璊�o���ׂ����́A���_���Ƃ̃{�[���C���f�b�N�X�A���_���Ƃ̃{�[���E�F�C�g�A�o�C���h�s��A�|�[�Y�s��@��4����
	//int uvnum = meshNode->GetMesh()->GetTextureUVCount();
	//int facenum = meshNode->GetMesh()->GetPolygonCount();
	//auto vertexnum = meshNode->GetMesh()->GetControlPointsCount();

	//int i, k;
	//int iNumBone = 0;//�{�[����

	////�f�t�H�[�}�[�𓾂� 
	//FbxDeformer* pDeformer = meshNode->GetMesh()->GetDeformer(0);
	//FbxSkin* pSkinInfo = static_cast<FbxSkin*>(pDeformer);
	////
	////�{�[���𓾂�
	//iNumBone = pSkinInfo->GetClusterCount();
	//m_ppCluster = new  FbxCluster*[iNumBone];
	//for (i = 0; i < iNumBone; i++)
	//{
	//	m_ppCluster[i] = pSkinInfo->GetCluster(i);
	//}

	////�ʏ�̏ꍇ�@�i���_����=UV���@pvVB�����_�C���f�b�N�X�x�[�X�̏ꍇ�j
	//if (vertexnum >= uvnum)
	//{
	//	//���ꂼ��̃{�[���ɉe�����󂯂钸�_�𒲂ׂ�@��������t�ɁA���_�x�[�X�Ń{�[���C���f�b�N�X�E�d�݂𐮓ڂ���
	//	for (i = 0; i < iNumBone; i++)
	//	{
	//		int iNumIndex = m_ppCluster[i]->GetControlPointIndicesCount();//���̃{�[���ɉe�����󂯂钸�_��
	//		int* piIndex = m_ppCluster[i]->GetControlPointIndices();
	//		double* pdWeight = m_ppCluster[i]->GetControlPointWeights();
	//		//���_������C���f�b�N�X�����ǂ��āA���_�T�C�h�Ő�������
	//		for (k = 0; k < iNumIndex; k++)
	//		{
	//			for (int m = 0; m < 4; m++)//FBX��CG�\�t�g���{�[��4�{�ȓ��Ƃ͌���Ȃ��B5�{�ȏ�̏ꍇ�́A�d�݂̑傫������4�{�ɍi�� 
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
	////UV�C���f�b�N�X�x�[�X�̏ꍇ�@�i���_��<UV���j
	//else
	//{
	//	int PolyIndex = 0;
	//	int UVIndex = 0;
	//	//���ꂼ��̃{�[���ɉe�����󂯂钸�_�𒲂ׂ�@��������t�ɁA���_�x�[�X�Ń{�[���C���f�b�N�X�E�d�݂𐮓ڂ���
	//	for (i = 0; i < iNumBone; i++)
	//	{
	//		int iNumIndex = m_ppCluster[i]->GetControlPointIndicesCount();//���̃{�[���ɉe�����󂯂钸�_��
	//		int* piIndex = m_ppCluster[i]->GetControlPointIndices();
	//		double* pdWeight = m_ppCluster[i]->GetControlPointWeights();
	//		//���_������C���f�b�N�X�����ǂ��āA���_�T�C�h�Ő�������
	//		for (k = 0; k < iNumIndex; k++)
	//		{
	//			//���̒��_���܂�ł���|���S�����ׂĂɁA���̃{�[���ƃE�F�C�g��K�p
	//			for (int p = 0; p < PolyTable[piIndex[k]].NumRef; p++)
	//			{
	//				//���_�������|���S�������̃|���S����UV�C���f�b�N�X�@�Ƌt����
	//				PolyIndex = PolyTable[piIndex[k]].PolyIndex[p];
	//				UVIndex = meshNode->GetMesh()->GetTextureUVIndex(PolyIndex, PolyTable[piIndex[k]].Index123[p], FbxLayerElement::eTextureDiffuse);

	//				for (int m = 0; m < 4; m++)//FBX��CG�\�t�g���{�[��4�{�ȓ��Ƃ͌���Ȃ��B5�{�ȏ�̏ꍇ�́A�d�݂̑傫������4�{�ɍi�� 
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
	////�{�[���𐶐�
	//// �W���C���g�̕ϊ��s��̎擾
	//m_iNumBone = iNumBone;
	//m_BoneArray = new BONE[iNumBone];
	//std::vector<XMMATRIX> matrixs;

	//std::vector<float> matArry;//�v�f�𕪉����ĘA�������������ɃA�^�b�`����
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

	////�s��ɂ���
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
	
	//�}�e���A���ǂݍ���
	FbxNode* pNode = meshdata->GetNode();
	std::vector<FBX_MATERIAL> material;
	std::vector<ID3D11Buffer*> indexbuffer;
	int facenum = meshdata->GetPolygonCount();
	int m_dwNumMaterial = pNode->GetMaterialCount();
	
	
	

	material.resize(m_dwNumMaterial);//1���b�V��������̎g���Ă���}�e���A����
	indexbuffer.resize(m_dwNumMaterial);




	for (int i = 0; i < m_dwNumMaterial; i++)
	{
		//�t�H�����f����z��
		FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);
		FbxSurfacePhong* pPhong = (FbxSurfacePhong*)pMaterial;

		//����
		material[i].ambient.x = static_cast<float>(pPhong->Ambient.Get()[0]);
		material[i].ambient.y = static_cast<float>(pPhong->Ambient.Get()[1]);
		material[i].ambient.z = static_cast<float>(pPhong->Ambient.Get()[2]);
		//�g�U���ˌ�
		material[i].diffuse.x = static_cast<float>(pPhong->Diffuse.Get()[0]);
		material[i].diffuse.y = static_cast<float>(pPhong->Diffuse.Get()[1]);
		material[i].diffuse.z = static_cast<float>(pPhong->Diffuse.Get()[2]);
		//���ʔ��ˌ�
		material[i].specular.x = static_cast<float>(pPhong->Specular.Get()[0]);
		material[i].specular.y = static_cast<float>(pPhong->Specular.Get()[1]);
		material[i].specular.z = static_cast<float>(pPhong->Specular.Get()[2]);
		//�e�N�X�`���[�i�f�B�t���[�Y�e�N�X�`���[�̂݁j
		auto lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);




		int layeredTextureCount = lProperty.GetSrcObjectCount();//�����̃e�N�X�`�����g���Ă��邩
		if (layeredTextureCount > 1) {
			for (int j = 0; layeredTextureCount > j; j++) {
				FbxLayeredTexture* layeredTexture = lProperty.GetSrcObject<FbxLayeredTexture>(j);
				if (layeredTexture == nullptr) {
					printf("Error:\n");
					continue;
				}
				int textureCount = layeredTexture->GetSrcObjectCount() - lProperty.GetSrcObjectCount();//���C���[�����擾
				for (int k = 0; textureCount > k; k++) {//���C���[�������J��Ԃ�
					FbxFileTexture* texture = lProperty.GetSrcObject<FbxFileTexture>(k);
					if (texture) {
						std::string textureName = texture->GetRelativeFileName();
						std::string UVSetName = texture->UVSet.Get().Buffer();

					}
				}
			}
			continue;
		}
		int fileTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();//�e�N�X�`�������擾
		if (fileTextureCount > 0) {
			for (int j = 0; fileTextureCount > j; j++) {
				FbxFileTexture* texture = lProperty.GetSrcObject<FbxFileTexture>(j);
				if (texture) {
					std::string texfilepath = texture->GetRelativeFileName();//�e�N�X�`�����ł͂Ȃ��e�N�X�`���t�@�C�������擾����
					LoadTexture(TexturePath(texfilepath), device);
					//UV�Z�b�g�����ێ��FTODO:�@�悭�킩��Ȃ��̂Ō��
					//std::string UVSetName = texture->UVSet.Get().Buffer();	
				}
			}
		}



		//�}�e���A�����Ƃ̃C���f�b�N�X�o�b�t�@�쐬
		CreateIndexBuffer(meshdata, device, i, material, meshcount, indexbuffer);
	}
	//�e�[�u���T�C�Y�X�V
	_material.push_back(material);
	_indexbuffers.push_back(indexbuffer);
}

void FBXLoader::CreateVertexBuffer(FbxMesh*& meshdata, std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, std::vector<VERTEX>& vertexdata) {
	ID3D11Buffer* vertexbuffer = nullptr;
	// ���_�f�[�^�p�o�b�t�@�̐ݒ�
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
		int* piIndex = meshdata->GetPolygonVertices();//�i���_�C���f�b�N�X�j�ǂݍ���
		int iIndex0 = piIndex[iStartIndex];
		int iIndex1 = piIndex[iStartIndex + 1];
		int iIndex2 = piIndex[iStartIndex + 2];
	

	
		//���_���W
		VertexData(meshdata, i, iIndex0, iIndex1, iIndex2, vertexdata);
		//�@��
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
		static_cast<float>(-meshdata->GetControlPointAt(index)[0]),//FBX�͉E����W�n DirectX�͍���n
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
	int normalLayerCount = meshdata->GetElementNormalCount();//�@���Z�b�g��
	for (int nec = 0; nec < normalLayerCount; nec++) {
		auto Normal = meshdata->GetElementNormal(nec);    //���b�V������@���Z�b�g���擾
		auto NormalMapping = Normal->GetMappingMode();        //�}�b�s���O���[�h���擾
		auto NormalReference = Normal->GetReferenceMode();    //���t�@�����X���[�h���擾


		int normalNum = Normal->GetDirectArray().GetCount();//�@����
		
		if (NormalMapping == FbxGeometryElement::eByControlPoint && NormalReference == FbxGeometryElement::eDirect) {//�C���f�b�N�X�o�b�t�@�ōœK�����ꂽ�f�[�^
			continue;
		}
		if (NormalMapping == FbxGeometryElement::eByPolygonVertex && NormalReference == FbxGeometryElement::eIndexToDirect) {//�Ǝ��C���f�b�N�X�o�b�t�@
			continue;
		}

		if (NormalMapping == FbxGeometryElement::eByPolygonVertex && NormalReference == FbxGeometryElement::eDirect) {//���f�[�^�̂܂ܓ����Ă���
			fbxsdk::FbxVector4 normalvec;
			meshdata->GetPolygonVertexNormal(faceidx, 0, normalvec);
			vertexdata[idx0].normal.x = -normalvec[0];//FBX�͉E����W�n
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


	//�C���f�b�N�X�ƃ}�e���A���͕ʂŕۑ����ׂ����H
	//��͂�C���f�b�N�X���Ⴄ���b�V���̃C���f�b�N�X���g���ċ������ۂ�

	//���b�V���ɂ��Ă͐�����������
	for (int meshidx = 0; meshidx < _vertexbuffers.size(); meshidx++) {



		//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		device->GetDeviceContext()->IASetVertexBuffers(0, 1, &_vertexbuffers[meshidx], &stride, &offset);


		//�}�e���A�����[�v
		for (int materialidx = 0; materialidx < _material[meshidx].size(); materialidx++)
		{
			device->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			//�g�p����Ă��Ȃ��}�e���A���΍�
			if (_material[meshidx][materialidx].fecenum == 0)
			{
				continue;
			}
			//�C���f�b�N�X�o�b�t�@�[���Z�b�g
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