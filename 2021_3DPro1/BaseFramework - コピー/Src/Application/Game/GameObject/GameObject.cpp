#include "GameObject.h"

void GameObject::Draw()
{
	SHADER->m_standardShader.DrawModel(m_modelWork, m_mWorld);
}

bool GameObject::CheckCollisionBump(const SphereInfo& info, BumpResult& result)
{
	Math::Vector3 betweenVec = info.m_pos - m_mWorld.Translation();

	// 2点間の距離
	float distanceSqr = betweenVec.LengthSquared();

	// 当たり判定の半径合計
	float hitRadius = m_radius + info.m_radius;

	// 判定の結果
	result.m_isHit = (distanceSqr <= (hitRadius * hitRadius));

	if (result.m_isHit)
	{
		// 押し戻し
		result.m_pushVec = betweenVec;
		result.m_pushVec.Normalize();

		float distance = std::sqrt(distanceSqr);

		// 押し戻すベクトル×押し戻す値
		result.m_pushVec *= hitRadius - distance;
	}
	return result.m_isHit;
}

bool GameObject::CheckCollisionBump(const RayInfo& info, BumpResult& result)
{
	KdRayResult localResult;

	float minDist = FLT_MAX;

	for (UINT i = 0; i < m_modelWork.GetDataNodes().size(); i++) 
	{
		const KdModelData::Node& dataNode=m_modelWork.GetDataNodes()[i];
		const KdModelWork::Node& workNode=m_modelWork.GetNodes()[i];

	/*	result.m_isHit = KdRayToMesh(info.m_pos, info.m_dir, info.m_range, //発射位置、方向、判定する長さ、相手のメッシュ情報、相手の行列、出力結果
			*(m_modelWork.GetMesh(0).get()), m_mWorld, &localResult); */

		KdRayToMesh(
			info.m_pos,
			info.m_dir,
			info.m_range,
			*(dataNode.m_spMesh.get()),
			workNode.m_worldTransform * m_mWorld,
			&localResult);

		if (localResult.m_hit&&localResult.m_distance<minDist)
		{
			result.m_pushVec = info.m_dir * (localResult.m_distance - info.m_range);

			minDist = localResult.m_distance;
		}

	}

	return result.m_isHit;
}
