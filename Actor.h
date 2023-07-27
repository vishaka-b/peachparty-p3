#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"



class StudentWorld;


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject {
public:
	Actor(int ID, int startX, int startY, int startDir, int depth, StudentWorld* ptr);
	virtual void doSomething() = 0;
	bool isActive() const;
	void setActive(bool b);
	void setTicks(int ticks);
	int getTicks() const;
	void setWalkDir(int dir);
	int getWalkDir() const;
	bool canMove(int curr, int& xnew, int& ynew);
	void getMove(int curr, int& walkDir, int& spriteDir, int& xnew, int& ynew);
	virtual bool hasBalance() const;
	StudentWorld* getWorld() const;
	virtual bool isImpactable() const;
	virtual bool isSquare() const;
	virtual bool canChangeDirection() const;
	bool isAtFork();
	virtual void Impact();
	//Board* getBoard() const;
	
private:
	//int spriteDir;
	bool active;
	int ticks_to_move;
	int walk_direction;
//	Board* ptrToBoard;
	StudentWorld* ptrToWorld;
	
};

class Player : public Actor {
public:
	Player(int ID, int startX, int startY, int playerNum, StudentWorld* ptr);
	virtual void doSomething();
	int getCoins() const;
	void addCoins(int coins);
	void takeCoins(int coins);
	int getStars() const;
	void addStars(int stars);
	void takeStars(int stars);
	bool hasVortex() const;
	void giveVortex();
	void takeVortex();
	bool isWaiting() const;
	void setWaiting(bool state);
	int getWaitCount() const;
	void swapCoins(Player* other);
	void swapStars(Player* other);
	void swapPosition(Player* other);
	void teleportToRandom(int x, int y);
	int getRoll() const;
private:
	int playerNumber; //player 1:left   player 2: right
	bool waitingToRoll; 
	int die_roll;
	int m_coins;
	int m_stars;
	bool hasAVortex;
	bool activated;
	int waitCount;
};

class Baddie : public Actor {
public:
	Baddie(int ID, int startX, int startY, StudentWorld* ptr);
	bool isPausedState() const;
	void setPausedState(bool state);
	void setPauseCounter(int count);
	virtual bool isImpactable() const;
	virtual void Impact();
	int pickRandomDirection();
	bool sameSquareAsPlayer(Player* player) const;
	void move(int squares);
	void changeMove();
	void resetOnSquare1(bool b);
	void resetOnSquare2(bool b);
	bool onSquare1() const;
	bool onSquare2() const;

private:
	int squares_to_move;
	bool isPaused;
	int pauseCounter;
	bool onSameSquare1;
	bool onSameSquare2;
};

class Boo : public Baddie {
public:
	virtual void doSomething();
	Boo(int ID, int startX, int startY, StudentWorld* ptr);
private:
};

class Bowser : public Baddie {
public:
	virtual void doSomething();
	Bowser(int ID, int startX, int startY, StudentWorld* ptr);

private:
};


class Square : public Actor {
public:
	Square(int ID, int startX, int startY, int startDir, int depth, StudentWorld* ptr);
	bool hasLandedOn(Player* player) const;
	bool hasMovedOver(Player* player) const;
	virtual bool isSquare() const;

private:
};

class CoinSquare : public Square {
public:
	CoinSquare(bool blue, int ID, int startX, int startY, StudentWorld* ptr);
	virtual void doSomething();
private:
	bool isCoinBlue;
};

class StarSquare : public Square {
public:
	StarSquare(int ID, int startX, int startY, StudentWorld* ptr);
	virtual void doSomething();
private:

};

class DirectionalSquare : public Square {
public:
	DirectionalSquare(int forcingDir, int ID, int startX, int startY, StudentWorld* ptr);
	virtual void doSomething();
	virtual bool canChangeDirection() const;

private:
	int m_forcingDir;
};

class BankSquare : public Square {
public:
	BankSquare(int ID, int startX, int startY, StudentWorld* ptr);
	virtual void doSomething();

private:
};

class EventSquare : public Square {
public:
	EventSquare(int ID, int startX, int startY, StudentWorld* ptr);
	virtual void doSomething();
private:

};

class DroppingSquare : public Square {
public:
	DroppingSquare(int ID, int startX, int startY, StudentWorld* ptr);
	virtual void doSomething();
private:

};

class Vortex : public Actor {
public:
	Vortex(int ID, int startX, int startY, int firingDirection, StudentWorld* ptr);
	virtual void doSomething();

private:
	int fireDirection;
};


#endif // ACTOR_H_
