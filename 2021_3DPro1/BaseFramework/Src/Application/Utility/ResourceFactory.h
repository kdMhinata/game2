#pragma once

class ResourceFactory
{
public:
	ResourceFactory() {}
	~ResourceFactory() { Release(); }

	const std::shared_ptr<KdModelData> GetModelData(const std::string& fileName);

private:
	void Release();

	// ������ƃ��f���f�[�^���i�[
	std::unordered_map<std::string, std::shared_ptr<KdModelData>> m_modelMap;
};
