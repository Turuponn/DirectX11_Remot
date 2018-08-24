#include "ObjLoader.h"
#include <stdio.h>
#include <d3d11.h>



#include "D3D11Manager\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContextManager.h"
#include "D3D11Manager\Texture\WICTextureLoderManager.h"
#include "D3D11Manager\TextureSampler\TextureSamplerManager.h"

/*
モデルにエッジ等が入っていた場合インデックスが狂う？
ベイマックスのモデルより木のモデルのほうが頂点が多いが狂わない(ベイマックスは狂う）
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
//マテリアルファイルを読み込む関数
HRESULT ObjLoader::LoadMaterialFromFile(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, OBJ_MATERIAL** ppMaterial)
{
	std::shared_ptr<WICTextureLoderManager> w(new WICTextureLoderManager());

	//マテリアルファイルを開いて内容を読み込む
	FILE* fp = NULL;
	fopen_s(&fp, "resoce/tree/Tree.mtl", "rt");
	char key[110] = { 0 };
	DirectX::XMFLOAT4 v(0, 0, 0, 1);

	//マテリアル数を調べる
	while (!feof(fp))
	{
		//キーワード読み込み
		fscanf_s(fp, "%s ", key, sizeof(key));
		//マテリアル名
		if (strcmp(key, "newmtl") == 0)
		{
			m_dwNumMaterial++;
		}
	}
	OBJ_MATERIAL* pMaterial = new OBJ_MATERIAL[m_dwNumMaterial];

	//本読み込み	
	fseek(fp, SEEK_SET, 0);
	INT iMCount = -1;

	while (!feof(fp))
	{
		//キーワード読み込み
		fscanf_s(fp, "%s ", key, sizeof(key));
		//マテリアル名
		if (strcmp(key, "newmtl") == 0)
		{
			iMCount++;
			fscanf_s(fp, "%s ", key, sizeof(key));
			strcpy_s(pMaterial[iMCount].szName, key);
		}
		//Ka　アンビエント
		if (strcmp(key, "Ka") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			pMaterial[iMCount].ambient = v;
		}
		//Kd　ディフューズ
		if (strcmp(key, "Kd") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			pMaterial[iMCount].diffuse = v;
		}
		//Ks　スペキュラー
		if (strcmp(key, "Ks") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			pMaterial[iMCount].specular = v;
		}
		//map_Kd　テクスチャー
		if (strcmp(key, "map_Kd") == 0)
		{
			fscanf_s(fp, "%s", &pMaterial[iMCount].szTextureName, sizeof(pMaterial[iMCount].szTextureName));

			std::wstring filepaths;
			StringToWString(filepaths, pMaterial[iMCount].szTextureName);
			//テクスチャーを作成
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
	DWORD dwVCount = 0;//読み込みカウンター
	DWORD dwVNCount = 0;//読み込みカウンター
	DWORD dwVTCount = 0;//読み込みカウンター
	DWORD dwFCount = 0;//読み込みカウンター

	char key[200] = { 0 };
	//OBJファイルを開いて内容を読み込む
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	//事前に頂点数、ポリゴン数を調べる
	while (!feof(fp))
	{
		//キーワード読み込み
		fscanf_s(fp, "%s ", key, sizeof(key));
		//マテリアル読み込み
		if (strcmp(key, "mtllib") == 0)
		{
			fscanf_s(fp, "%s ", key, sizeof(key));
			LoadMaterialFromFile(device, &m_pMaterial);
		}
		//頂点
		if (strcmp(key, "v") == 0)
		{
			(*pMesh)->vertexnum++;
		}
		//法線
		if (strcmp(key, "vn") == 0)
		{
			dwVNCount++;
		}
		//テクスチャー座標
		if (strcmp(key, "vt") == 0)
		{
			dwVTCount++;
		}
		//フェイス（ポリゴン）
		if (strcmp(key, "f") == 0)
		{
			(*pMesh)->faceindexnum++;
		}
	}

	//一時的なメモリ確保（頂点バッファとインデックスバッファ）
	OBJ_VERTEX* pvVertexBuffer = new OBJ_VERTEX[(*pMesh)->faceindexnum * 3];
	DirectX::XMFLOAT3* pvCoord = new DirectX::XMFLOAT3[(*pMesh)->vertexnum];
	DirectX::XMFLOAT3* pvNormal = new DirectX::XMFLOAT3[dwVNCount];
	DirectX::XMFLOAT2* pvTexture = new DirectX::XMFLOAT2[dwVTCount];

	//本読み込み	
	fseek(fp, SEEK_SET, 0);
	dwVCount = 0;
	dwVNCount = 0;
	dwVTCount = 0;
	dwFCount = 0;

	while (!feof(fp))
	{
		//キーワード 読み込み
		ZeroMemory(key, sizeof(key));
		fscanf_s(fp, "%s ", key, sizeof(key));

		//頂点 読み込み
		if (strcmp(key, "v") == 0)
		{
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			pvCoord[dwVCount].x = -x;//OBJは右手座標系なのでxあるいはｚを反転
			pvCoord[dwVCount].y = y;
			pvCoord[dwVCount].z = z;
			dwVCount++;
		}

		//法線 読み込み
		if (strcmp(key, "vn") == 0)
		{
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			pvNormal[dwVNCount].x = -x;//OBJは右手座標系なのでxあるいはｚを反転
			pvNormal[dwVNCount].y = y;
			pvNormal[dwVNCount].z = z;
			dwVNCount++;
		}

		//テクスチャー座標 読み込み
		if (strcmp(key, "vt") == 0)
		{
			fscanf_s(fp, "%f %f", &x, &y);
			pvTexture[dwVTCount].x = x;
			pvTexture[dwVTCount].y = 1 - y;//OBJファイルはY成分が逆なので合わせる
			dwVTCount++;
		}
	}

	//マテリアルの数だけインデックスバッファーを作成
	(*pMesh)->ppIndexBuffer = new ID3D11Buffer*[m_dwNumMaterial];

	//フェイス　読み込み　バラバラに収録されている可能性があるので、マテリアル名を頼りにつなぎ合わせる
	bool boFlag = false;
	int* piFaceBuffer = new int[(*pMesh)->faceindexnum * 3];//３頂点ポリゴンなので、1フェイス=3頂点(3インデックス)
	dwFCount = 0;
	DWORD dwPartFCount = 0;
	for (DWORD i = 0; i<m_dwNumMaterial; i++)
	{
		dwPartFCount = 0;
		fseek(fp, SEEK_SET, 0);

		while (!feof(fp))
		{
			//キーワード 読み込み
			ZeroMemory(key, sizeof(key));
			fscanf_s(fp, "%s ", key, sizeof(key));

			//フェイス 読み込み→頂点インデックスに
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
				if (m_pMaterial[i].pTexture != NULL)//テクスチャーありサーフェイス
				{
					fscanf_s(fp, "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
				}
				else//テクスチャー無しサーフェイス
				{
					fscanf_s(fp, "%d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3);
				}

				//インデックスバッファー
				piFaceBuffer[dwPartFCount * 3] = dwFCount * 3;
				piFaceBuffer[dwPartFCount * 3 + 1] = dwFCount * 3 + 1;
				piFaceBuffer[dwPartFCount * 3 + 2] = dwFCount * 3 + 2;
				//頂点構造体に代入
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
		if (dwPartFCount == 0)//使用されていないマテリアル対策
		{
			(*pMesh)->ppIndexBuffer[i] = NULL;
			continue;
		}

		//インデックスバッファーを作成
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

	//バーテックスバッファーを作成
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

	//一時的な入れ物は、もはや不要
	delete pvCoord;
	delete pvNormal;
	delete pvTexture;
	delete[] pvVertexBuffer;


	return S_OK;
}


//シーンを画面にレンダリング
void ObjLoader::Render(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, OBJ_MESH* pMesh)
{

	
	//バーテックスバッファーをセット
	UINT stride = sizeof(OBJ_VERTEX);
	UINT offset = 0;
	device->GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh->pVertexBuffer, &stride, &offset);
	//マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
	for (DWORD i = 0; i<m_dwNumMaterial; i++)
	{
		//使用されていないマテリアル対策
		if (m_pMaterial[i].dwNumFace == 0)
		{
			continue;
		}
		//インデックスバッファーをセット
		stride = sizeof(int);
		offset = 0;
		device->GetDeviceContext()->IASetIndexBuffer(pMesh->ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);

		//テクスチャーをシェーダーに渡す
		if (m_pMaterial[i].szTextureName[0] != NULL)
		{
			//device->GetDeviceContext()->PSSetSamplers(0, 1, &_texsmpler->GetTextureSampler());//スロットは０で
			//device->GetDeviceContext()->PSSetShaderResources(0, 1, &m_pMaterial[i].pTexture);
		}
		//プリミティブをレンダリング
		device->GetDeviceContext()->DrawIndexed(m_pMaterial[i].dwNumFace * 3, 0, 0);
	}
}

