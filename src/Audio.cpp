#include "Audio.h"
#include "BPGame.h"


static AudioPriv *
g_priv = NULL;

class AudioPriv {
public:
    AudioPriv();
    virtual ~AudioPriv();

    virtual void start();
    virtual void stop();

    void set_last_music(BPGame *game, const char *last_music);
    void on_acquired(bool acquired);

protected:
    BPGame *game;
    const char *last_music;
    bool mix_opened;
};

AudioPriv::AudioPriv()
    : game(NULL)
    , last_music(NULL)
    , mix_opened(false)
{
}

AudioPriv::~AudioPriv()
{
    on_acquired(false);
}

void AudioPriv::start()
{
    on_acquired(true);
}

void AudioPriv::stop()
{
    on_acquired(false);
}

void AudioPriv::set_last_music(BPGame *game, const char *last_music)
{
    this->game = game;
    this->last_music = last_music;
}

void AudioPriv::on_acquired(bool acquired)
{
    if (acquired && !mix_opened) {
        Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);
        mix_opened = true;
        if (game && last_music != NULL) {
            game->PlayMusic(last_music);
        }
    } else if (!acquired && mix_opened) {
        Mix_CloseAudio();
        mix_opened = false;
    }
}


#if defined(HAVE_AUDIORESOURCE)
#include <audioresource.h>

void
on_audio_resource_acquired(audioresource_t *audio_resource, bool acquired, void *user_data)
{
    AudioPriv *priv = static_cast<AudioPriv *>(user_data);
    priv->on_acquired(acquired);
}

class AudioPrivNemo : public AudioPriv {
public:
    AudioPrivNemo();
    virtual ~AudioPrivNemo();

    virtual void start();
    virtual void stop();

private:
    audioresource_t *audiores;
};

AudioPrivNemo::AudioPrivNemo()
    : AudioPriv()
    , audiores(audioresource_init(AUDIO_RESOURCE_GAME, on_audio_resource_acquired, static_cast<AudioPriv *>(this)))
{
}

AudioPrivNemo::~AudioPrivNemo()
{
    audioresource_free(audiores);
}

void AudioPrivNemo::start()
{
    audioresource_acquire(audiores);
    while (!mix_opened) {
        g_main_context_iteration(NULL, false);
    }
}

void AudioPrivNemo::stop()
{
    audioresource_release(audiores);
    while (mix_opened) {
        g_main_context_iteration(NULL, false);
    }
}
#endif /* defined(HAVE_AUDIORESOURCE) */


Audio::Audio()
#if defined(HAVE_AUDIORESOURCE)
    : priv(new AudioPrivNemo())
#else
    : priv(new AudioPriv())
#endif
{
    g_priv = priv;
}

Audio::~Audio()
{
    delete priv;
    g_priv = NULL;
}

void Audio::start()
{
    priv->start();
}

void Audio::stop()
{
    priv->stop();
}

void Audio::set_last_music(BPGame *game, const char *last_music)
{
    g_priv->set_last_music(game, last_music);
}
