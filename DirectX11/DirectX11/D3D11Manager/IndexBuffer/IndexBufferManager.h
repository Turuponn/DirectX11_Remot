#pragma once

#include <vector>
#include <memory>


class IndexBuffer;
struct ID3D11Buffer;
class D3D11DeviceAndSwapChainAndContextManager;

class IndexBufferManager {
public:
	IndexBufferManager();
	virtual ~IndexBufferManager();

	/// <summary>
	/// インデックスバッファをサイズ分作成する
	/// </summary>
	/// <param name="device"></param>
	/// <param name="buffsize"> バッファの全サイズを入れる</param>
	/// <param name="fecedata"> インデックス用のデータをすべて入れる</param>
	void CreateIndexBuffer(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, const unsigned int buffsize, void* fecedata);
	/// <summary>
	/// 
	/// </summary>
	/// <returns> インデックスバッファを返す </returns>
	ID3D11Buffer*& GetIndexBuffer();
private:
	ID3D11Buffer* _indexbuffer;
};
