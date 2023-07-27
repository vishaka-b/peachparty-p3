#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GraphObject.h"
#include "Board.h"
#include "Actor.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  bool isBoardEmpty(int x, int y);    //squares
  Player* getPeach() const;
  Player* getYoshi() const;
  int getBalance() const;
  void setBalance(int amount);
  void getRandomSquare(int& x, int& y);
  Actor* isOnSquare(int x, int y);
  Actor* isADirectional(int x, int y);
  Actor* isOverlapping(int x, int y);
  void addDropping(int x, int y);
  void addVortex(int x, int y, int dir);
  virtual void cleanUp();
  ~StudentWorld();

private:
	std::list <Actor*> actorList;
	Player* Peach;
	Player* Yoshi;
	Board board;
	int m_balance;

};

#endif // STUDENTWORLD_H_
