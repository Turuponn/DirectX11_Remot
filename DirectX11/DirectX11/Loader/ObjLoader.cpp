#include "ObjLoader.h"
#include <stdio.h>
#include <d3d11.h>



#include "D3D11Manager\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContextManager.h"
#include "D3D11Manager\Texture\WICTextureLoderManager.h"
#include "D3D11Manager\TextureSampler\TextureSamplerManager.h"

/*
���f���ɃG�b�W���������Ă����ꍇ�C���f�b�N�X�������H
�x�C�}�b�N�X�̃��f�����؂̃��f���̂ق������_������������Ȃ�(�x�C�}�b�N�X�͋����j
*/


ObjLoader::ObjLoader() {
	m_dwNumMaterial = 0;
}
ObjLoader::~ObjLoader() {

}
int StringToWString(std::wstring &ws, const std::string &s)
{
	std::wstring wsTmp(s.begin(), s.end());

	ws = wsTmp;

	return 0;
}
//�}�e���A���t�@�C����ǂݍ��ފ֐�
HRESULT ObjLoader::LoadMaterialFromFile(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, OBJ_MATERIAL** ppMaterial)
{
	std::shared_ptr<WICTextureLoderManager> w(new WICTextureLoderManager());

	//�}�e���A���t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	fopen_s(&fp, "resoce/tree/Tree.mtl", "rt");
	char key[110] = { 0 };
	DirectX::XMFLOAT4 v(0, 0, 0, 1);

	//�}�e���A�����𒲂ׂ�
	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s ", key, sizeof(key));
		//�}�e���A����
		if (strcmp(key, "newmtl") == 0)
		{
			m_dwNumMaterial++;
		}
	}
	OBJ_MATERIAL* pMaterial = new OBJ_MATERIAL[m_dwNumMaterial];

	//�{�ǂݍ���	
	fseek(fp, SEEK_SET, 0);
	INT iMCount = -1;

	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s ", key, sizeof(key));
		//�}�e���A����
		if (strcmp(key, "newmtl") == 0)
		{
			iMCount++;
			fscanf_s(fp, "%s ", key, sizeof(key));
			strcpy_s(pMaterial[iMCount].szName, key);
		}
		//Ka�@�A���r�G���g
		if (strcmp(key, "Ka") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			pMaterial[iMCount].ambient = v;
		}
		//Kd�@�f�B�t���[�Y
		if (strcmp(key, "Kd") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			pMaterial[iMCount].diffuse = v;
		}
		//Ks�@�X�y�L�����[
		if (strcmp(key, "Ks") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			pMaterial[iMCount].specular = v;
		}
		//map_Kd�@�e�N�X�`���[
		if (strcmp(key, "map_Kd") == 0)
		{
			fscanf_s(fp, "%s", &pMaterial[iMCount].szTextureName, sizeof(pMaterial[iMCount].szTextureName));

			std::wstring filepaths;
			StringToWString(filepaths, pMaterial[iMCount].szTextureName);
			//�e�N�X�`���[���쐬
			//w->LoadTexture(device, filepaths);
			//pMaterial[iMCount].pTexture = w->GetTexView();
			
		}
	}
	fclose(fp);

	*ppMaterial = pMaterial;

	return S_OK;
}

//
HRESULT ObjLoader::InitStaticMesh(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device,const char* FileName, OBJ_MESH** pMesh)
{
	std::shared_ptr<TextureSamplerManager> texsmp(new TextureSamplerManager());
	texsmp->CreateTextureSampler(device);
	_texsmpler = texsmp;
	*pMesh = new OBJ_MESH();


	float x, y, z;
	int v1 = 0, v2 = 0, v3 = 0;
	int vn1 = 0, vn2 = 0, vn3 = 0;
	int vt1 = 0, vt2 = 0, vt3 = 0;
	DWORD dwVCount = 0;//�ǂݍ��݃J�E���^�[
	DWORD dwVNCount = 0;//�ǂݍ��݃J�E���^�[
	DWORD dwVTCount = 0;//�ǂݍ��݃J�E���^�[
	DWORD dwFCount = 0;//�ǂݍ��݃J�E���^�[

	char key[200] = { 0 };
	//OBJ�t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	//���O�ɒ��_���A�|���S�����𒲂ׂ�
	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s ", key, sizeof(key));
		//�}�e���A���ǂݍ���
		if (strcmp(key, "mtllib") == 0)
		{
			fscanf_s(fp, "%s ", key, sizeof(key));
			LoadMaterialFromFile(device, &m_pMaterial);
		}
		//���_
		if (strcmp(key, "v") == 0)
		{
			(*pMesh)->vertexnum++;
		}
		//�@��
		if (strcmp(key, "vn") == 0)
		{
			dwVNCount++;
		}
		//�e�N�X�`���[���W
		if (strcmp(key, "vt") == 0)
		{
			dwVTCount++;
		}
		//�t�F�C�X�i�|���S���j
		if (strcmp(key, "f") == 0)
		{
			(*pMesh)->faceindexnum++;
		}
	}

	//�ꎞ�I�ȃ������m�ہi���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�j
	OBJ_VERTEX* pvVertexBuffer = new OBJ_VERTEX[(*pMesh)->faceindexnum * 3];
	DirectX::XMFLOAT3* pvCoord = new DirectX::XMFLOAT3[(*pMesh)->vertexnum];
	DirectX::XMFLOAT3* pvNormal = new DirectX::XMFLOAT3[dwVNCount];
	DirectX::XMFLOAT2* pvTexture = new DirectX::XMFLOAT2[dwVTCount];

	//�{�ǂݍ���	
	fseek(fp, SEEK_SET, 0);
	dwVCount = 0;
	dwVNCount = 0;
	dwVTCount = 0;
	dwFCount = 0;

	while (!feof(fp))
	{
		//�L�[���[�h �ǂݍ���
		ZeroMemory(key, sizeof(key));
		fscanf_s(fp, "%s ", key, sizeof(key));

		//���_ �ǂݍ���
		if (strcmp(key, "v") == 0)
		{
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			pvCoord[dwVCount].x = -x;//OBJ�͉E����W�n�Ȃ̂�x���邢�͂��𔽓]
			pvCoord[dwVCount].y = y;
			pvCoord[dwVCount].z = z;
			dwVCount++;
		}

		//�@�� �ǂݍ���
		if (strcmp(key, "vn") == 0)
		{
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			pvNormal[dwVNCount].x = -x;//OBJ�͉E����W�n�Ȃ̂�x���邢�͂��𔽓]
			pvNormal[dwVNCount].y = y;
			pvNormal[dwVNCount].z = z;
			dwVNCount++;
		}

		//�e�N�X�`���[���W �ǂݍ���
		if (strcmp(key, "vt") == 0)
		{
			fscanf_s(fp, "%f %f", &x, &y);
			pvTexture[dwVTCount].x = x;
			pvTexture[dwVTCount].y = 1 - y;//OBJ�t�@�C����Y�������t�Ȃ̂ō��킹��
			dwVTCount++;
		}
	}

	//�}�e���A���̐������C���f�b�N�X�o�b�t�@�[���쐬
	(*pMesh)->ppIndexBuffer = new ID3D11Buffer*[m_dwNumMaterial];

	//�t�F�C�X�@�ǂݍ��݁@�o���o���Ɏ��^����Ă���\��������̂ŁA�}�e���A�����𗊂�ɂȂ����킹��
	bool boFlag = false;
	int* piFaceBuffer = new int[(*pMesh)->faceindexnum * 3];//�R���_�|���S���Ȃ̂ŁA1�t�F�C�X=3���_(3�C���f�b�N�X)
	dwFCount = 0;
	DWORD dwPartFCount = 0;
	for (DWORD i = 0; i<m_dwNumMaterial; i++)
	{
		dwPartFCount = 0;
		fseek(fp, SEEK_SET, 0);

		while (!feof(fp))
		{
			//�L�[���[�h �ǂݍ���
			ZeroMemory(key, sizeof(key));
			fscanf_s(fp, "%s ", key, sizeof(key));

			//�t�F�C�X �ǂݍ��݁����_�C���f�b�N�X��
			if (strcmp(key, "usemtl") == 0)
			{
				fscanf_s(fp, "%s ", key, sizeof(key));
				if (strcmp(key, m_pMaterial[i].szName) == 0)
				{
					boFlag = true;
				}
				else
				{
					boFlag = false;
				}
			}
			if (strcmp(key, "f") == 0 && boFlag == true)
			{
				if (m_pMaterial[i].pTexture != NULL)//�e�N�X�`���[����T�[�t�F�C�X
				{
					fscanf_s(fp, "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
				}
				else//�e�N�X�`���[�����T�[�t�F�C�X
				{
					fscanf_s(fp, "%d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3);
				}

				//�C���f�b�N�X�o�b�t�@�[
				piFaceBuffer[dwPartFCount * 3] = dwFCount * 3;
				piFaceBuffer[dwPartFCount * 3 + 1] = dwFCount * 3 + 1;
				piFaceBuffer[dwPartFCount * 3 + 2] = dwFCount * 3 + 2;
				//���_�\���̂ɑ��
				pvVertexBuffer[dwFCount * 3].pos = pvCoord[v1 - 1];
				pvVertexBuffer[dwFCount * 3].normal = pvNormal[vn1 - 1];
				pvVertexBuffer[dwFCount * 3].uv = pvTexture[vt1 - 1];
				pvVertexBuffer[dwFCount * 3 + 1].pos = pvCoord[v2 - 1];
				pvVertexBuffer[dwFCount * 3 + 1].normal = pvNormal[vn2 - 1];
				pvVertexBuffer[dwFCount * 3 + 1].uv = pvTexture[vt2 - 1];
				pvVertexBuffer[dwFCount * 3 + 2].pos = pvCoord[v3 - 1];
				pvVertexBuffer[dwFCount * 3 + 2].normal = pvNormal[vn3 - 1];
				pvVertexBuffer[dwFCount * 3 + 2].uv = pvTexture[vt3 - 1];

				dwPartFCount++;
				dwFCount++;

			}
		}
		if (dwPartFCount == 0)//�g�p����Ă��Ȃ��}�e���A���΍�
		{
			(*pMesh)->ppIndexBuffer[i] = NULL;
			continue;
		}

		//�C���f�b�N�X�o�b�t�@�[���쐬
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * dwPartFCount * 3;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = piFaceBuffer;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		if (FAILED(device->GetDevice()->CreateBuffer(&bd, &InitData, &(*pMesh)->ppIndexBuffer[i]))) {
			throw(1);
		}
		m_pMaterial[i].dwNumFace = dwPartFCount;
	}
	delete[] piFaceBuffer;
	fclose(fp);

	//�o�[�e�b�N�X�o�b�t�@�[���쐬
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(OBJ_VERTEX) * (*pMesh)->faceindexnum * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pvVertexBuffer;
	if (FAILED(device->GetDevice()->CreateBuffer(&bd, &InitData, &(*pMesh)->pVertexBuffer)))
		throw(1);

	//�ꎞ�I�ȓ��ꕨ�́A���͂�s�v
	delete pvCoord;
	delete pvNormal;
	delete pvTexture;
	delete[] pvVertexBuffer;


	return S_OK;
}


//�V�[������ʂɃ����_�����O
void ObjLoader::Render(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, OBJ_MESH* pMesh)
{

	
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(OBJ_VERTEX);
	UINT offset = 0;
	device->GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh->pVertexBuffer, &stride, &offset);
	//�}�e���A���̐������A���ꂼ��̃}�e���A���̃C���f�b�N�X�o�b�t�@�|��`��
	for (DWORD i = 0; i<m_dwNumMaterial; i++)
	{
		//�g�p����Ă��Ȃ��}�e���A���΍�
		if (m_pMaterial[i].dwNumFace == 0)
		{
			continue;
		}
		//�C���f�b�N�X�o�b�t�@�[���Z�b�g
		stride = sizeof(int);
		offset = 0;
		device->GetDeviceContext()->IASetIndexBuffer(pMesh->ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);

		//�e�N�X�`���[���V�F�[�_�[�ɓn��
		if (m_pMaterial[i].szTextureName[0] != NULL)
		{
			//device->GetDeviceContext()->PSSetSamplers(0, 1, &_texsmpler->GetTextureSampler());//�X���b�g�͂O��
			//device->GetDeviceContext()->PSSetShaderResources(0, 1, &m_pMaterial[i].pTexture);
		}
		//�v���~�e�B�u�������_�����O
		device->GetDeviceContext()->DrawIndexed(m_pMaterial[i].dwNumFace * 3, 0, 0);
	}
}

