#pragma once


#include <DirectXMath.h>
#include <memory>


//�V�[���p�萔
__declspec(align(16)) struct SCENE_MATRIX {
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX viewproje;
};




class ConstantManager;
struct D3D11_MAPPED_SUBRESOURCE;
class D3D11DeviceAndSwapChainAndContextManager;




class CameraManager {
private:
	/**
	@brief �萔�쐬
	*/
	void CreateConstant(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
public:
	CameraManager();
	virtual ~CameraManager();

	/**
	@brief �J�����쐬
	@param
	@param
	*/
	void CreateCamera(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);

	/**
	* @brief �J�����Ǝˉe�s���Ԃ�
	* @param
	* @param
	*/
	DirectX::XMMATRIX& GetViewProjMat();
	/**
	* @brief �萔��Ԃ�
	* @return 
	*/
	std::shared_ptr<ConstantManager>& GetConstantManager();
	/**
	* @brief �萔�X�V
	*/
	void Update(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);


	D3D11_MAPPED_SUBRESOURCE*& GetAddress();


private:
	DirectX::XMMATRIX _viewProj;//�r���[�e�s��
	DirectX::XMMATRIX _camera;//�J�����s��
	DirectX::XMMATRIX _projection;//�ˉe�s��
	DirectX::XMVECTOR _eye;//�����x�N�g��


	std::shared_ptr<ConstantManager> _constant;
	//!�萔�p
	D3D11_MAPPED_SUBRESOURCE* _address;

	//! �J�����萔
	SCENE_MATRIX _scenecameramat;

};
