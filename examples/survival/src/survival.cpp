#include "survival.hpp"

using namespace cangine;





static const int   WIN_W          = 800;
static const int   WIN_H          = 600;

static const float MAP_W          = 760.0f;
static const float MAP_H          = 540.0f;


static const float PLAYER_SIZE    = 18.0f;
static const float PLAYER_SPEED   = 200.0f;


static const int   WALL_COUNT     = 28;
static const float WALL_MIN       = 20.0f;
static const float WALL_MAX       = 55.0f;
static const float WALL_STUCK_DUR = 2.0f;   


static const float ENEMY_BASE_SPEED    = 95.0f;
static const float ENEMY_SPEED_GROWTH  = 3.5f;
static const float ENEMY_SIZE          = 20.0f;
static const float ENEMY_MIN_SPAWN_DIST= 220.0f;

static const float SPAWN_ENEMY1   = 0.0f;
static const float SPAWN_ENEMY2   = 15.0f;
static const float SPAWN_ENEMY3   = 30.0f;



static float vdist(float ax, float ay, float bx, float by) {
    float dx = ax - bx, dy = ay - by;
    return std::sqrt(dx * dx + dy * dy);
}

static std::string fmtTime(float t) {
    int mins = (int)t / 60;
    int secs = (int)t % 60;
    int ms   = (int)((t - (int)t) * 10);
    std::ostringstream ss;
    ss << mins << ":" << std::setw(2) << std::setfill('0') << secs
       << "." << ms;
    return ss.str();
}




struct WallData {
    float x, y, w, h;
};




enum class EnemyType { Chaser, Zigzag, Rusher };




struct EnemyState {
    EnemyType type       = EnemyType::Chaser;
    float     speed      = ENEMY_BASE_SPEED;
    float     stuckTimer = 0.0f;
    bool      active     = false;
    bool      dead       = false;

    float zigTimer  = 0.0f;
    float zigOffset = 0.0f;
    int   zigDir    = 1;

    float dashCooldown = 0.0f;
    bool  dashing      = false;
    float dashTimer    = 0.0f;
    float dashDirX     = 0.0f;
    float dashDirY     = 0.0f;
};




class EnemyMover : public IComponent {
public:
    EnemyState* state   = nullptr;
    float*      playerX = nullptr;
    float*      playerY = nullptr;
    std::vector<WallData>* walls = nullptr;
    std::vector<EnemyState*>* allStates = nullptr;
    float elapsed = 0.0f;

    void frame(double dt) override {
        if (!state->active || state->dead) return;

        elapsed += (float)dt;

        if (state->stuckTimer > 0.0f) {
            state->stuckTimer -= (float)dt;
            return;
        }

        float spd = state->speed + ENEMY_SPEED_GROWTH * elapsed;

        Transform* t  = this->getGameObject()->getComponent<Transform>();
        Vector3    pos = t->getPosition();

        float dirX = 0.0f, dirY = 0.0f;

        if (state->type == EnemyType::Chaser) {
            
            float dx = *playerX - pos.x;
            float dy = *playerY - pos.y;
            float d  = std::sqrt(dx * dx + dy * dy);
            if (d > 1.0f) { dirX = dx / d; dirY = dy / d; }

        } else if (state->type == EnemyType::Zigzag) {
            state->zigTimer += (float)dt;
            if (state->zigTimer > 0.6f) {
                state->zigTimer  = 0.0f;
                state->zigDir   *= -1;
                state->zigOffset = 60.0f * state->zigDir;
            }

            float tx = *playerX + state->zigOffset;
            float ty = *playerY;
            float dx = tx - pos.x;
            float dy = ty - pos.y;
            float d  = std::sqrt(dx * dx + dy * dy);
            if (d > 1.0f) { dirX = dx / d; dirY = dy / d; }

        } else if (state->type == EnemyType::Rusher) {
            if (state->dashing) {
                dirX = state->dashDirX;
                dirY = state->dashDirY;
                state->dashTimer -= (float)dt;
                spd *= 2.8f;
                if (state->dashTimer <= 0.0f) {
                    state->dashing      = false;
                    state->dashCooldown = 2.5f;
                }
            } else {
                state->dashCooldown -= (float)dt;
                float dx = *playerX - pos.x;
                float dy = *playerY - pos.y;
                float d  = std::sqrt(dx * dx + dy * dy);
                if (d > 1.0f) { dirX = dx / d; dirY = dy / d; }

                if (state->dashCooldown <= 0.0f && d < 250.0f) {
                    state->dashing      = true;
                    state->dashTimer    = 0.35f;
                    state->dashDirX     = dirX;
                    state->dashDirY     = dirY;
                    state->dashCooldown = 2.5f;
                }
            }
        }

        if (allStates) {
            for (auto* other : *allStates) {
                if (other == state || !other->active || other->dead) continue;
            }
        }

        float newX = pos.x + dirX * spd * (float)dt;
        float newY = pos.y + dirY * spd * (float)dt;

        float half = ENEMY_SIZE / 2.0f;
        newX = std::max(-(MAP_W / 2.0f - half), std::min(MAP_W / 2.0f - half, newX));
        newY = std::max(-(MAP_H / 2.0f - half), std::min(MAP_H / 2.0f - half, newY));

        bool hitWall = false;
        if (walls) {
            for (auto& w : *walls) {
                float wHalfW = w.w / 2.0f + half;
                float wHalfH = w.h / 2.0f + half;
                if (std::abs(newX - w.x) < wHalfW &&
                    std::abs(newY - w.y) < wHalfH) {
                    hitWall = true;
                    float testX = pos.x + dirX * spd * (float)dt;
                    float testY = pos.y;
                    if (std::abs(testX - w.x) < wHalfW &&
                        std::abs(testY - w.y) < wHalfH) {
                        testX = pos.x;
                    }
                    float testX2 = pos.x;
                    float testY2 = pos.y + dirY * spd * (float)dt;
                    if (std::abs(testX2 - w.x) < wHalfW &&
                        std::abs(testY2 - w.y) < wHalfH) {
                        testY2 = pos.y;
                    }
                    newX = testX;
                    newY = testY2;

                    if (newX == pos.x && newY == pos.y) {
                        state->stuckTimer = WALL_STUCK_DUR;
                    }
                    break;
                }
            }
        }
        (void)hitWall;

        t->setPosition(Vector3(newX, newY, pos.z));
    }
};




class SurvivalGame : public CangineApp {
public:
    SurvivalGame() : CangineApp(WIN_W, WIN_H, "cangine :: Survival", false, Color(18, 18, 28)) {}

    
    float      px = 0.0f, py = 0.0f;
    GameObject* playerObj = nullptr;
    QuadRendererComponent* playerQRC = nullptr;

    
    std::vector<WallData>   walls;
    std::vector<GameObject*> wallObjs;

    
    static const int MAX_ENEMIES = 3;
    GameObject*  enemyObjs[MAX_ENEMIES]   = {};
    EnemyMover*  enemyMovers[MAX_ENEMIES] = {};
    EnemyState   enemyStates[MAX_ENEMIES];
    std::vector<EnemyState*> allStatePtrs;

    
    float elapsed   = 0.0f;
    float bestTime  = 0.0f;
    bool  gameOver  = false;
    bool  spawned[MAX_ENEMIES] = {false, false, false};

    
    Label* timerLabel    = nullptr;
    Label* bestLabel     = nullptr;
    Label* gameOverLabel = nullptr;
    Label* finalLabel    = nullptr;
    Label* restartLabel  = nullptr;
    Label* hintLabel     = nullptr;

    
    float flashTimer = 0.0f;

    
    void onFirstFrame() override {
        std::srand((unsigned)std::time(nullptr));
        setupMap();
        setupPlayer();
        setupUI();

        for (int i = 0; i < MAX_ENEMIES; i++)
            allStatePtrs.push_back(&enemyStates[i]);
    }

    
    void onFrame(double dt) override {
        if (gameOver) {
            if (Input::getKeyDown(Key::R)) restartGame();
            return;
        }

        elapsed += (float)dt;

        
        if (flashTimer > 0.0f) {
            flashTimer -= (float)dt;
            playerQRC->setRectColor(flashTimer > 0.05f ?
                Color(255, 80, 80) : Color(100, 220, 255));
        }

        
        checkSpawns();

        movePlayer(dt);
        checkEnemyPlayerCollisions();
        updateUI();
    }

private:

    
    void setupMap() {
        
        auto* bg  = instantiateObject<GameObject>();
        auto* qrc = new QuadRendererComponent();
        bg->appendComponent<QuadRendererComponent>(qrc);
        qrc->setRectWidth((int)MAP_W);
        qrc->setRectHeight((int)MAP_H);
        qrc->setRectColor(Color(25, 28, 40));
        Transform::posFor(bg, Vector3(0, 0, 1));

        
        spawnBorder();

        
        spawnWalls();
    }

    void spawnBorder() {
        auto spawnLine = [&](float w, float h, float x, float y) {
            auto* obj = instantiateObject<GameObject>();
            auto* q   = new QuadRendererComponent();
            obj->appendComponent<QuadRendererComponent>(q);
            q->setRectWidth((int)w); q->setRectHeight((int)h);
            q->setRectColor(Color(60, 70, 110));
            Transform::posFor(obj, Vector3(x, y, 2));
        };
        spawnLine(MAP_W + 8, 8, 0,  MAP_H / 2.0f + 4);
        spawnLine(MAP_W + 8, 8, 0, -MAP_H / 2.0f - 4);
        spawnLine(8, MAP_H + 8, -MAP_W / 2.0f - 4, 0);
        spawnLine(8, MAP_H + 8,  MAP_W / 2.0f + 4, 0);
    }

    void spawnWalls() {
        int attempts = 0;
        int placed   = 0;

        while (placed < WALL_COUNT && attempts < 500) {
            attempts++;

            float w = WALL_MIN + (float)(std::rand() % (int)(WALL_MAX - WALL_MIN));
            float h = WALL_MIN + (float)(std::rand() % (int)(WALL_MAX - WALL_MIN));

            float rx = (float)(std::rand() % (int)(MAP_W - w - 40)) - (MAP_W / 2.0f - 20);
            float ry = (float)(std::rand() % (int)(MAP_H - h - 40)) - (MAP_H / 2.0f - 20);

            if (vdist(rx, ry, 0, 0) < 90.0f) continue;

            bool overlap = false;
            for (auto& wl : walls) {
                if (std::abs(rx - wl.x) < (w / 2 + wl.w / 2 + 12) &&
                    std::abs(ry - wl.y) < (h / 2 + wl.h / 2 + 12)) {
                    overlap = true; break;
                }
            }
            if (overlap) continue;

            walls.push_back({rx, ry, w, h});

            auto* obj = instantiateObject<GameObject>();
            auto* q   = new QuadRendererComponent();
            obj->appendComponent<QuadRendererComponent>(q);
            q->setRectWidth((int)w);
            q->setRectHeight((int)h);
            
            int shade = 50 + std::rand() % 20;
            q->setRectColor(Color(shade, shade + 10, shade + 30));
            Transform::posFor(obj, Vector3(rx, ry, 3));
            wallObjs.push_back(obj);
            placed++;
        }
    }

    
    void setupPlayer() {
        playerObj = instantiateObject<GameObject>();
        playerQRC = new QuadRendererComponent();
        playerObj->appendComponent<QuadRendererComponent>(playerQRC);
        playerQRC->setRectWidth((int)PLAYER_SIZE);
        playerQRC->setRectHeight((int)PLAYER_SIZE);
        playerQRC->setRectColor(Color(100, 220, 255));
        Transform::posFor(playerObj, Vector3(0, 0, 10));
        px = 0; py = 0;
    }

    
    void setupUI() {
        timerLabel = instantiateObject<Label>("00:00.0");
        Transform::posFor(timerLabel, Vector3(
            timerLabel->getSize().x / -2.0f,
            WIN_H / 2.0f - timerLabel->getSize().y - 6, 81
        ));

        bestLabel = instantiateObject<Label>("Best: 00:00.0");
        Transform::posFor(bestLabel, Vector3(
            WIN_W / 2.0f - bestLabel->getSize().x - 10,
            WIN_H / 2.0f - bestLabel->getSize().y - 6, 81
        ));

        hintLabel = instantiateObject<Label>("WASD / Arrow keys to move");
        hintLabel->setColor(Color(120, 120, 160));
        Transform::posFor(hintLabel, Vector3(
            hintLabel->getSize().x / -2.0f,
            -(WIN_H / 2.0f - hintLabel->getSize().y - 6), 81
        ));

        gameOverLabel = instantiateObject<Label>("YOU DIED");
        gameOverLabel->setColor(Color(255, 60, 60));
        Transform::posFor(gameOverLabel, Vector3(gameOverLabel->getSize().x / -2.0f, 50, 81));
        gameOverLabel->setActive(false);

        finalLabel = instantiateObject<Label>("Time: 0:00.0");
        finalLabel->setColor(Color(255, 200, 60));
        Transform::posFor(finalLabel, Vector3(finalLabel->getSize().x / -2.0f, 10, 81));
        finalLabel->setActive(false);

        restartLabel = instantiateObject<Label>("Press R to restart");
        Transform::posFor(restartLabel, Vector3(restartLabel->getSize().x / -2.0f, -30, 81));
        restartLabel->setActive(false);
    }

    
    
    void checkSpawns() {
        float spawnTimes[MAX_ENEMIES] = {SPAWN_ENEMY1, SPAWN_ENEMY2, SPAWN_ENEMY3};
        EnemyType types[MAX_ENEMIES]  = {EnemyType::Chaser, EnemyType::Zigzag, EnemyType::Rusher};
        Color colors[MAX_ENEMIES]     = {
            Color(255, 80,  80),
            Color(255, 160, 40),
            Color(200, 60, 200)
        };

        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (!spawned[i] && elapsed >= spawnTimes[i]) {
                spawnEnemy(i, types[i], colors[i]);
                spawned[i] = true;
            }
        }
    }

    void spawnEnemy(int idx, EnemyType type, Color col) {
        float ex, ey;
        int attempts = 0;
        do {
            ex = (float)(std::rand() % (int)(MAP_W - 60)) - (MAP_W / 2.0f - 30);
            ey = (float)(std::rand() % (int)(MAP_H - 60)) - (MAP_H / 2.0f - 30);
            attempts++;
        } while (vdist(ex, ey, px, py) < ENEMY_MIN_SPAWN_DIST && attempts < 200);

        for (int j = 0; j < MAX_ENEMIES; j++) {
            if (j == idx || !spawned[j]) continue;
            if (enemyObjs[j] == nullptr) continue;
            Vector3 ePos = Transform::posFrom(enemyObjs[j]);
            if (vdist(ex, ey, ePos.x, ePos.y) < 100.0f) {
                ex += 120.0f * (idx % 2 == 0 ? 1 : -1);
                ey += 120.0f * (idx % 2 == 0 ? -1 : 1);
            }
        }

        auto* obj = instantiateObject<GameObject>();
        auto* q   = new QuadRendererComponent();
        obj->appendComponent<QuadRendererComponent>(q);
        q->setRectWidth((int)ENEMY_SIZE);
        q->setRectHeight((int)ENEMY_SIZE);
        q->setRectColor(col);
        Transform::posFor(obj, Vector3(ex, ey, 8));

        auto* inner = instantiateObject<GameObject>();
        auto* qi    = new QuadRendererComponent();
        inner->appendComponent<QuadRendererComponent>(qi);
        int iSize = (type == EnemyType::Rusher) ? 6 : 8;
        qi->setRectWidth(iSize); qi->setRectHeight(iSize);
        qi->setRectColor(Color(255, 255, 255));
        Transform::posFor(inner, Vector3(ex, ey, 9));

        
        enemyStates[idx].type       = type;
        enemyStates[idx].speed      = ENEMY_BASE_SPEED + idx * 8.0f;
        enemyStates[idx].active     = true;
        enemyStates[idx].dead       = false;
        enemyStates[idx].stuckTimer = 0.0f;
        enemyStates[idx].zigTimer   = 0.0f;
        enemyStates[idx].zigOffset  = 0.0f;
        enemyStates[idx].zigDir     = (idx % 2 == 0) ? 1 : -1;
        enemyStates[idx].dashing    = false;
        enemyStates[idx].dashCooldown = 1.5f;

        auto* mover       = new EnemyMover();
        mover->state      = &enemyStates[idx];
        mover->playerX    = &px;
        mover->playerY    = &py;
        mover->walls      = &walls;
        mover->allStates  = &allStatePtrs;
        obj->appendComponent<EnemyMover>(mover);

        enemyObjs[idx]    = obj;
        enemyMovers[idx]  = mover;
    }

    
    
    
    void movePlayer(double dt) {
        float dx = 0, dy = 0;

        if (Input::getKey(Key::W) || Input::getKey(Key::Up))    dy += 1.0f;
        if (Input::getKey(Key::S) || Input::getKey(Key::Down))  dy -= 1.0f;
        if (Input::getKey(Key::A) || Input::getKey(Key::Left))  dx -= 1.0f;
        if (Input::getKey(Key::D) || Input::getKey(Key::Right)) dx += 1.0f;

        
        float len = std::sqrt(dx * dx + dy * dy);
        if (len > 0) { dx /= len; dy /= len; }

        float newX = px + dx * PLAYER_SPEED * (float)dt;
        float newY = py + dy * PLAYER_SPEED * (float)dt;

        float half = PLAYER_SIZE / 2.0f;
        newX = std::max(-(MAP_W / 2.0f - half), std::min(MAP_W / 2.0f - half, newX));
        newY = std::max(-(MAP_H / 2.0f - half), std::min(MAP_H / 2.0f - half, newY));

        for (auto& w : walls) {
            float wHW = w.w / 2.0f + half;
            float wHH = w.h / 2.0f + half;

            if (std::abs(newX - w.x) < wHW && std::abs(newY - w.y) < wHH) {
                
                float testX = px + dx * PLAYER_SPEED * (float)dt;
                if (std::abs(testX - w.x) < wHW && std::abs(py - w.y) < wHH)
                    newX = px;
                
                float testY = py + dy * PLAYER_SPEED * (float)dt;
                if (std::abs(px - w.x) < wHW && std::abs(testY - w.y) < wHH)
                    newY = py;
                if (std::abs(newX - w.x) < wHW && std::abs(newY - w.y) < wHH) {
                    newX = px; newY = py;
                }
            }
        }

        px = newX; py = newY;
        Transform::posFor(playerObj, Vector3(px, py, 10));
    }

    
    void checkEnemyPlayerCollisions() {
        float phalf = PLAYER_SIZE / 2.0f;
        float ehalf = ENEMY_SIZE  / 2.0f;

        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (!enemyStates[i].active || enemyStates[i].dead) continue;
            if (enemyObjs[i] == nullptr) continue;

            Vector3 ePos = Transform::posFrom(enemyObjs[i]);
            if (std::abs(ePos.x - px) < phalf + ehalf &&
                std::abs(ePos.y - py) < phalf + ehalf) {
                triggerGameOver();
                return;
            }
        }
    }

    
    void updateUI() {
        
        timerLabel->setText(fmtTime(elapsed));
        Transform::posFor(timerLabel, Vector3(
            timerLabel->getSize().x / -2.0f,
            WIN_H / 2.0f - timerLabel->getSize().y - 6, 81
        ));

        bool anyStuck = false;
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (enemyStates[i].active && enemyStates[i].stuckTimer > 0.0f)
                anyStuck = true;
        }
        hintLabel->setText(anyStuck ? "ENEMY STUCK!" : "WASD / Arrows to move");
        hintLabel->setColor(anyStuck ? Color(80, 255, 120) : Color(120, 120, 160));
        Transform::posFor(hintLabel, Vector3(
            hintLabel->getSize().x / -2.0f,
            -(WIN_H / 2.0f - hintLabel->getSize().y - 6), 81
        ));

        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (!enemyStates[i].active || enemyObjs[i] == nullptr) continue;
            Vector3 ePos = Transform::posFrom(enemyObjs[i]);
            if (vdist(ePos.x, ePos.y, px, py) < 80.0f && flashTimer <= 0.0f) {
                flashTimer = 0.08f;
                playerQRC->setRectColor(Color(255, 180, 180));
            }
        }
    }

    
    void triggerGameOver() {
        gameOver = true;
        if (elapsed > bestTime) bestTime = elapsed;

        playerQRC->setRectColor(Color(255, 60, 60));

        gameOverLabel->setActive(true);
        finalLabel->setText("Survived: " + fmtTime(elapsed) +
                            "  Best: " + fmtTime(bestTime));
        Transform::posFor(finalLabel, Vector3(finalLabel->getSize().x / -2.0f, 10, 81));
        finalLabel->setActive(true);
        restartLabel->setActive(true);
    }

    
    void restartGame() {
        
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (enemyObjs[i] != nullptr) {
                enemyObjs[i]->destroy();
                enemyObjs[i] = nullptr;
            }
            enemyMovers[i] = nullptr;
            enemyStates[i] = EnemyState{};
            spawned[i]     = false;
        }

        elapsed   = 0.0f;
        gameOver  = false;
        flashTimer = 0.0f;

        px = 0; py = 0;
        playerQRC->setRectColor(Color(100, 220, 255));
        Transform::posFor(playerObj, Vector3(0, 0, 10));

        
        gameOverLabel->setActive(false);
        finalLabel->setActive(false);
        restartLabel->setActive(false);

        bestLabel->setText("Best: " + fmtTime(bestTime));
        Transform::posFor(bestLabel, Vector3(
            WIN_W / 2.0f - bestLabel->getSize().x - 10,
            WIN_H / 2.0f - bestLabel->getSize().y - 6, 81
        ));

        updateUI();
    }
};


int main() {
    SurvivalGame().run();
    return 0;
}
