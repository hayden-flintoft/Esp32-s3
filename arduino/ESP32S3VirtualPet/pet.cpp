#include "pet.h"
#include <Preferences.h>

static Preferences prefs;
static const char *NS = "pet";

static uint8_t clampU8(int v) {
    if (v < 0) return 0;
    if (v > 100) return 100;
    return (uint8_t)v;
}

void Pet::begin() {
    prefs.begin(NS, false);
    _s.hunger     = prefs.getUChar("hunger", 80);
    _s.happiness  = prefs.getUChar("happy", 80);
    _s.energy     = prefs.getUChar("energy", 100);
    _s.hygiene    = prefs.getUChar("hygiene", 100);
    _s.asleep     = prefs.getBool("asleep", false);
    _s.sick       = prefs.getBool("sick", false);
    _s.ageSeconds = prefs.getUInt("age", 0);
    _s.stage      = (PetStage)prefs.getUChar("stage", (uint8_t)PetStage::Egg);
}

void Pet::save() {
    prefs.putUChar("hunger", _s.hunger);
    prefs.putUChar("happy", _s.happiness);
    prefs.putUChar("energy", _s.energy);
    prefs.putUChar("hygiene", _s.hygiene);
    prefs.putBool("asleep", _s.asleep);
    prefs.putBool("sick", _s.sick);
    prefs.putUInt("age", _s.ageSeconds);
    prefs.putUChar("stage", (uint8_t)_s.stage);
}

void Pet::clamp() {
    _s.hunger    = clampU8(_s.hunger);
    _s.happiness = clampU8(_s.happiness);
    _s.energy    = clampU8(_s.energy);
    _s.hygiene   = clampU8(_s.hygiene);
}

void Pet::updateStage() {
    // Egg hatches after 60s so the demo doesn't sit on an egg screen forever;
    // later stages are paced in "pet minutes" (ageSeconds), not real days.
    if (_s.stage == PetStage::Egg && _s.ageSeconds >= 60) {
        _s.stage = PetStage::Baby;
    } else if (_s.stage == PetStage::Baby && _s.ageSeconds >= 30 * 60) {
        _s.stage = PetStage::Child;
    } else if (_s.stage == PetStage::Child && _s.ageSeconds >= 2 * 3600) {
        _s.stage = PetStage::Adult;
    }
}

void Pet::applyDecayTick() {
    if (_s.stage == PetStage::Egg) {
        return; // eggs don't need care
    }

    if (_s.asleep) {
        // Sleeping: energy recovers, everything else decays slower.
        _s.energy = clampU8(_s.energy + 4);
        _s.hunger = clampU8(_s.hunger - 1);
    } else {
        _s.hunger    = clampU8(_s.hunger - 2);
        _s.happiness = clampU8(_s.happiness - 1);
        _s.energy    = clampU8(_s.energy - 1);
        _s.hygiene   = clampU8(_s.hygiene - 1);
    }

    // Neglect makes the pet sick; care (via giveMedicine) fixes it.
    if (!_s.sick && (_s.hunger == 0 || _s.hygiene == 0)) {
        _s.sick = true;
    }
    if (_s.sick) {
        _s.happiness = clampU8(_s.happiness - 1);
    }
}

void Pet::update(uint32_t deltaMs) {
    _s.ageSeconds += deltaMs / 1000;
    _decayAccumMs += deltaMs;

    while (_decayAccumMs >= DECAY_TICK_MS) {
        _decayAccumMs -= DECAY_TICK_MS;
        applyDecayTick();
    }

    updateStage();
    clamp();
}

void Pet::feed() {
    if (_s.stage == PetStage::Egg) return;
    _s.hunger = clampU8(_s.hunger + 25);
    _s.happiness = clampU8(_s.happiness + 3);
}

void Pet::play() {
    if (_s.stage == PetStage::Egg || _s.asleep) return;
    _s.happiness = clampU8(_s.happiness + 20);
    _s.energy    = clampU8(_s.energy - 10);
    _s.hunger    = clampU8(_s.hunger - 5);
}

void Pet::clean() {
    if (_s.stage == PetStage::Egg) return;
    _s.hygiene = clampU8(_s.hygiene + 40);
}

void Pet::toggleSleep() {
    if (_s.stage == PetStage::Egg) return;
    _s.asleep = !_s.asleep;
}

void Pet::giveMedicine() {
    _s.sick = false;
    _s.happiness = clampU8(_s.happiness + 5);
}

PetMood Pet::mood() const {
    if (_s.sick) return PetMood::Sick;
    int avg = (_s.hunger + _s.happiness + _s.energy + _s.hygiene) / 4;
    if (avg >= 70) return PetMood::Great;
    if (avg >= 40) return PetMood::Okay;
    return PetMood::Sad;
}

const char *Pet::stageName() const {
    switch (_s.stage) {
        case PetStage::Egg:   return "Egg";
        case PetStage::Baby:  return "Baby";
        case PetStage::Child: return "Child";
        case PetStage::Adult: return "Adult";
    }
    return "?";
}

const char *Pet::moodName() const {
    switch (mood()) {
        case PetMood::Great: return "Great";
        case PetMood::Okay:  return "Okay";
        case PetMood::Sad:   return "Sad";
        case PetMood::Sick:  return "Sick";
    }
    return "?";
}
