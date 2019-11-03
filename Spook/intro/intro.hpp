#pragma once
#include "core/engine.hpp"

//////////////////////////////////////////////////////
constexpr int kPulseTime = 1500;
constexpr int kTextureFadeTime = 1500;
constexpr int kFadeoutTime = 3000;
class IntroState : public State
{
public:
    enum State
    {
        INITIAL = 0,
        START,
        BROCK,
        ZAYA,
        TICA,
        IRA,
        FINAL
    };
    int GetStateTimeMinimum(State state) const
    {
        if (state == State::INITIAL) return 8000;
        if (state == State::START) return 11000;
        if (state == State::BROCK) return 5000;
        if (state == State::ZAYA) return 5000;
        if (state == State::TICA) return 4000;
        if (state == State::IRA) return 5000;
        if (state == State::FINAL) return 5000;
        return 0;
    }
    std::string GetStateVoiceLine(State state) const
    {
        if (state == State::INITIAL) return std::string("");
        if (state == State::START) return std::string("res/intro/line1.wav");
        if (state == State::BROCK) return std::string("res/intro/line2.wav");
        if (state == State::ZAYA) return std::string("res/intro/line3.wav");
        if (state == State::TICA) return std::string("res/intro/line4.wav");
        if (state == State::IRA) return std::string("res/intro/line5.wav");
        if (state == State::FINAL) return std::string("res/intro/line6.wav");
        return std::string("");
    }

    IntroState(WAV_track music);
    ~IntroState();

    void init() override;
    void cleanup() override;

    void pause() override {}
    void resume() override {}

    void update() override;
    void render() override;
private:
    State m_state;
    int m_state_timer;
    int m_fadeout_timer;

    Texture * m_black;
    Texture * m_background;
    Texture * m_vignette;
    Texture * m_brock;
    Texture * m_zaya;
    Texture * m_tica;
    Texture * m_ira;

    // Fades for each of the textures
    int m_brock_fade;
    int m_zaya_fade;
    int m_tica_fade;
    int m_ira_fade;

    // Vignette pulse
    void UpdatePulse();
    int m_pulseDelta;
    bool m_pulseIncreasing;

    // Voice lines
    WAV_track m_curr_voice;
    WAV_track m_music;
};