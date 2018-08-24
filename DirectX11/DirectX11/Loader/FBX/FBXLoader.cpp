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
	CreateVertexBuffer(device);

	return true;
}

void FBXLoader::CreateVertexBuffer(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {

	//Note: ASCII�ɂȂ��ĂȂ��o�C�i������Ȃ���͂��������Ȃ邩��
	//Note: �t�@�C�����ɃJ�����⃉�C�g���̂����Ă���Ƃ�������b�V���Ƃ��ĂƂ邩������Ȃ�
	//�T���v���̐�Ԃ͂��Ԃ�g���Ȃ��@�o�[�W�����̂����H�@�Â��H

	
	_rigidbodyMeshnum = m_pFbxScene->GetRootNode()->GetChildCount();
	printf("���̐�: %d  \n", _rigidbodyMeshnum);
	// ���_�f�[�^�̎��o��
	for (int i = 0; i < m_pFbxScene->GetRootNode()->GetChildCount(); i++) {//�q�܂Ńm�[�h������ : ���̂���������ꍇ���̐����܂����ۂ�
		if (m_pFbxScene->GetRootNode()->GetChild(i)->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh) {
			std::vector<VERTEX> v;
			auto _mesh = m_pFbxScene->GetRootNode()->GetChild(i)->GetMesh();

			
			
			VertexDataImport(_mesh, v);
			//�}�e���A�������o��
			MaterialData(_mesh, device);
			//���_�o�b�t�@�쐬
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

void FBXLoader::MaterialData(FbxMesh*& meshdata,std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {
	//�}�e���A���ǂݍ���
	FbxNode* pNode = meshdata->GetNode();
	int m_dwNumMaterial = pNode->GetMaterialCount();
	int facenum = meshdata->GetPolygonCount();
	_material.resize(m_dwNumMaterial);



	for (int i = 0; i < m_dwNumMaterial; i++)
	{
		//�t�H�����f����z��
		FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);
		FbxSurfacePhong* pPhong = (FbxSurfacePhong*)pMaterial;

		//����
		_material[i].ambient.x = static_cast<float>(pPhong->Ambient.Get()[0]);
		_material[i].ambient.y = static_cast<float>(pPhong->Ambient.Get()[1]);
		_material[i].ambient.z = static_cast<float>(pPhong->Ambient.Get()[2]);
		//�g�U���ˌ�
		_material[i].diffuse.x = static_cast<float>(pPhong->Diffuse.Get()[0]);
		_material[i].diffuse.y = static_cast<float>(pPhong->Diffuse.Get()[1]);
		_material[i].diffuse.z = static_cast<float>(pPhong->Diffuse.Get()[2]);
		//���ʔ��ˌ�
		_material[i].specular.x = static_cast<float>(pPhong->Specular.Get()[0]);
		_material[i].specular.y = static_cast<float>(pPhong->Specular.Get()[1]);
		_material[i].specular.z = static_cast<float>(pPhong->Specular.Get()[2]);
		//�e�N�X�`���[�i�f�B�t���[�Y�e�N�X�`���[�̂݁j
		auto lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);




		int layeredTextureCount = lProperty.GetSrcObjectCount();//�����̃e�N�X�`�����g���Ă��邩
		if (layeredTextureCount > 1) {
			for (int j = 0; layeredTextureCount > j; j++) {
				FbxLayeredTexture* layeredTexture = lProperty.GetSrcObject<FbxLayeredTexture>(j);
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
		CreateIndexBuffer(meshdata, device, i);
	}
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
		vertexdata.resize(m_dwNumUV); // ���_��
	}
	else {
		vertexdata.resize(m_dwNumVert); // ���_��
	}


	for (DWORD i = 0; i < facenum; i++) {

		int iStartIndex = meshdata->GetPolygonVertexIndex(i);
		int* piIndex = meshdata->GetPolygonVertices();//�i���_�C���f�b�N�X�j�ǂݍ���
		int iIndex0 = piIndex[iStartIndex];
		int iIndex1 = piIndex[iStartIndex + 1];
		int iIndex2 = piIndex[iStartIndex + 2];
		if (m_dwNumVert<m_dwNumUV)//UV���̂ق��������ꍇ�͖{���́i���_�x�[�X�j�C���f�b�N�X�𗘗p���Ȃ��BUV�C���f�b�N�X����ɂ���
		{
			iIndex0 = meshdata->GetTextureUVIndex(i, 0, FbxLayerElement::eTextureDiffuse);
			iIndex1 = meshdata->GetTextureUVIndex(i, 1, FbxLayerElement::eTextureDiffuse);
			iIndex2 = meshdata->GetTextureUVIndex(i, 2, FbxLayerElement::eTextureDiffuse);
		}

	
		//���_���W
		VertexData(meshdata, i, iIndex0, iIndex1, iIndex2, vertexdata);
		//�@��
		NormalData(meshdata, i, iIndex0, iIndex1, iIndex2, vertexdata);
		//UV
		UVData(meshdata, i, iIndex0, iIndex1, iIndex2, vertexdata);
		
	}

	//TODO:�@�o�C�i���Ȃ�K�v�Ȃ��Ƃ������񂾂���
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


	for (int vertexidx = 0; vertexidx < _vertexbuffers.size(); vertexidx++) {
		//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		device->GetDeviceContext()->IASetVertexBuffers(0, 1, &_vertexbuffers[vertexidx], &stride, &offset);


		//�}�e���A�����[�v
		for (int i = 0; i < _material.size(); i++)
		{
			//�g�p����Ă��Ȃ��}�e���A���΍�
			if (_material[i].dwNumFace == 0)
			{
				continue;
			}
			//�C���f�b�N�X�o�b�t�@�[���Z�b�g
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