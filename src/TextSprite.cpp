#include "TextSprite.h"

TextSprite::TextSprite(std::string name, std::string tg)
{

}

TextSprite::~TextSprite()
{
    //dtor
}

void TextSprite::setText(std::string txt)
{
    this->txt = txt;
}

std::string TextSprite::getText()
{
    return this->txt;
}
