#ifndef BRAINPARTY_AUDIO_H
#define BRAINPARTY_AUDIO_H

class BPGame;
class AudioPriv;

class Audio {
public:
    Audio();
    ~Audio();

    void start();
    void stop();
    
    static void set_last_music(BPGame *game, const char *last_music);

private:
    AudioPriv *priv;
};

#endif
