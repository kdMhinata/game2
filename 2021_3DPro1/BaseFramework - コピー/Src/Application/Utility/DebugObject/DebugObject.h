#pragma once

class DebugObject
{
public:
	~DebugObject() { Release(); }

	//�f�o�b�O���C�����̓o�^
	void AddDebugLine(const Math::Vector3& p1, const Math::Vector3& p2,
		const Math::Color& color = { 1.0f,1.0f,1.0f,1.0f });

	//�f�o�b�O�X�t�B�A���o�^
	void AddDebugSphere(const Math::Vector3& pos, float radius, const Math::Color& color = { 1.0f,1.0f,1.0f,1.0f });

	//�o�^���ꂽ���ׂẴf�o�b�O����`��
	void DrawDebug();
private:
	void Release();

	//�f�o�b�O�`��p�̒��_�z��
	std::vector<KdEffectShader::Vertex> m_debugVertex;
};
