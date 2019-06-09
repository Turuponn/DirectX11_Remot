#pragma once


#include <DirectXMath.h>
#include <memory>


//シーン用定数
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
	@brief 定数作成
	*/
	void CreateConstant(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);
public:
	CameraManager();
	virtual ~CameraManager();

	/**
	@brief カメラ作成
	@param
	@param
	*/
	void CreateCamera(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);

	/**
	* @brief カメラと射影行列を返す
	* @param
	* @param
	*/
	DirectX::XMMATRIX& GetViewProjMat();
	/**
	* @brief 定数を返す
	* @return 
	*/
	std::shared_ptr<ConstantManager>& GetConstantManager();
	/**
	* @brief 定数更新
	*/
	void Update(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);


	D3D11_MAPPED_SUBRESOURCE*& GetAddress();


private:
	DirectX::XMMATRIX _viewProj;//ビュー影行列
	DirectX::XMMATRIX _camera;//カメラ行列
	DirectX::XMMATRIX _projection;//射影行列
	DirectX::XMVECTOR _eye;//視線ベクトル


	std::shared_ptr<ConstantManager> _constant;
	//!定数用
	D3D11_MAPPED_SUBRESOURCE* _address;

	//! カメラ定数
	SCENE_MATRIX _scenecameramat;

};
