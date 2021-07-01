#pragma once

class StageMap
{
public:
	StageMap();
	~StageMap();

	void Init();
	void Update();
	void Draw();

private:
	void Release();
	KdModelWork m_modelWork;

	KdModelWork m_stage;
	DirectX::SimpleMath::Matrix m_stageMat;
};