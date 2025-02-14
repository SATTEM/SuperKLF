#ifndef GAMESTAGEFWD_H
#define GAMESTAGEFWD_H
enum class Occasion{
	OnShoot,
	OnHit,
	Instant,
	Always,
	OnBattleStart,
	OnPlayerHit,
	OnHPThreshold
};
enum class GameStage{MainMenu,Battle,Victory,Defeat,Pause,EXIT};
enum class EventSignal{IDLE,EXIT,RESTART};
#endif