#include "Dialogue.h"

Dialogue::Dialogue(const std::vector<std::string>& sentences,
    const Texture2D& portraitTexture, const Music& sound)
    : sentences(sentences)
    , currentSentenceIndex(0)
    , currentCharIndex(0)
    , charTimer(0.0f)
    , finished(false)
    , sentenceFinished(false)
{
    this->portraitTexture = portraitTexture;
    this->dialogueMusic = sound;
    this->font = GetFontDefault();

    if (this->sentences.size() > 0 && this->sentences[0].length() > 0) {
        float musicLength = GetMusicTimeLength(this->dialogueMusic);
        this->charInterval = (musicLength / this->sentences[0].length()) * 0.1;
    } else {
        this->charInterval = 0.005f;
    }

    this->loadNextSentence();
}

Dialogue::~Dialogue() { }

void Dialogue::loadNextSentence()
{
    if (this->currentSentenceIndex < this->sentences.size()) {
        PlayMusicStream(this->dialogueMusic);
        this->displayedText = "";
        this->currentCharIndex = 0;
        this->charTimer = 0.0f;
        this->sentenceFinished = false;

        if (this->sentences[this->currentSentenceIndex].length() > 0) {
            float musicLength = GetMusicTimeLength(this->dialogueMusic);
            this->charInterval
                = (musicLength
                      / this->sentences[this->currentSentenceIndex].length())
                * 0.1;
        } else {
            this->charInterval = 0.05f;
        }
    } else {
        this->finished = true;
    }
}

void Dialogue::update(float dt)
{
    if (this->finished)
        return;

    UpdateMusicStream(this->dialogueMusic);

    if (!this->sentenceFinished) {
        this->charTimer += dt;
        if (this->charTimer >= this->charInterval
            && this->currentCharIndex
                < this->sentences[this->currentSentenceIndex].length()) {
            this->displayedText += this->sentences[this->currentSentenceIndex]
                                                  [this->currentCharIndex];
            this->currentCharIndex++;
            this->charTimer = 0.0f;
        }

        if (this->currentCharIndex
            >= this->sentences[this->currentSentenceIndex].length()) {
            this->sentenceFinished = true;
            StopMusicStream(this->dialogueMusic);
        }
    }
}

void Dialogue::reset()
{
    this->currentSentenceIndex = 0;
    this->currentCharIndex = 0;
    this->charTimer = 0.0f;

    this->finished = false;
    this->sentenceFinished = false;
    this->displayedText = "";

    StopMusicStream(this->dialogueMusic);

    this->loadNextSentence();
}

void Dialogue::draw(int screenWidth, int screenHeight)
{
    if (this->finished)
        return;

    int boxHeight = screenHeight / 5.4f;
    int boxY = screenHeight - boxHeight;
    float fontSize = screenHeight / 33.0f;

    DrawRectangle(0, boxY, screenWidth, boxHeight, Fade(BLACK, 0.8f));

    DrawTexture(this->portraitTexture, 20, boxY + (boxHeight - this->portraitTexture.height) / 2, WHITE);

    DrawText(this->displayedText.c_str(),
        (float)(this->portraitTexture.width + 40),
        (float)(boxY + boxHeight * 0.1f), fontSize, WHITE);

    float hintFontSize = fontSize * 0.7f;

    int textWidth = MeasureText("Press Enter to continue", hintFontSize);

    DrawText("Press Enter to continue", screenWidth - textWidth - 20, screenHeight - hintFontSize - 10, hintFontSize, GRAY);
}

bool Dialogue::isFinished() const
{
    return this->finished;
}

void Dialogue::handleInput()
{
    if (IsKeyPressed(KEY_ENTER)) {
        if (!this->sentenceFinished) {
            this->displayedText = this->sentences[this->currentSentenceIndex];
            this->sentenceFinished = true;
            this->currentCharIndex
                = this->sentences[this->currentSentenceIndex].length();
            StopMusicStream(this->dialogueMusic);
        } else {
            this->currentSentenceIndex++;
            if (this->currentSentenceIndex < this->sentences.size()) {
                this->loadNextSentence();
            } else {
                this->finished = true;
            }
        }
    }
}
