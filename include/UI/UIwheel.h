#ifndef UIWHEEL_H
#define UIWHEEL_H

#include<string>
#include "UI/UIUtility.h"
extern  "C"{
    #include "raylib.h"
}
class Detail{
private:
    std::wstring text;
    Color backgroundColor,textColor;
    int length,fontSize;
    Rectangle box={0,0,0,0};
public:
    Detail(const std::wstring& str=L"",const Color& bColor=BLANK,const Color& tColor=BLACK,const int len=UI::RELIC_DISPLAY_MAX_LENGTH,int fSize=UI::EXPLAIN_FONTSIZE);
    void operator=(const std::wstring& str){text=str;}
    const bool operator==(const std::wstring& str){return text==str;}
    void setRectangle(const Rectangle& rect){box=rect;}
    void setBackgroundColor(const Color& color){backgroundColor=color;}
    void setTextColor(const Color& color){textColor=color;}
    void setFontSize(const int fs){fontSize=fs;}
    void setLineLength(const int len){length=len;text=UI::autoSplit(text,length);}
    void draw()const;
};

#endif