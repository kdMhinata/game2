#include "ResourceFactory.h"

const std::shared_ptr<KdModelData> ResourceFactory::GetModelData(const std::string& fileName)
{
	// ���X�g���̃L�[�Ō���
	//std::unordered_map<std::string,std::shared_ptr<KdModelData>>::iterator
		//foundItr = m_modelMap.find(fileName);
	auto foundItr = m_modelMap.find(fileName);
	// �������X�g�̒��ɂ���
	if (foundItr != m_modelMap.end())
	{
		// �������f�[�^��Ԃ�
		return (*foundItr).second;	// second�c2�ڂ̗v�f(���f���f�[�^)��Ԃ�
	}

	// �������X�g���ɂȂ��ꍇ
	std::shared_ptr<KdModelData> newModelData = std::make_shared<KdModelData>();

	// ���f����V�K�œǂݍ���
	if (newModelData->Load(fileName))
	{
		// ���X�g�ǉ�
		// pair�^��2�̗v�f������
		/*m_modelMap.insert(std::pair<std::string,
		  std::shared_ptr<KdModelData>>(fileName, newModelData));*/
		m_modelMap[fileName] = newModelData;
		// �ǂݍ��񂾃f�[�^��Ԃ�
		return newModelData;
	}

	// �ǂݍ��݂����s
	std::string errorMsg = "ResourceFactory::GetModelData() ModelLoad Error" + fileName;

	assert(0 && errorMsg.c_str());

	return nullptr;
}

void ResourceFactory::Release()
{
	m_modelMap.clear();
}
