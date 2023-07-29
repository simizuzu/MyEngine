#pragma once

#include <string>
#include <vector>
#include "Vector3.h"

// ���x���f�[�^
struct LevelData {

	struct ObjectData {
		// �t�@�C����
		std::string fileName;
		// ���s�ړ�
		MyMath::Vector3 translation;
		// ��]�p
		MyMath::Vector3 rotation;
		// �X�P�[�����O
		MyMath::Vector3 scaling;
	};

	// �I�u�W�F�N�g�z��
	std::vector<ObjectData> objects;
};

// ���x���f�[�^�̃��[�_�[
class LevelLoader {

public:// �萔
	// �f�t�H���g�̓ǂݍ��݃f�B���N�g��
	static const std::string kDefaultBaseDirectory;
	// �t�@�C���g���q
	static const std::string kExtension;

public:// �����o�֐�

	/// <summary>
	/// ���x���f�[�^�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	static LevelData* LoadFile(const std::string& fileName);
};
