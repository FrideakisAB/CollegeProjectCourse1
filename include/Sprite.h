#ifndef SPRITE_H
#define SPRITE_H

#include <string>

using namespace std;

class Sprite
{
public:
    bool active = true;
    float x, y, z, sx, sy;

    Sprite(string name, string tg = "") {
        this->name = name; this->tag = tg;
    }

    void setTexture(string text, string sh = "Standart") {
        this->texture = text;
        this->shader = sh;
    }

    string getShader() {
        return this->shader;
    }

    string getTexure() {
        return this->texture;
    }

    string getTag() {
        return this->tag;
    }

    void setTag(string t) {
        this->tag = t;
    }

    string getName() {
        return this->name;
    }

    void setName(string t) {
        this->name = t;
    }
private:
    string texture, name, tag, shader;
};

#endif // SPRITE_H
