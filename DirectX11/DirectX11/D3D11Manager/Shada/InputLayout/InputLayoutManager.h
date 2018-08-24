#pragma once


#include <memory>

class D3D11DeviceAndSwapChainAndContextManager;
struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;
class VertexShadaManager;


class InputLayoutManager {
public:
	InputLayoutManager();
	virtual ~InputLayoutManager();


	/**
	* @brief 頂点レイアウト作成
	* @param [in] inputlaout VSレイアウト用配列
	* @param [in] inputlaoutarrysize 配列の要素数
	*/
	void InputLaoutCreate(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, const D3D11_INPUT_ELEMENT_DESC* inputlaout, const unsigned int& inputlaoutarrysize, std::shared_ptr<VertexShadaManager>& vertexshadamanager);

	/**
	* @brief 頂点レイアウトを返す
	*/
	ID3D11InputLayout*& GetInputLayout();

private:
	ID3D11InputLayout* _inputlayout;
	
};
