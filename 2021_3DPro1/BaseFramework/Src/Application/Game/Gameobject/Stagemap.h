#pragma once

class StageMap
{
public:
	StageMap();
	~StageMap();

	void Init();
	void Updata();
	void Draw();

private:
	void Release();

	KdModelWork m_stage;
	DirectX::SimpleMath::Matrix m_stageMat;
};