#include "aimtrainer.hpp"

using namespace cangine;

// ─────────────────────────────────────────────
//  Sabitler
// ─────────────────────────────────────────────

static const int WIN_W = 800;
static const int WIN_H = 600;

static const float MAP_W = 700.0f;
static const float MAP_H = 480.0f;
static const float MAP_X = 0.0f;
static const float MAP_Y = -20.0f;

static const float TARGET_RADIUS = 35.0f;
static const float TARGET_LIFETIME = 3.0f;
static const float SPAWN_INTERVAL = 1.5f;
static const int MAX_TARGETS = 3;

static const int POINTS_1SEC = 10;
static const int POINTS_2SEC = 8;
static const int POINTS_3SEC = 5;
static const int PENALTY_MISS = -3;

static const float GAME_DURATION = 60.0f;

// ─────────────────────────────────────────────
//  TargetData
// ─────────────────────────────────────────────
struct TargetData {
    float age = 0.0f;
    bool hit = false;
    bool expired = false;
};

// ─────────────────────────────────────────────
// ─────────────────────────────────────────────
struct FloatText {
    Vector3 pos;
    std::string text;
    Color col;
};

// ─────────────────────────────────────────────
// ─────────────────────────────────────────────
class LifetimeComponent : public IComponent {
   public:
    float life = 0.8f;

    void frame(double dt) override {
        life -= (float)dt;
        if (life <= 0.0f)
            this->getGameObject()->destroy();
    }
};

// ─────────────────────────────────────────────
//  TargetComponent
// ─────────────────────────────────────────────
class TargetComponent : public IComponent {
   public:
    TargetData* data = nullptr;
    QuadRendererComponent* qrc = nullptr;

    void frame(double dt) override {
        if (data->hit || data->expired) return;

        data->age += (float)dt;

        float t = std::min(data->age / TARGET_LIFETIME, 1.0f);
        Color col;
        if (t < 0.5f) {
            float f = t / 0.5f;
            col = Color((int)(50 + f * 205), (int)(220 - f * 20), (int)(80 - f * 70));
        } else {
            float f = (t - 0.5f) / 0.5f;
            col = Color(255, (int)(200 - f * 200), 10);
        }
        if (qrc) qrc->setRectColor(col);

        if (data->age >= TARGET_LIFETIME) {
            data->expired = true;
            this->getGameObject()->destroy();
        }
    }
};

// ─────────────────────────────────────────────
//  ClickHandler — sadece flag set eder, destroy/spawn yok
// ─────────────────────────────────────────────
class ClickHandler : public IComponent {
   public:
    std::function<void(Vector2)> onClick;

    void onMouseEvent(const MouseEvent* e) override {
        if (e->event == MouseEventType::MouseClick &&
            e->button == MouseButton::Left) {
            if (onClick) onClick(e->pos);
        }
    }
};

// ─────────────────────────────────────────────
//  AimGame
// ─────────────────────────────────────────────
class AimGame : public CangineApp {
   public:
    AimGame() : CangineApp(WIN_W, WIN_H, "cangine :: Aim Trainer", false, Color(20, 20, 30)) {}

    int score = 0;
    float timeLeft = GAME_DURATION;
    float spawnTimer = 0.0f;
    bool gameOver = false;

    std::vector<GameObject*> targets;
    std::vector<TargetData*> targetDatas;

    std::vector<FloatText> pendingFloats;
    int pendingScore = 0;

    Label* scoreLabel = nullptr;
    Label* timerLabel = nullptr;
    Label* resultLabel = nullptr;
    Label* restartLabel = nullptr;

    // ─────────────────────────────────────────
    void onFirstFrame() override {
        std::srand((unsigned)std::time(nullptr));
        setupMap();
        setupClickHandler();
        setupUI();
    }

    // ─────────────────────────────────────────
    void onFrame(double dt) override {
        if (gameOver) {
            if (Input::getKeyDown(Key::R)) restartGame();
            return;
        }

        timeLeft -= (float)dt;
        if (timeLeft <= 0.0f) {
            timeLeft = 0.0f;
            triggerGameOver();
            return;
        }

        // Spawn
        spawnTimer += (float)dt;
        if (spawnTimer >= SPAWN_INTERVAL && (int)targets.size() < MAX_TARGETS) {
            spawnTimer = 0.0f;
            spawnTarget();
        }

        score += pendingScore;
        pendingScore = 0;

        for (auto& f : pendingFloats)
            showFloatingText(f.pos, f.text, f.col);
        pendingFloats.clear();

        cleanTargets();
        updateUI();
    }

   private:
    // ─────────────────────────────────────────
    void setupMap() {
        auto* bg = instantiateObject<GameObject>();
        auto* qrc = new QuadRendererComponent();
        bg->appendComponent<QuadRendererComponent>(qrc);
        qrc->setRectWidth((int)MAP_W);
        qrc->setRectHeight((int)MAP_H);
        qrc->setRectColor(Color(30, 32, 45));
        Transform::posFor(bg, Vector3(MAP_X, MAP_Y, 1));

        spawnBorderLine(MAP_W + 4, 4, MAP_X, MAP_Y + MAP_H / 2.0f + 2);
        spawnBorderLine(MAP_W + 4, 4, MAP_X, MAP_Y - MAP_H / 2.0f - 2);
        spawnBorderLine(4, MAP_H + 4, MAP_X - MAP_W / 2.0f - 2, MAP_Y);
        spawnBorderLine(4, MAP_H + 4, MAP_X + MAP_W / 2.0f + 2, MAP_Y);
    }

    void spawnBorderLine(float w, float h, float x, float y) {
        auto* obj = instantiateObject<GameObject>();
        auto* qrc = new QuadRendererComponent();
        obj->appendComponent<QuadRendererComponent>(qrc);
        qrc->setRectWidth((int)w);
        qrc->setRectHeight((int)h);
        qrc->setRectColor(Color(80, 100, 160));
        Transform::posFor(obj, Vector3(x, y, 2));
    }

    // ─────────────────────────────────────────
    void setupClickHandler() {
        auto* obj = instantiateObject<GameObject>();
        auto* handler = new ClickHandler();
        handler->onClick = [this](Vector2 pos) { this->handleClick(pos); };
        obj->appendComponent<ClickHandler>(handler);
    }

    // ─────────────────────────────────────────
    void setupUI() {
        scoreLabel = instantiateObject<Label>("Score: 0");
        Transform::posFor(scoreLabel, Vector3(
                                          -WIN_W / 2.0f + 10,
                                          WIN_H / 2.0f - scoreLabel->getSize().y - 6, 81));

        timerLabel = instantiateObject<Label>("Time: 60");
        Transform::posFor(timerLabel, Vector3(
                                          timerLabel->getSize().x / -2.0f,
                                          WIN_H / 2.0f - timerLabel->getSize().y - 6, 81));

        resultLabel = instantiateObject<Label>("TIME'S UP!");
        resultLabel->setColor(Color(255, 200, 50));
        Transform::posFor(resultLabel, Vector3(resultLabel->getSize().x / -2.0f, 40, 81));
        resultLabel->setActive(false);

        restartLabel = instantiateObject<Label>("Press R to restart");
        Transform::posFor(restartLabel, Vector3(restartLabel->getSize().x / -2.0f, 0, 81));
        restartLabel->setActive(false);
    }

    // ─────────────────────────────────────────
    void spawnTarget() {
        float margin = TARGET_RADIUS + 5.0f;
        float rx = MAP_X + (float)(std::rand() % (int)(MAP_W - margin * 2)) - (MAP_W / 2.0f - margin);
        float ry = MAP_Y + (float)(std::rand() % (int)(MAP_H - margin * 2)) - (MAP_H / 2.0f - margin);

        auto* obj = instantiateObject<GameObject>();
        auto* qrc = new QuadRendererComponent();
        obj->appendComponent<QuadRendererComponent>(qrc);
        qrc->setRectWidth((int)(TARGET_RADIUS * 2));
        qrc->setRectHeight((int)(TARGET_RADIUS * 2));
        qrc->setRectColor(Color(50, 220, 80));
        Transform::posFor(obj, Vector3(rx, ry, 10));

        auto* data = new TargetData{0.0f, false, false};
        auto* tComp = new TargetComponent();
        tComp->data = data;
        tComp->qrc = qrc;
        obj->appendComponent<TargetComponent>(tComp);

        targets.push_back(obj);
        targetDatas.push_back(data);
    }

    // ─────────────────────────────────────────
    //  handleClick — sadece flag + pending queue, destroy/spawn YOK
    // ─────────────────────────────────────────
    void handleClick(Vector2 clickPos) {
        if (gameOver) return;

        bool hitAny = false;

        for (int i = 0; i < (int)targets.size(); i++) {
            if (targets[i] == nullptr) continue;
            if (targetDatas[i]->hit) continue;
            if (targetDatas[i]->expired) continue;

            Vector3 tPos = Transform::posFrom(targets[i]);
            float dx = clickPos.x - tPos.x;
            float dy = clickPos.y - tPos.y;

            if (std::sqrt(dx * dx + dy * dy) <= TARGET_RADIUS) {
                hitAny = true;

                float age = targetDatas[i]->age;
                int pts;
                if (age <= 1.0f)
                    pts = POINTS_1SEC;
                else if (age <= 2.0f)
                    pts = POINTS_2SEC;
                else
                    pts = POINTS_3SEC;

                targetDatas[i]->hit = true;
                pendingScore += pts;
                pendingFloats.push_back({tPos, "+" + std::to_string(pts), Color(80, 255, 120)});
                break;
            }
        }

        if (!hitAny && isInsideMap(clickPos)) {
            pendingScore += PENALTY_MISS;
            pendingFloats.push_back({Vector3(clickPos.x, clickPos.y, 0),
                                     std::to_string(PENALTY_MISS),
                                     Color(255, 80, 80)});
        }
    }

    // ─────────────────────────────────────────
    void showFloatingText(Vector3 pos, const std::string& text, Color col) {
        auto* lbl = instantiateObject<Label>(text);
        lbl->setColor(col);
        pos.z = 20;
        Transform::posFor(lbl, pos);

        auto* lc = new LifetimeComponent();
        lbl->appendComponent<LifetimeComponent>(lc);
    }

    // ─────────────────────────────────────────
    bool isInsideMap(Vector2 pos) {
        return pos.x >= MAP_X - MAP_W / 2.0f &&
               pos.x <= MAP_X + MAP_W / 2.0f &&
               pos.y >= MAP_Y - MAP_H / 2.0f &&
               pos.y <= MAP_Y + MAP_H / 2.0f;
    }

    // ─────────────────────────────────────────
    void cleanTargets() {
        std::vector<GameObject*> newO;
        std::vector<TargetData*> newD;

        for (int i = 0; i < (int)targets.size(); i++) {
            if (targets[i] != nullptr && !targetDatas[i]->hit && !targetDatas[i]->expired) {
                newO.push_back(targets[i]);
                newD.push_back(targetDatas[i]);
            } else {
                if (targets[i] != nullptr)
                    targets[i]->destroy();
                delete targetDatas[i];
            }
        }

        targets = newO;
        targetDatas = newD;
    }

    // ─────────────────────────────────────────
    void updateUI() {
        scoreLabel->setText("Score: " + std::to_string(score));
        Transform::posFor(scoreLabel, Vector3(
                                          -WIN_W / 2.0f + 10,
                                          WIN_H / 2.0f - scoreLabel->getSize().y - 6, 81));

        int secs = (int)std::ceil(timeLeft);
        timerLabel->setText("Time: " + std::to_string(secs));
        if (timeLeft <= 10.0f)
            timerLabel->setColor(Color(255, 80, 80));
        Transform::posFor(timerLabel, Vector3(
                                          timerLabel->getSize().x / -2.0f,
                                          WIN_H / 2.0f - timerLabel->getSize().y - 6, 81));
    }

    // ─────────────────────────────────────────
    void triggerGameOver() {
        gameOver = true;
        for (auto* t : targets)
            if (t != nullptr) t->destroy();

        resultLabel->setText("TIME'S UP!  Final: " + std::to_string(score));
        Transform::posFor(resultLabel, Vector3(resultLabel->getSize().x / -2.0f, 40, 81));
        resultLabel->setActive(true);
        restartLabel->setActive(true);
    }

    // ─────────────────────────────────────────
    void restartGame() {
        for (auto* t : targets) {
            if (t != nullptr) t->destroy();
        }
        for (auto* d : targetDatas) delete d;
        targets.clear();
        targetDatas.clear();
        pendingFloats.clear();
        pendingScore = 0;

        score = 0;
        timeLeft = GAME_DURATION;
        spawnTimer = 0.0f;
        gameOver = false;

        resultLabel->setActive(false);
        restartLabel->setActive(false);
        timerLabel->setColor(Color(255, 255, 255));

        updateUI();
    }
};

// ─────────────────────────────────────────────
int main() {
    AimGame().run();
    return 0;
}
