#pragma once
#include <Arduino.h>

enum class PetStage : uint8_t { Egg, Baby, Child, Adult };
enum class PetMood  : uint8_t { Great, Okay, Sad, Sick };

struct PetState {
    uint8_t hunger    = 80;  // 0 = starving, 100 = full
    uint8_t happiness = 80;  // 0 = miserable, 100 = delighted
    uint8_t energy     = 100; // 0 = exhausted, 100 = fully rested
    uint8_t hygiene    = 100; // 0 = filthy, 100 = spotless
    bool    asleep     = false;
    bool    sick        = false;
    uint32_t ageSeconds = 0;   // total time alive, accumulated across power cycles
    PetStage stage       = PetStage::Egg;
};

// Drives all the virtual-pet game logic. No hardware dependencies --
// callers feed it elapsed time and player actions, and read back state to render.
class Pet {
public:
    void begin();               // loads saved state from NVS (Preferences), or starts fresh
    void update(uint32_t deltaMs); // call every loop with elapsed real time
    void save();                 // persist to NVS; call periodically and before sleep/reset

    void feed();
    void play();
    void clean();
    void toggleSleep();
    void giveMedicine();

    const PetState &state() const { return _s; }
    PetMood mood() const;
    const char *stageName() const;
    const char *moodName() const;

private:
    PetState _s;
    uint32_t _decayAccumMs = 0;
    static const uint32_t DECAY_TICK_MS = 30000; // one decay step every 30s

    void applyDecayTick();
    void updateStage();
    void clamp();
};
