#include "intro.hpp"
#include "../game/game.hpp"

IntroState::IntroState(WAV_track music)
    : m_state(State::INITIAL)
    , m_state_timer(0)
    , m_fadeout_timer(0)
    , m_pulseDelta(0)
    , m_pulseIncreasing(true)
    , m_music(music)
    , m_brock_fade(0)
    , m_zaya_fade(0)
    , m_tica_fade(0)
    , m_ira_fade(0)
{
    m_black = new Texture("res/black.png");
    m_background = new Texture("res/intro/background.png");
    m_vignette = new Texture("res/intro/vignette.png");
    m_brock = new Texture("res/intro/brock.png");
    m_zaya = new Texture("res/intro/zaya.png");
    m_tica = new Texture("res/intro/tica.png");
    m_ira = new Texture("res/intro/ira.png");

    QcE::get_instance()->getAudioEngine()->setVolume(m_music, 0.2f);
}

IntroState::~IntroState()
{
    delete m_black;
    delete m_background;
    delete m_vignette;
    delete m_brock;
    delete m_zaya;
    delete m_tica;
    delete m_ira;
}


void IntroState::init()
{

}

void IntroState::cleanup()
{

}

void IntroState::update()
{
    bool start_game = false;
    m_state_timer += delta;
    if (keyPressed(SDL_SCANCODE_ESCAPE)) {
        exit();
    }
    if (keyPressed(SDL_SCANCODE_SPACE)) {
        if (m_state_timer > GetStateTimeMinimum(m_state))
        {
            m_state_timer = 0;
            if (m_state != State::FINAL)
            {
                m_state = static_cast<State>(static_cast<int>(m_state) + 1);
                // Play voice lines for the THINGS
                std::string voice = GetStateVoiceLine(m_state);
                if (voice.size() > 0)
                {
                    m_curr_voice = loadWAV(voice, false);
                    if (!m_curr_voice) {
                        ERR("Something went wrong when trying to load WAV File");
                    }
                    playWAV(m_curr_voice);
                }
            }
        }
    }
    if (m_state == State::FINAL)
    {
        if (m_fadeout_timer > kFadeoutTime)
        {
            start_game = true;
        }
        else
        {
            m_fadeout_timer += delta;
            // CALCULATE THE FADEOUT
            // Finish fading out a half second before
            int total_time = kFadeoutTime - 500;
            float ratio = static_cast<float>(m_fadeout_timer) / static_cast<float>(total_time);
            if (ratio > 1.f) ratio = 1.f;
            m_black->setAlpha(static_cast<int>(ratio * 255.f));

            float music_volume = 0.2f + ratio * 0.8f;
            QcE::get_instance()->getAudioEngine()->setVolume(m_music, music_volume);
        }
    }
    // Secret hack to skip
    if (keyPressed(SDL_SCANCODE_N))
    {
        start_game = true;
    }
    UpdatePulse();
    if (start_game)
    {
        changeState(std::make_unique<GameState>());
    }
    // CALCULATE FADE IN
    if (m_state == State::INITIAL)
    {
        // Finish fading in a second before
        int total_time = GetStateTimeMinimum(State::INITIAL) - 1000;
        float ratio = 1.f - static_cast<float>(m_state_timer) / static_cast<float>(total_time);
        if (ratio < 0.f) ratio = 0.f;
        m_black->setAlpha(static_cast<int>(ratio * 255.f));
    }
}

void IntroState::render()
{
    m_background->render(0, 0);
    // RENDER BASED ON STATE
    if (static_cast<int>(m_state) >= State::BROCK)
    {
        if (m_brock_fade < kTextureFadeTime)
        {
            m_brock_fade += delta;
            float ratio = static_cast<float>(m_brock_fade) / static_cast<float>(kTextureFadeTime);
            if (ratio > 1.f) ratio = 1.f;
            m_brock->setAlpha(static_cast<int>(ratio * 255.f));
        }
        m_brock->render(0, 0);
    }
    if (static_cast<int>(m_state) >= State::ZAYA)
    {
        if (m_zaya_fade < kTextureFadeTime)
        {
            m_zaya_fade += delta;
            float ratio = static_cast<float>(m_zaya_fade) / static_cast<float>(kTextureFadeTime);
            if (ratio > 1.f) ratio = 1.f;
            m_zaya->setAlpha(static_cast<int>(ratio * 255.f));
        }
        m_zaya->render(0, 0);
    }
    if (static_cast<int>(m_state) >= State::TICA)
    {
        if (m_tica_fade < kTextureFadeTime)
        {
            m_tica_fade += delta;
            float ratio = static_cast<float>(m_tica_fade) / static_cast<float>(kTextureFadeTime);
            if (ratio > 1.f) ratio = 1.f;
            m_tica->setAlpha(static_cast<int>(ratio * 255.f));
        }
        m_tica->render(0, 0);
    }
    if (static_cast<int>(m_state) >= State::IRA)
    {
        if (m_ira_fade < kTextureFadeTime)
        {
            m_ira_fade += delta;
            float ratio = static_cast<float>(m_ira_fade) / static_cast<float>(kTextureFadeTime);
            if (ratio > 1.f) ratio = 1.f;
            m_ira->setAlpha(static_cast<int>(ratio * 255.f));
        }
        m_ira->render(0, 0);
    }
    m_vignette->render(0, 0);
    // Render fade in first
    m_black->render(0, 0, 1600, 900);
}

void IntroState::UpdatePulse()
{
    if (m_pulseIncreasing)
    {
        m_pulseDelta += delta;
        if (m_pulseDelta > kPulseTime)
        {
            m_pulseDelta = kPulseTime;
            m_pulseIncreasing = false;
        }
    }
    else
    {
        m_pulseDelta -= delta;
        if (m_pulseDelta < 0)
        {
            m_pulseDelta = 0;
            m_pulseIncreasing = true;
        }
    }
    float ratio = static_cast<float>(m_pulseDelta) / static_cast<float>(kPulseTime);
    m_vignette->setAlpha(static_cast<int>(255.f * ratio));
}