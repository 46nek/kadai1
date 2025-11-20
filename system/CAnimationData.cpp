#include "CAnimationData.h"
#include <iostream>

const aiScene* CAnimationData::LoadAnimation(const std::string filename, const std::string name)
{
    // 新しいImporterを動的に確保
    auto importer = std::make_unique<Assimp::Importer>();

    // 読み込み設定 (左手系への変換など)
    const aiScene* scene = importer->ReadFile(
        filename.c_str(),
        aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_LimitBoneWeights | aiProcess_GenNormals
    );

    // エラーチェック
    if (scene == nullptr) {
        std::cout << "[Error] Animation Load Failed: " << filename << "\n"
            << "Reason: " << importer->GetErrorString() << std::endl;
        return nullptr;
    }

    // 辞書にシーンポインタを登録
    m_Animation[name] = scene;

    // Importerの所有権をベクターに移して、メモリが解放されないようにする
    m_importers.push_back(std::move(importer));

    return scene;
}

aiAnimation* CAnimationData::GetAnimation(const std::string& name, int idx) {
    // 存在確認 (これがないとクラッシュします)
    auto it = m_Animation.find(name);
    if (it == m_Animation.end()) {
        std::cout << "[Warning] Animation not found: " << name << std::endl;
        return nullptr;
    }

    const aiScene* scene = it->second;

    // インデックスの範囲チェック
    if (idx < 0 || idx >= (int)scene->mNumAnimations) {
        std::cout << "[Warning] Animation index out of bounds: " << idx << std::endl;
        return nullptr;
    }

    return scene->mAnimations[idx];
}