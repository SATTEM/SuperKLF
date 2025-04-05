#include "UI/UIwheel.h"
#include "UI/UIUtility.h"
#include "ResourceManager.h"
void Detail::draw()const{
    std::string textUTF=UI::wstrToUTF(text);
    Vector2 textSize=
    MeasureTextEx(ResourceManager::Get().getFont(textUTF),textUTF.c_str(),fontSize,1);
    UI::drawInTop([&,textSize]()->void{
        DrawRectangleRec(box,backgroundColor);
        UI::drawText(text,box.x+textSize.x/2.f,box.y,fontSize,textColor);});
}
Detail::Detail(const std::wstring& str,const Color& bColor,const Color& tColor,const int len,int fSize)
:text(str),backgroundColor(bColor),fontSize(fSize),textColor(tColor),length(len){
    text=UI::autoSplit(text,length);
}