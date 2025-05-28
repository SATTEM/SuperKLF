#include "UI/UIwheel.h"
#include "UI/UIUtility.h"
#include "Tools.h"
void Detail::draw()const{
    std::string textUTF=Trans::wstrToUTF(text);
    UI::drawInTop([&]()->void{
        DrawRectangleRec(box,backgroundColor);
        UI::drawTextInRect(text, box, fontSize, textColor);});
}
Detail::Detail(const std::wstring& str,const Color& bColor,const Color& tColor,int fSize)
:text(str),backgroundColor(bColor),fontSize(fSize),textColor(tColor){}