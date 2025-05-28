#ifndef UIWHEEL_H
#define UIWHEEL_H

#include<string>
#include "Tools.h"
#include "UI/UIUtility.h"
extern  "C"{
    #include "raylib.h"
}
class Detail{
private:
    std::wstring text;
    Color backgroundColor,textColor;
    int fontSize;
    Rectangle box={0,0,0,0};
public:
    Detail(const std::wstring& str=L"",const Color& bColor=BLANK,const Color& tColor=BLACK,int fSize=UI::ButtonCFG::EXPLAIN_FONTSIZE);
    ~Detail()=default;
    const Detail& operator=(const std::wstring& str){text=str;return *this;}
    const bool operator==(const std::wstring& str){return text==str;}
    void setRectangle(const Rectangle& rect){box=rect;}
    void setBackgroundColor(const Color& color){backgroundColor=color;}
    void setTextColor(const Color& color){textColor=color;}
    void setFontSize(const int fs){fontSize=fs;}
    void setText(const std::wstring& t){text=t;}
    void setText(const std::string& t){setText(Trans::UTFTowstr(t));}
    void draw()const;
};

#endif