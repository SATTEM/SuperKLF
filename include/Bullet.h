#ifndef BULLET_H
#define BULLET_H

#include "ResourceManager.h"
#include <vector>
extern "C"{
	#include "raylib.h"
}
#include <memory>
#include <string>
#include <unordered_map>
#include "Effect/BulletEffect.h"


class Entity;


namespace BULLET{
	//子弹
	const Vector2 BULLET_SIZE={240,240};
	const float BULLET_RADIUS=150.0f;
	//大招
	const float BLAST_DRAWSCALE=0.3f;
	//全局
	const int BASIC_SPEED=160 ;//基础速度，单位是每秒像素	
	const int BASIC_DMG=50;//基础伤害
}

//基础版本子弹，有子弹的基本功能，特效只有伤害
class Bullet{
protected:
	Vector2 position;
	Texture2D texture;
	std::string texturePath;
	Vector2 velocity;//速度倍率
	float drawScale=0.1f;//子弹缩放倍率
	int damage;
	float colliderRadius;
	bool active,isOnShoot=true,shouldRemove=false;
	std::vector<std::shared_ptr<BulletEffect>> effects;
	
public:
	~Bullet()=default;
	Bullet(const std::string texPath=ASSETS_IMAGE_PATH+std::string("pen.png"),const Vector2& vel={0,1},const Vector2& pos={0,0},const int dmg=BULLET::BASIC_DMG,const bool act=false);
	Bullet copy()const{return *this;}
	std::unique_ptr<Bullet> shoot(const Vector2& begin);
	void Update(const float deltaTime,Entity& shooter);
	void addEffect(std::shared_ptr<BulletEffect> effect){effects.push_back(std::move(effect));}
	void setDamage(const int dmg){damage=dmg;}
	void setDrawScale(const float val){drawScale=val;}
	void setVelocity(const Vector2& v){velocity=v;}
	const Vector2 getColliderCentre() const {return {position.x+int(texture.width*drawScale/2),position.y+int(texture.height*drawScale/2)};}
	const float getColliderRadius() const {return colliderRadius;}
	const bool shouldBeRemoved() const{return shouldRemove;}
	void DrawAsPattern(const Vector2& pos,float scale=0.f) const;
private:
	void Draw() const;
	void checkHit(Entity& shooter);
	void tryTriggerEffects(Entity& shooter,const Occasion& timing);
protected:
	Bullet(const Bullet& proto,const Vector2& begin);
	void countColliderRadius(const Vector2 size=BULLET::BULLET_SIZE);
};
class Blast:public Bullet{

public:
	Blast():Bullet(){}
	Blast(const std::string texPath,const Vector2& vel={0,1},const Vector2& pos={0,0},const int dmg=10*BULLET::BASIC_DMG,const bool act=false);
	Blast(const Blast& proto,const Vector2& begin):Bullet(*this,begin){}
	void setImage(const std::string path){texture=ResourceManager::Get().loadTexture(path);}
};

namespace BULLET{
	static std::unordered_map<std::string,std::unique_ptr<Bullet>> IDtoBullet;
	static std::unordered_map<std::string, std::unique_ptr<Blast>> IDtoBlast;
	std::unique_ptr<Bullet> getBullet(const std::string& id);
	std::unique_ptr<Blast> getBlast(const std::string& id);
}
#endif