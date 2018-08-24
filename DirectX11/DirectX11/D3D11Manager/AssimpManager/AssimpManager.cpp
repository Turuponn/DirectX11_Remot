#include "AssimpManager.h"
#include "D3D11Manager\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContextManager.h"
#include <MicrosoftLiblary\WICTextureLoader\WICTextureLoader.h>
using namespace std;
using namespace DirectX;
/*
参考URL:
http://assimp.sourceforge.net/lib_html/usage.html
サンプルではディフューズしか対応してないとのこと
https://github.com/tek-nishi/SkeletalAnimation　・・・アニメーションまでさせてるサンプル
http://blog.livedoor.jp/tek_nishi/archives/9271123.html　・・・唯一日本語のサイト
*/

AssimpManager::AssimpManager() {

}
AssimpManager::~AssimpManager() {
	Close();
}

void AssimpManager::SetDir(const std::string& filepath) {
	std::string workstr;
	workstr = filepath.substr(0, filepath.find_last_of("/") + 1);
	directory = workstr;
}

bool AssimpManager::import(const std::string& PFILE, std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {
	printf("\n");
	//インポータの定義
	Assimp::Importer importer;
	//相対パス用のパスを保存
	SetDir(PFILE);


	const aiScene* scene = importer.ReadFile(
		PFILE,
		aiProcess_CalcTangentSpace | // 法線計算 
		aiProcess_Triangulate | //三角形化？
		aiProcess_JoinIdenticalVertices | // 
		aiProcess_SortByPType
	);
	if (!scene) {
		// エラー情報と対応する処理を得るためのインポートエラー
		printf("Error!:   %s\n",importer.GetErrorString());
		return false;
	}

	//DoTheSceneProcessing(scene); //これはいつの間にか使えなくなっている　自前で実装する必要がある
	ProcessNode(device,scene->mRootNode, scene);


	return true;

}
void AssimpManager::ProcessNode(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device,aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(this->ProcessMesh(device,mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(device,node->mChildren[i], scene);
	}
}
Mesh AssimpManager::ProcessMesh(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device,aiMesh*  mesh, const aiScene* scene)
{
	// Data to fill
	vector<VERTEX> vertices;
	vector<UINT> indices;
	vector<Texture> textures;

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

		if (textype.empty()) textype = determineTextureType(scene, mat);
	}

	//頂点情報取得
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		VERTEX vertex;

		//頂点座標
		vertex.X = mesh->mVertices[i].x;
		vertex.Y = mesh->mVertices[i].y;
		vertex.Z = mesh->mVertices[i].z;


		//UV
		if (mesh->mTextureCoords[0])
		{
			vertex.texcoord.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.texcoord.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}
	
	//頂点インデックス情報取得
	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	//マテリアル情報取得
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		vector<Texture> diffuseMaps = this->loadMaterialTextures(device,material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}

	return Mesh(device->GetDevice(), vertices, indices, textures);
}
vector<Texture> AssimpManager::loadMaterialTextures(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device,aiMaterial* mat, aiTextureType type, string typeName, const aiScene* scene)
{
	vector<Texture> textures;
	for (UINT i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (UINT j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.c_str(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // If texture hasn't been loaded already, load it
			HRESULT hr;
			Texture texture;
			if (textype == "embedded compressed texture")
			{
				int textureindex = getTextureIndex(&str);
				texture.texture = getTextureFromModel(device,scene, textureindex);
			}
			else
			{
				string filename = string(str.C_Str());
				filename = directory + filename;
				wstring filenamews = wstring(filename.begin(), filename.end());//これでstring->wstringに変更可能
				//Note: パスの抽出精度がよすぎて　製作者さんのパスそのまま持ってくる仕様っぽい->制作者さんがテクスチャのパスを絶対パスで登録している？
				//WICはTGA形式は対応していない ＴＯＤＯ：　あとでそのへんつくる
				hr = CreateWICTextureFromFile(device->GetDevice(), device->GetDeviceContext(), filenamews.c_str(), nullptr, &texture.texture);
				if (FAILED(hr)) {
					throw(1);
				}
			}
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

ID3D11ShaderResourceView* AssimpManager::getTextureFromModel(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device,const aiScene * scene, int textureindex)
{
	HRESULT hr;
	ID3D11ShaderResourceView* texture;

	int* size = reinterpret_cast<int*>(&scene->mTextures[textureindex]->mWidth);

	hr = CreateWICTextureFromMemory(device->GetDevice(), device->GetDeviceContext(), reinterpret_cast<unsigned char*>(scene->mTextures[textureindex]->pcData), *size, nullptr, &texture);
	if (FAILED(hr)) {
		throw(1);
	}
		

	return texture;
}

int AssimpManager::getTextureIndex(aiString * str)
{
	string tistr;
	tistr = str->C_Str();
	tistr = tistr.substr(1);
	return stoi(tistr);
}

string AssimpManager::determineTextureType(const aiScene * scene, aiMaterial * mat)
{
	aiString textypeStr;
	mat->GetTexture(aiTextureType_DIFFUSE, 0, &textypeStr);
	string textypeteststr = textypeStr.C_Str();
	if (textypeteststr == "*0" || textypeteststr == "*1" || textypeteststr == "*2" || textypeteststr == "*3" || textypeteststr == "*4" || textypeteststr == "*5")
	{
		if (scene->mTextures[0]->mHeight == 0)
		{
			return "embedded compressed texture";
		}
		else
		{
			return "embedded non-compressed texture";
		}
	}
	if (textypeteststr.find('.') != string::npos)
	{
		return "textures are on disk";
	}
	return "null";
}
void AssimpManager::Close()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		//meshes[i].Close();
	}
}
void AssimpManager::Draw(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device)
{
	for (int i = 0; i < meshes.size(); i++)
	{
		//meshes[i].Draw(device->GetDeviceContext());
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;

		/*device->GetDeviceContext()->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
		device->GetDeviceContext()->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);*/

		//devcon->PSSetShaderResources(0, 1, &textures[0].texture);
		device->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//device->GetDeviceContext()->DrawIndexed(indices.size(), 0, 0);
	}
}

void AssimpManager::LoadFile(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device,const std::string& filepath) {
	if (import(filepath, device) != true) {
		throw(1);
	}
}

void AssimpManager::SetupMesh(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device)
{
	HRESULT hr = E_FAIL;

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	//vbd.ByteWidth = sizeof(VERTEX) * vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	//initData.pSysMem = &vertices[0];

	hr = device->GetDevice()->CreateBuffer(&vbd, &initData, &_vertexBuffer);
	if (hr != S_OK) {
		throw(1);
	}
		
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	//ibd.ByteWidth = sizeof(UINT) * indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;

	//initData.pSysMem = &indices[0];

	hr = device->GetDevice()->CreateBuffer(&ibd, &initData, &_indexBuffer);
	if (hr != S_OK) {
		throw(1);
	}
}