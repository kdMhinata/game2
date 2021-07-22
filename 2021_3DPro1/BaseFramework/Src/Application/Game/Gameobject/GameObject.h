#pragma once

class GameObject
{
public:

	enum classID
	{
		eBase,
		eStage,
		ePlayer,
		eEnemy
	};

	GameObject() {}
	virtual ~GameObject() { Release(); }

	virtual void Init() {}
	virtual void Update() {}
	void Draw();

	bool IsAlive() { return m_isAlive; }

	virtual const Math::Vector3 GetPos() const { return m_mWorld.Translation(); }

	virtual classID GetClassID() const { return eBase; }

	// åpè≥ÇµÇΩÇ‡ÇÃÇÃÇ›êGÇÍÇÈ 
protected:

	KdModelWork		m_modelWork;
	Math::Matrix	m_mWorld;

	bool			m_isAlive = true;

	float			m_radius = 0.0f;

private:
	void Release() {}

};
