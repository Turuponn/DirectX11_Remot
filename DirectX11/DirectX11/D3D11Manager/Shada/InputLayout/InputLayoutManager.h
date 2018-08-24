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
	* @brief ���_���C�A�E�g�쐬
	* @param [in] inputlaout VS���C�A�E�g�p�z��
	* @param [in] inputlaoutarrysize �z��̗v�f��
	*/
	void InputLaoutCreate(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, const D3D11_INPUT_ELEMENT_DESC* inputlaout, const unsigned int& inputlaoutarrysize, std::shared_ptr<VertexShadaManager>& vertexshadamanager);

	/**
	* @brief ���_���C�A�E�g��Ԃ�
	*/
	ID3D11InputLayout*& GetInputLayout();

private:
	ID3D11InputLayout* _inputlayout;
	
};
