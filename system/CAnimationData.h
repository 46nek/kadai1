#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include "NonCopyable.h"

class CAnimationData : public NonCopyable {
private:
    // アニメーションデータへのポインタ辞書
    std::unordered_map<std::string, const aiScene*> m_Animation;

    // Importerの実体を保持するコンテナ
    // ここに保持しておかないとaiSceneのポインタが無効になります
    std::vector<std::unique_ptr<Assimp::Importer>> m_importers;

public:
    // コンストラクタ・デストラクタ
    CAnimationData() = default;
    ~CAnimationData() = default;

    // ロード関数
    const aiScene* LoadAnimation(const std::string filename, const std::string name);

    // 取得関数
    aiAnimation* GetAnimation(const std::string& name, int idx);
};