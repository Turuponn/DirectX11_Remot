#pragma once


#include <memory>

struct ID3D11Buffer;
class D3D11DeviceAndSwapChainAndContextManager;

class VertexBufferManager {
public:
	VertexBufferManager();
	virtual ~VertexBufferManager();

	/**
	* @brief 頂点バッファ作成
	* @param [in] vertexdata 頂点データ
	* @param [in] onevertexsize 一頂点あたりの頂点サイズ
	* @param [in] vertexarrysize 頂点配列の要素数
	* @detail  buffersize = onevertexsize*vertexarrysize 
	*/
	void CreateVertexBuffer(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, void* vertexdata, const size_t& onevertexsize, const int& vertexarrysize);
	/**
	* @brief 頂点バッファを返す
	* @return 
	*/
	ID3D11Buffer*& GetVertexBuffer();

private:
	
	ID3D11Buffer* _vertexbuffer;

};