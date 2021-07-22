#include "GameObject.h"

void GameObject::Draw()
{
	SHADER->m_standardShader.DrawModel(m_modelWork, m_mWorld);
}
