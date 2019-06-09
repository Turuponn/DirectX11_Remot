#pragma once

#include <memory>



class D3D11DeviceAndSwapChainAndContextManager;
struct ID3D11Buffer;
struct D3D11_MAPPED_SUBRESOURCE;

class ConstantManager {
public:
	ConstantManager();
	virtual ~ConstantManager();


	/**
	* @brief 定数バッファ作成
	* @param [in] constantstrctsize 1定数あたりのサイズ
	*/
	void CreateConstant(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, const size_t& constantstrctsize);
	/**
	* @brief 定数バッファをマップする
	* @param [out] addres　バッファに関連付けるためのアドレスを入れる
	*/
	void ConstantMap(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, D3D11_MAPPED_SUBRESOURCE* address);
	/**
	* @brief 定数バッファをアンマップする
	*/
	void ConstantUnMap(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
	/**
	* @brief 定数バッファを返す
	* @return 
	* @Note 
	*/
	ID3D11Buffer*& GetCbuff();



private:
	
	ID3D11Buffer* _constantbuffer;


};
