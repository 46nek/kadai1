#include	"player.h"	
#include    "../system/CDirectInput.h"
#include    "../system/Renderer.h"

void player::init() {

    // 1. メッシュ（モデル形状）のロード (Akai.fbx)
    m_mesh = std::make_unique<CAnimationMeshBlender>();
    m_mesh->Load("assets/model/akai/Akai.fbx", "assets/model/akai");


    // 2. アニメーションデータのロード
    // (1) Idleアニメーション
    m_animData.LoadAnimation("assets/model/akai/Akai_Idle.fbx", "IdleAnim");
    m_pAnimIdle = m_animData.GetAnimation("IdleAnim", 0);

    // (2) Runアニメーション (追加)
    m_animData.LoadAnimation("assets/model/akai/Akai_run.fbx", "RunAnim");
    m_pAnimRun = m_animData.GetAnimation("RunAnim", 0);

    // (3) ブレンド設定
    if (m_pAnimIdle && m_pAnimRun) {
        // 初期状態は Idle (0.0) に設定
        // From に Idle、To に Run をセットしておく
        m_mesh->SetFromAnimation(m_pAnimIdle);
        m_mesh->SetToAnimation(m_pAnimRun);

        // 初期ブレンド率は0 (Idle側)
        m_AnimBlendRate = 0.0f;
        m_mesh->SetBlendrate(m_AnimBlendRate);
    }


    // 3. アニメーション制御オブジェクトの初期化
    m_model = std::make_unique<CAnimationObject>();
    m_model->Init();

    m_model->SetAnimationMesh(m_mesh.get());


    // 4. シェーダー作成
    m_shader = std::make_unique<CShader>();
    m_shader->Create("shader/vertexLightingOneSkinVS.hlsl", "shader/vertexLightingPS.hlsl");

    m_srt.pos = Vector3(0, 0, 0);
    m_srt.scale = Vector3(1.0f, 1.0f, 1.0f);
    m_srt.rot = Vector3(0, 0, 0);
    m_destrot = Vector3(0, 0, 0);

    m_moveTimeCount = 0.0f;
    m_AnimBlendRate = 0.0f;
    m_oldTime = 0;
}

void player::update(uint64_t dt) {

    // --- キー入力判定 ---
    bool isMoving = false;

    if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W) ||
        CDirectInput::GetInstance().CheckKeyBuffer(DIK_A) ||
        CDirectInput::GetInstance().CheckKeyBuffer(DIK_S) ||
        CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
    {
        isMoving = true;
    }

    if (isMoving) {
        // 移動中なら時間を進める
        m_moveTimeCount += (float)dt;

        // ★変更: わかりやすく「2秒（2000ms）」かけて加速させる
        if (m_moveTimeCount >= 2000.0f) {
            speedRate = 1.0f;
        }
        else {
            // 0.0(停止) から 1.0(最大) へ徐々に増やす
            speedRate = m_moveTimeCount / 2000.0f;
        }
    }
    else {
        m_moveTimeCount = 0.0f;
        speedRate = 0.0f;
    }


    // --- キー入力による移動処理 ---

    if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_A)) {
        if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
        {// 左前
            float radian = PI * 0.75f;
            // ここに * speedRate を追加
            m_move.x -= sinf(radian) * VALUE_MOVE_MODEL * speedRate;
            m_move.z -= cosf(radian) * VALUE_MOVE_MODEL * speedRate;
            m_destrot.y = radian;
        }
        else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
        {// 左後
            float radian = PI * 0.25f;
            m_move.x -= sinf(radian) * VALUE_MOVE_MODEL * speedRate;
            m_move.z -= cosf(radian) * VALUE_MOVE_MODEL * speedRate;
            m_destrot.y = radian;
        }
        else
        {// 左
            float radian = PI * 0.50f;
            m_move.x -= sinf(radian) * VALUE_MOVE_MODEL * speedRate;
            m_move.z -= cosf(radian) * VALUE_MOVE_MODEL * speedRate;
            m_destrot.y = radian;
        }
    }
    else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
    {
        if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W)) {
            // 右前
            float radian = -PI * 0.75f;
            m_move.x -= sinf(radian) * VALUE_MOVE_MODEL * speedRate;
            m_move.z -= cosf(radian) * VALUE_MOVE_MODEL * speedRate;
            m_destrot.y = radian;
        }
        else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
        {// 右後
            float radian = -PI * 0.25f;
            m_move.x -= sinf(radian) * VALUE_MOVE_MODEL * speedRate;
            m_move.z -= cosf(radian) * VALUE_MOVE_MODEL * speedRate;
            m_destrot.y = radian;
        }
        else
        {// 右
            float radian = -PI * 0.50f;
            m_move.x -= sinf(radian) * VALUE_MOVE_MODEL * speedRate;
            m_move.z -= cosf(radian) * VALUE_MOVE_MODEL * speedRate;
            m_destrot.y = radian;
        }
    }
    else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
    {// 前
        float radian = PI;
        m_move.x -= sinf(radian) * VALUE_MOVE_MODEL * speedRate;
        m_move.z -= cosf(radian) * VALUE_MOVE_MODEL * speedRate;
        m_destrot.y = radian;
    }
    else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
    {// 後
        float radian = 0.0f;
        m_move.x -= sinf(radian) * VALUE_MOVE_MODEL * speedRate;
        m_move.z -= cosf(radian) * VALUE_MOVE_MODEL * speedRate;
        m_destrot.y = 0.0f;
    }

    // --- 回転制御 ---
    if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT)) {
        m_destrot.y = m_srt.rot.y - VALUE_ROTATE_MODEL;
        if (m_destrot.y < -PI) m_destrot.y += PI * 2.0f;
    }

    if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT)) {
        m_destrot.y = m_srt.rot.y + VALUE_ROTATE_MODEL;
        if (m_destrot.y > PI) m_destrot.y -= PI * 2.0f;
    }

    float diffrot = m_destrot.y - m_srt.rot.y;
    if (diffrot > PI) diffrot -= PI * 2.0f;
    if (diffrot < -PI) diffrot += PI * 2.0f;

    m_srt.rot.y += diffrot * RATE_ROTATE_MODEL;
    if (m_srt.rot.y > PI) m_srt.rot.y -= PI * 2.0f;
    if (m_srt.rot.y < -PI) m_srt.rot.y += PI * 2.0f;

    // 位置移動と慣性
    m_srt.pos += m_move;
    m_move += -m_move * RATE_MOVE_MODEL;

    if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RETURN)) {
        m_srt.pos = Vector3(0.0f, 0.0f, 0.0f);
        m_srt.rot = Vector3(0.0f, 0.0f, 0.0f);
    }

    // --- アニメーションの更新 ---
    if (isMoving) {
        m_AnimBlendRate = 1.0f;
    }
    else {
        m_AnimBlendRate = 0.0f;
    }

    if (m_mesh) {
        m_mesh->SetBlendrate(m_AnimBlendRate);
    }

    // アニメーション速度の更新
    if (m_model) {
        float currentAnimSpeed = (1.0f - m_AnimBlendRate) * SPEED_RATE_IDLE
            + m_AnimBlendRate * SPEED_RATE_RUN;

        float deltaSeconds = (float)dt / 1000.0f;

        m_model->BlendUpdate(deltaSeconds * currentAnimSpeed);
    }
}

void player::draw(uint64_t dt) {

    Matrix4x4 mtx = m_srt.GetMatrix();
    Renderer::SetWorldMatrix(&mtx);

    if (m_shader) {
        m_shader->SetGPU();
    }

    if (m_model) {
        m_model->Draw();
    }
}

void player::dispose() {
}