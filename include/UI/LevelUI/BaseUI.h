#ifndef BASEUI_H
#define BASEUI_H
class BaseUI{
public:
	virtual ~BaseUI()=default;
	BaseUI()=default;
	BaseUI(const BaseUI&)=delete;
	const BaseUI& operator=(const BaseUI&)=delete;
	//基本状态的绘制
	virtual void Draw()const;
};

#endif