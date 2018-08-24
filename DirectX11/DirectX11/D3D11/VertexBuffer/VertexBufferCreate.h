#pragma once


struct ID3D11Device;
struct ID3D11Buffer;


class VertexBufferCreate {
public:
	VertexBufferCreate();
	virtual ~VertexBufferCreate();

	/**
	* @brief 頂点バッファ作成
	* @param [out] vertexbuffer 頂点バッファを返す
	* @param [in] vertexdata 頂点データを入れる
	* @detail  buffersize = onevertexsize*allvertexarrysize
	*/
	void CreateVertexBuffer(ID3D11Device*& device, const int& buffersize, void* vertexdata, ID3D11Buffer** vertexbuffer);


private:
};