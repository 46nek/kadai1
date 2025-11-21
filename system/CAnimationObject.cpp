#include	"CAnimationObject.h"
#include    <cmath> // fmodを使うために必要かも

void CAnimationObject::Init()
{
    m_BoneCombMatrix.Create();
}

void CAnimationObject::Update(float dt)
{
    // 1. データから正しいTickレートを取得する
    double tps = m_AnimMesh->GetTicksPerSecond();
    // データに設定がない(0.0)場合は24.0などを仮定する
    float ticksPerSecond = (tps != 0.0) ? (float)tps : 24.0f;

    // 時間経過をTick単位に変換して加算
    m_CurrentFrame += dt * ticksPerSecond;

    // 2. データから正しい長さを取得してループさせる
    double dur = m_AnimMesh->GetDuration();
    float duration = (float)dur;

    // duration が有効な値ならループ処理を行う
    if (duration > 0.0f) {
        m_CurrentFrame = fmod(m_CurrentFrame, duration);
    }

    int frame = static_cast<int>(m_CurrentFrame);

    // アニメーションメッシュ更新
    m_AnimMesh->Update(m_BoneCombMatrix, frame);
}

void CAnimationObject::BlendUpdate(float dt)
{
    // Update関数と同様の修正を行います

    // 1. Tickレート取得
    double tps = m_AnimMesh->GetTicksPerSecond();
    float ticksPerSecond = (tps != 0.0) ? (float)tps : 24.0f;

    // dt は呼び出し元で加工された「経過時間(秒)」なので、ここでフレーム進行量に変換
    m_CurrentFrame += dt * ticksPerSecond;

    // 2. ループ処理の復活
    double dur = m_AnimMesh->GetDuration();
    float duration = (float)dur;

    if (duration > 0.0f) {
        m_CurrentFrame = fmod(m_CurrentFrame, duration);
    }

    int frame = static_cast<int>(m_CurrentFrame);

    // アニメーションメッシュ更新
    m_AnimMesh->UpdateBlended(m_BoneCombMatrix, frame);
}

void CAnimationObject::Draw()
{
    // ボーンコンビネーションを定数バッファへ反映させる
    m_BoneCombMatrix.Update();

    // 定数バッファGPUへセット
    m_BoneCombMatrix.SetGPU();

    // メッシュ描画
    m_AnimMesh->Draw();
}