#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <raylib.h>

#include <string>
#include <vector>

class Dialogue {
public:
    Dialogue(const std::vector<std::string>& sentences,
        const Texture2D& portraitTexture, const Music& sound);
    ~Dialogue();

    void update(float dt);
    void draw(int screenWidth, int screenHeight);
    void reset();

    bool isFinished() const;
    void handleInput();

private:
    std::vector<std::string> sentences;
    int currentSentenceIndex;

    std::string displayedText;
    int currentCharIndex;
    float charTimer;
    float charInterval;

    Texture2D portraitTexture;
    Music dialogueMusic;
    Font font;

    bool finished;
    bool sentenceFinished;

    void loadNextSentence();
};

#endif // DIALOGUE_H
