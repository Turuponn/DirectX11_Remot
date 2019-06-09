#pragma once



#include <DirectXMath.h>


struct PrimitivVertex {
	//IA�X�e�[�W�p�̃Z�}���e�B�N�X���C�A�E�g
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 color;
	

	PrimitivVertex() {
		pos = DirectX::XMFLOAT3(0, 0, 0);
		normal = DirectX::XMFLOAT3(0, 0, 0);
		uv = DirectX::XMFLOAT2(0, 0);
		color = DirectX::XMFLOAT3(0, 0, 0);
		
	}

	PrimitivVertex(DirectX::XMFLOAT3& p, DirectX::XMFLOAT3& norm, DirectX::XMFLOAT2& coord) {
		pos = p;
		normal = norm;
		uv = coord;
	}
	PrimitivVertex(float x, float y, float z, float nomal_x, float nomal_y, float nomal_z, float u, float v) {
		pos.x = x;
		pos.y = y;
		pos.z = z;
		normal.x = nomal_x;
		normal.y = nomal_y;
		normal.z = nomal_z;
		uv.x = u;
		uv.y = v;
	}
};

struct PrimitivConstant {
	DirectX::XMMATRIX wvp;
};
