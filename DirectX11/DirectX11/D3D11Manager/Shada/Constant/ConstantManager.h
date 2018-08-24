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
	* @brief �萔�o�b�t�@�쐬
	* @param [in] constantstrctsize 1�萔������̃T�C�Y
	*/
	void CreateConstant(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, const size_t& constantstrctsize);
	/**
	* @brief �萔�o�b�t�@���}�b�v����
	* @param [out] addres�@�o�b�t�@�Ɋ֘A�t���邽�߂̃A�h���X������
	*/
	void ConstantMap(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, D3D11_MAPPED_SUBRESOURCE* address);
	/**
	* @brief �萔�o�b�t�@���A���}�b�v����
	*/
	void ConstantUnMap(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
	/**
	* @brief �萔�o�b�t�@��Ԃ�
	* @return 
	* @Note Dx12�ł̓o�b�t�@��Ԃ��Ȃ��Ă����[�g�V�O�l�`���Ŋ������Ă��B�@Dx11�ł͂ǂ̒萔���g���̂����t���[���K�v
	*/
	ID3D11Buffer*& GetCbuff();



private:
	
	ID3D11Buffer* _constantbuffer;


};
