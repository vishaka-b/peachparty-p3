#include "Actor.h"
#include "StudentWorld.h"
//#include "Board.h"


// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


//ACTOR IMPLEMENTATION

Actor::Actor(int ID, int startX, int startY, int startDir, int depth, StudentWorld* ptr) 
	: GraphObject(ID, startX, startY, startDir, depth) {
	ticks_to_move = 0;
	active = true;
	ptrToWorld = ptr;
	walk_direction = right;
}

bool Actor::isActive() const {
	return active;
}

void Actor::setActive(bool b) {
	active = b;
}

void Actor::setTicks(int ticks) {
	ticks_to_move = ticks;
}

int Actor::getTicks() const {
	return ticks_to_move;
}

void Actor::setWalkDir(int dir) {
	walk_direction = dir;
}
int Actor::getWalkDir() const {
	return walk_direction;
}

bool Actor::isImpactable() const {
	return false;
}

void Actor::Impact() {
	return;
}

bool Actor::canMove(int curr, int& xnew, int& ynew) {
	//if the position in front of you is not empty (using board), return true
	bool canMove = true;
	getPositionInThisDirection(curr, SPRITE_WIDTH, xnew, ynew);

	if ((getY() % SPRITE_WIDTH) == 0 && (getX() % SPRITE_WIDTH) == 0) {
		if (getWorld()->isBoardEmpty((xnew / SPRITE_WIDTH), (ynew / SPRITE_WIDTH))) {
			return false;
		}
	}
	return true;

}

void Actor::getMove(int curr, int& walkDir, int& spriteDir, int& xnew, int& ynew) {
		//if currently going up or down, if you can go right, set walking direction to right, else go left
		if (curr == up || curr == down) {
			getPositionInThisDirection(right, SPRITE_WIDTH, xnew, ynew);
			if (!(getWorld()->isBoardEmpty(xnew / SPRITE_WIDTH, ynew / SPRITE_WIDTH)))
				walkDir = right;
			else
				walkDir = left;
		}
		//if currently going left or right, if you can go up, set walking direction to up, else go down
		else if (curr == right || curr == left) {
			getPositionInThisDirection(up, 16, xnew, ynew);
			if (!(getWorld()->isBoardEmpty(xnew / SPRITE_WIDTH, ynew / SPRITE_WIDTH))) {
				walkDir = up;
			}
			else
				walkDir = down;
		}
		//update sprite direction
		if (walkDir == left)
			spriteDir = left;
		else
			spriteDir = right;

}

StudentWorld* Actor::getWorld() const {
	return ptrToWorld;
}

bool Actor::hasBalance() const {
	return false;
}

bool Actor::isSquare() const {
	return false;
}

bool Actor::canChangeDirection() const {
	return false;
}

bool Actor::isAtFork() {

	int count = 0;
	int xnew, ynew;

	//if you are not on a square (coordinates aren't multiples of 16), return false
	//if you can move in a direction, increment count
	//if count is more than 2 (the direction in front and behind you), that means you
	//are at a fork, so return true

	if (!((getY() % SPRITE_WIDTH) == 0 && (getX() % SPRITE_WIDTH) == 0))
		return false;

	getPositionInThisDirection(right, SPRITE_WIDTH, xnew, ynew);

	if (canMove(right, xnew, ynew)) {
		count++;
		
	}
	getPositionInThisDirection(up, SPRITE_WIDTH, xnew, ynew);

	if (canMove(up, xnew, ynew)) {
		count++;
		

	}
	getPositionInThisDirection(down, SPRITE_WIDTH, xnew, ynew);

	if (canMove(down, xnew, ynew)) {
		count++;
		

	}
	getPositionInThisDirection(left, SPRITE_WIDTH, xnew, ynew);

	if (canMove(left, xnew, ynew)) {
		count++;
		

	}

	if (count > 2) {
		return true;
	}
	return false;
	
}

//PLAYER IMPLEMENTATION

Player::Player(int ID, int startX, int startY, int playerNum, StudentWorld* ptr) : Actor(ID, startX, startY, right, 0, ptr) {
	waitingToRoll = true;
	playerNumber = playerNum;
	die_roll = 0;
	m_coins = 0;
	m_stars = 0;
	hasAVortex = false;
	activated = false;
	waitCount = 0;
}


void Player::doSomething() {
	int walkDir, spriteDir;
	int xnew, ynew;
	bool shouldMove = true;
	//if in waiting state, if you cannot move in current direction, change direction to perpendicular direction
	//if user presses roll key, set random die roll and set state to walking
	//is user presses vortex key and player has a vortex, fire a vortex, play sound, and take the vortex from player
	if (waitingToRoll) {

		if (canMove(getWalkDir(), xnew, ynew) == false && (!isAtFork())) {
			getMove(getWalkDir(), walkDir, spriteDir, xnew, ynew);
			setWalkDir(walkDir);
			setDirection(spriteDir);
		}
		waitCount++;
		int action = getWorld()->getAction(playerNumber);
		if (action == ACTION_ROLL) {
			die_roll = randInt(1, 10);
			setTicks(die_roll * 8);
			waitingToRoll = false;

		}
		
		if (action == ACTION_FIRE && hasVortex()) {
			if (getWalkDir() == right)
				getWorld()->addVortex(getX() + SPRITE_HEIGHT, getY(), getWalkDir());

			if (getWalkDir() == left)
				getWorld()->addVortex(getX() - SPRITE_HEIGHT, getY(), getWalkDir());

			if (getWalkDir() == up)
				getWorld()->addVortex(getX(), getY() + SPRITE_HEIGHT, getWalkDir());

			if (getWalkDir() == down)
				getWorld()->addVortex(getX(), getY() - SPRITE_HEIGHT, getWalkDir());

			getWorld()->playSound(SOUND_PLAYER_FIRE);
			takeVortex();
		}
		else
			return;
	}
	//if player is in waiting state, set waitCount to 0
	
	if (!waitingToRoll) {   
		waitCount = 0;
		bool notDirectional = true;

		//if at a directional square, do not activate fork branch
		if (getWorld()->isADirectional(getX(), getY()) != nullptr) {
			notDirectional = false;
		}
		//if at a fork, get action from player and if you can move in that direction (not where you originally came from),
		//set walking and sprite direction accordingly
		if (notDirectional && isAtFork()) {
			shouldMove = false;
			int mydir = getWorld()->getAction(playerNumber);
			if (mydir == ACTION_UP) {

				if (canMove(up, xnew, ynew) && getWalkDir() != down) {

					setWalkDir(up);
					setDirection(right);
					shouldMove = true;
				}
			}
			if (mydir == ACTION_RIGHT) {

				if (canMove(right, xnew, ynew) && getWalkDir() != left) {

					setWalkDir(right);
					setDirection(right);
					shouldMove = true;
				}
			}
			if (mydir == ACTION_LEFT) {

				if (canMove(left, xnew, ynew) && getWalkDir() != right) {
			

					setWalkDir(left);
					setDirection(left);
					shouldMove = true;
				}
			}
			if (mydir == ACTION_DOWN) {
				
				if (canMove(down, xnew, ynew) && getWalkDir() != up) {
					setWalkDir(down);
					setDirection(right);
					shouldMove = true;
				}
			}

		}
		//if cannot move in current direction, get perpendicular direction and set walking and sprite direction
		else if (canMove(getWalkDir(), xnew, ynew) == false) {  
			getMove(getWalkDir(), walkDir, spriteDir, xnew, ynew);
			setWalkDir(walkDir);
			setDirection(spriteDir);
		}
		//if not a fork, move 2 pixels forward and change ticks accordingly
		if (shouldMove) {
			moveAtAngle(getWalkDir(), 2);

			setTicks(getTicks() - 1);
			if (getTicks() == 0) {

				waitingToRoll = true;
			}
		}

	}
}


int Player::getCoins() const {
	return m_coins;
}

void Player::addCoins(int coins) {
	m_coins = m_coins + coins;
}

void Player::takeCoins(int coins) {
	if (coins >= m_coins)
		m_coins = 0;
	else
		m_coins = m_coins - coins;
}

int Player::getStars() const {
	return m_stars;
}

void Player::addStars(int stars) {
	m_stars = m_stars + stars;
}

void Player::takeStars(int stars) {
	if (stars >= m_stars)
		m_stars = 0;
	else
		m_stars = m_stars - stars;
}

bool Player::hasVortex() const {
	return hasAVortex;
}

void Player::giveVortex() {
	hasAVortex = true;
}

void Player::takeVortex() {
	hasAVortex = false;
}

bool Player::isWaiting() const {
	return waitingToRoll;
}

void Player::setWaiting(bool state) {
	waitingToRoll = state;
}

int Player::getWaitCount() const {
	return waitCount;
}

//create temporary variable to swap player's coins
void Player::swapCoins(Player* other) {
	int temp = other->getCoins();
	other->takeCoins(temp);
	other->addCoins(m_coins);
	m_coins = temp;
}
//create temporary variable to swap player's stars
void Player::swapStars(Player* other) {
	int temp = other->getStars();
	other->takeStars(temp);
	other->addStars(m_stars);
	m_stars = temp;
}
//create temporary variables to swap player's location, ticks, state, and both directions
void Player::swapPosition(Player* other) {
	int tempX = getX();
	int tempY = getY();
	moveTo(other->getX(), other->getY());
	other->moveTo(tempX, tempY);

	int tempTicks = other->getTicks();
	other->setTicks(getTicks());
	setTicks(tempTicks);

	int walkDir = other->getWalkDir();
	other->setWalkDir(getWalkDir());
	setWalkDir(walkDir);

	int spriteDir = other->getDirection();
	other->setDirection(getDirection());
	setDirection(spriteDir);

	bool tempWait = other->isWaiting();
	other->setWaiting(isWaiting());
	setWaiting(tempWait);

}

void Player::teleportToRandom(int x, int y) {
	moveTo(x, y);
}


int Player::getRoll() const {
	return die_roll;
}


Baddie::Baddie(int ID, int startX, int startY, StudentWorld* ptr) : Actor(ID, startX, startY, right, 0, ptr) {
	squares_to_move = 0;
	pauseCounter = 180;
	isPaused = true;
	onSameSquare1 = false;
	onSameSquare2 = false;
}



bool Baddie::isPausedState() const {
	return isPaused;
}

void Baddie::setPauseCounter(int count) {
	pauseCounter = count;
}

void Baddie::setPausedState(bool state) {
	isPaused = state;
}

bool Baddie::isImpactable() const {
	return true;
}

//moves baddie to random location and set walking and sprite direction
void Baddie::Impact() {
	//teleport to random square
	int x, y;
	getWorld()->getRandomSquare(x, y);
	moveTo(x, y);
	setWalkDir(right);
	setDirection(right);
	isPaused = true;
	pauseCounter = 180;
}

//if a player is in waiting state and has same coordinates as square, return true
bool Baddie::sameSquareAsPlayer(Player* player) const {
	if (player->isWaiting() && player->getX() == getX() &&
		player->getY() == getY()) {
		return true;
	}
	else {
		return false;
	}

}

//helper function for doSomething
//decrement pause counter and if 0, get random squares to move and move in random direction
void Baddie::move(int squares) {
	pauseCounter--;
	if (pauseCounter == 0) {
		squares_to_move = randInt(1, squares);
		setTicks(squares_to_move * 8);
		//UPDATE WALK AND SPRITE DIRECTION RANDOMLY
		setWalkDir(pickRandomDirection());
		if (getWalkDir() == left)
			setDirection(left);
		else
			setDirection(right);

		isPaused = false;
	}
}

//get number of possible directions and get random number between 0 and number of possible directions
//until you reach this number, find a direction and return the direction once number is reached
int Baddie::pickRandomDirection() {
	int count = 0;
	int xnew, ynew;
	getPositionInThisDirection(right, SPRITE_WIDTH, xnew, ynew);
	if (canMove(right, xnew, ynew)) {
		count++;
	}
	getPositionInThisDirection(up, SPRITE_WIDTH, xnew, ynew);
	if (canMove(up, xnew, ynew)) {
		count++;
	}
	getPositionInThisDirection(down, SPRITE_WIDTH, xnew, ynew);
	if (canMove(down, xnew, ynew)) {
		count++;
	}
	getPositionInThisDirection(left, SPRITE_WIDTH, xnew, ynew);
	if (canMove(left, xnew, ynew)) {
		count++;
	}
	int n = randInt(1, count);
	int check = 0;

		getPositionInThisDirection(right, SPRITE_WIDTH, xnew, ynew);
		if (canMove(right, xnew, ynew)) {
			check++;
		}
		if (check == n)
			return right;
		getPositionInThisDirection(up, SPRITE_WIDTH, xnew, ynew);
		if (canMove(up, xnew, ynew)) {
			check++;
		}
		if (check == n)
			return up;
		getPositionInThisDirection(down, SPRITE_WIDTH, xnew, ynew);
		if (canMove(down, xnew, ynew)) {
			check++;
		}
		if (check == n)
			return down;
		getPositionInThisDirection(left, SPRITE_WIDTH, xnew, ynew);
		if (canMove(left, xnew, ynew)) {
			check++;
		}
		return left;

}

void Baddie::resetOnSquare1(bool b) {
	onSameSquare1 = b;
}

void Baddie::resetOnSquare2(bool b) {
	onSameSquare2 = b;
}

bool Baddie::onSquare1() const {
	return onSameSquare1;
}
bool Baddie::onSquare2() const {
	return onSameSquare2;
}


void Baddie::changeMove() {
	//if at a fork and on a square, pick a random direction and set walking and sprite direction
	if (isAtFork() && getWorld()->isOnSquare(getX(), getY())) {
		setWalkDir(pickRandomDirection());
		if (getWalkDir() == left)
			setDirection(left);
		else
			setDirection(right);
	}
		
	//if on a square and cannot move in current direction, get perpendicular direction and set walking and sprite direction
	int xnew, ynew;
	int walkDir, spriteDir;
	if (getWorld()->isOnSquare(getX(), getY()) && !(canMove(getWalkDir(), xnew, ynew))) {
		getMove(getWalkDir(), walkDir, spriteDir, xnew, ynew);
		setWalkDir(walkDir);
		setDirection(spriteDir);
	}
	//move two pixels forward and decrement ticks
	moveAtAngle(getWalkDir(), 2);
	setTicks(getTicks() - 1);
}


Boo::Boo(int ID, int startX, int startY, StudentWorld* ptr) : Baddie(ID, startX, startY, ptr) {

}


Bowser::Bowser(int ID, int startX, int startY, StudentWorld* ptr) : Baddie(ID, startX, startY, ptr) {

}

void Boo::doSomething() {
	
	if (isPausedState()) {
		//if this is the first time peach and boo are on same square
		if (sameSquareAsPlayer(getWorld()->getPeach()) && !onSquare1()) {
				//swap coins or stars of players (randomly choose one) and play sound
				int n = randInt(1, 2);
				if (n == 1) {
					getWorld()->getPeach()->swapCoins(getWorld()->getYoshi());
				}
				else {
					getWorld()->getPeach()->swapStars(getWorld()->getYoshi());

				}
				getWorld()->playSound(SOUND_BOO_ACTIVATE);
				
				resetOnSquare1(true);
		}
		//if this is the first time yoshi and boo are on same square
		if (sameSquareAsPlayer(getWorld()->getYoshi()) && !onSquare2()) {
				//swap coins or stars of players (randomly choose one) and play sound

				int n = randInt(1, 2);
				if (n == 1) {
					getWorld()->getYoshi()->swapCoins(getWorld()->getPeach());
				}
				else {
					getWorld()->getYoshi()->swapStars(getWorld()->getPeach());

				}
				getWorld()->playSound(SOUND_BOO_ACTIVATE);
				
				resetOnSquare2(true);
		}
		
		move(3);
	}
	//reset that players have moved off the boo
	if (!getWorld()->getPeach()->isWaiting()) {

		resetOnSquare1(false);

	}
	if (!getWorld()->getYoshi()->isWaiting()) {

		resetOnSquare2(false);

	}
	//if in walking state, change that boo has moved off players and change move
	if (!isPausedState()) {
		resetOnSquare1(false);
		resetOnSquare2(false);

		changeMove();
		if (getTicks() == 0) {
			setPausedState(true);
			setPauseCounter(180);
		}
	}
}

void Bowser::doSomething() {
	if (isPausedState()) {
		//if this is the first time peach and bowser are on same square

		if (sameSquareAsPlayer(getWorld()->getPeach()) && !onSquare1()) {
			
			//   1/2 chance that players coins and stars are set to 0
			int n = randInt(1, 2);
			if (n == 1) {
				getWorld()->getPeach()->takeCoins(getWorld()->getPeach()->getCoins());
				getWorld()->getPeach()->takeStars(getWorld()->getPeach()->getStars());

				getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
			}
			
			resetOnSquare1(true);

		}
		//if this is the first time peach and bowser are on same square

		if (sameSquareAsPlayer(getWorld()->getYoshi()) && !onSquare2()) {
			
			//   1/2 chance that players coins and stars are set to 0
			int n = randInt(1, 2);
			if (n == 1) {
				getWorld()->getYoshi()->takeCoins(getWorld()->getYoshi()->getCoins());
				getWorld()->getYoshi()->takeStars(getWorld()->getYoshi()->getStars());
				getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
			}
			
			resetOnSquare2(true);

		}

		move(10);
	}
	//reset that players have moved off bowser
	if (!getWorld()->getPeach()->isWaiting()) {
		resetOnSquare1(false);
	}
	if (!getWorld()->getYoshi()->isWaiting()) {
		resetOnSquare2(false);
	}
	//if in walking state, reset that bowser has moved off players and change move
	if (!isPausedState()) {
		resetOnSquare1(false);
		resetOnSquare2(false);
		changeMove();

		//if no more ticks left, 1/4 chance that bowser changes the square it is on to a s dropping square
		if (getTicks() == 0) {
			setPausedState(true);
			setPauseCounter(180);
			int n = randInt(1,4);
			 if (n == 1) {
				 if (getWorld()->isOnSquare(getX(), getY()) != nullptr) {
					 getWorld()->isOnSquare(getX(), getY())->setActive(false);
					 getWorld()->addDropping(getX(), getY());
					 getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
				 }

			}

		}
	}
}


//SQUARE IMPLEMENTATION

Square::Square(int ID, int startX, int startY, int startDir, int depth, StudentWorld* ptr)
	: Actor(ID, startX, startY, startDir, depth, ptr) {}

bool Square::isSquare() const {
	return true;
}

//if player is in waiting state and coordinates of player and square match, return true
bool Square::hasLandedOn(Player* player) const {

	if (player->isWaiting() && player->getX() == getX() &&
		player->getY() == getY() && player->getWaitCount() == 1) {
		return true;
	}
	else {
		return false;
	}
}
//if player is in walking state and coordinates match the square coordinates, return true
bool Square::hasMovedOver(Player* player) const {

	if (!(player->isWaiting()) && player->getX() == getX() &&
		player->getY() == getY() && (player->getX() % SPRITE_HEIGHT == 0) && (player->getY() % SPRITE_HEIGHT == 0)) {
		return true;
	}
	else {
		return false;
	}
}

CoinSquare::CoinSquare(bool blue, int ID, int startX, int startY, StudentWorld* ptr) : Square(ID, startX, startY, right, 1, ptr) {
	isCoinBlue = blue;
}

//if landed on a player, add coins and play sound if blue coin, and take coins and play sound if red coin
void CoinSquare::doSomething() {
	if (!isActive())
		return;


	Player* peach = getWorld()->getPeach();
	Player* yoshi = getWorld()->getYoshi();
	if (!hasLandedOn(peach) && !hasLandedOn(yoshi)) {
		return;
	}

	if (isCoinBlue) {
		if (hasLandedOn(peach)) {
			peach->addCoins(3);
			getWorld()->playSound(SOUND_GIVE_COIN);
		}
		if (hasLandedOn(yoshi)) {
			yoshi->addCoins(3);
			getWorld()->playSound(SOUND_GIVE_COIN);
		}
	}

	else {
		if (hasLandedOn(peach)) {
			peach->takeCoins(3);
			getWorld()->playSound(SOUND_TAKE_COIN);

		}
		if (hasLandedOn(yoshi)) {
			yoshi->takeCoins(3);
			getWorld()->playSound(SOUND_TAKE_COIN);

		}
	}
	return;
}


StarSquare::StarSquare(int ID, int startX, int startY, StudentWorld* ptr) : Square(ID, startX, startY, right, 1, ptr) {

}

//if landed on or moved over, add a star if current coins are 20 or more
void StarSquare::doSomething() {

	//ptr1 is peach, ptr2 is yoshi
	Player* ptr1 = getWorld()->getPeach();
	Player* ptr2 = getWorld()->getYoshi();
	if (!hasLandedOn(ptr1) && !hasLandedOn(ptr2) && !hasMovedOver(ptr1) && !hasMovedOver(ptr2)) {
		return;
	}

	if (hasLandedOn(ptr1) || hasMovedOver(ptr1)) {
		if (ptr1->getCoins() < 20)
			return;
		else {
			ptr1->takeCoins(20);
			ptr1->addStars(1);
			getWorld()->playSound(SOUND_GIVE_STAR);
			return;
		}
	}
	if (hasLandedOn(ptr2) || hasMovedOver(ptr2)) {
		if (ptr2->getCoins() < 20)
			return;
		else {
			ptr2->takeCoins(20);
			ptr2->addStars(1);
			getWorld()->playSound(SOUND_GIVE_STAR);
			return;
		}
	}

}

DirectionalSquare::DirectionalSquare(int forcingDir, int ID, int startX, int startY, StudentWorld* ptr) : Square(ID, startX, startY, forcingDir, 1, ptr) {
	m_forcingDir = forcingDir;
}

//if player moves over or lands on square, change its walking direction 
void DirectionalSquare::doSomething() {

	Player* ptr1 = getWorld()->getPeach();
	Player* ptr2 = getWorld()->getYoshi();
	if (!hasLandedOn(ptr1) && !hasLandedOn(ptr2) && !hasMovedOver(ptr1) && !hasMovedOver(ptr2)) {
		return;
	}

	if (hasLandedOn(ptr1) || hasMovedOver(ptr1)) {
		ptr1->setWalkDir(m_forcingDir);
		if (m_forcingDir == left) {
			ptr1->setDirection(left);
		}
		else
			ptr1->setDirection(right);
	}
	if (hasLandedOn(ptr2) || hasMovedOver(ptr2)) {
		ptr2->setWalkDir(m_forcingDir);
		if (m_forcingDir == left) {
			ptr2->setDirection(left);
		}
		else
			ptr2->setDirection(right);
	}

}

bool DirectionalSquare::canChangeDirection() const {
	return true;
}

BankSquare::BankSquare(int ID, int startX, int startY, StudentWorld* ptr) : Square(ID, startX, startY, right, 1, ptr) {
	
}
//if moved over, deduct 5 coins and add to bank balance
//if landed on, give all coins in balance to player
void BankSquare::doSomething() {

	Player* ptr1 = getWorld()->getPeach();
	Player* ptr2 = getWorld()->getYoshi();

	if (!hasLandedOn(ptr1) && !hasLandedOn(ptr2)) {
		if (hasMovedOver(ptr1) && ptr1->getTicks() <= (ptr1->getRoll() * 8) - 4) {
			
			int coins = ptr1->getCoins();
			if (coins < 5) {
				ptr1->takeCoins(coins);
				getWorld()->setBalance(getWorld()->getBalance() + coins);
			}
			else {
				ptr1->takeCoins(5);
				getWorld()->setBalance(getWorld()->getBalance() + 5);
			}
			getWorld()->playSound(SOUND_DEPOSIT_BANK);
			
		}
		if (hasMovedOver(ptr2) && ptr2->getTicks() <= (ptr2->getRoll() * 8) - 4) {

			int coins = ptr2->getCoins();
			if (coins < 5) {
				ptr2->takeCoins(coins);
				getWorld()->setBalance(getWorld()->getBalance() + coins);
			}
			else {
				ptr2->takeCoins(5);
				getWorld()->setBalance(getWorld()->getBalance() + 5);
			}
			getWorld()->playSound(SOUND_DEPOSIT_BANK);
			
		}
		
		return;
	}

	if (hasLandedOn(ptr1)) {
		ptr1->addCoins(getWorld()->getBalance());
		getWorld()->setBalance(0);
		getWorld()->playSound(SOUND_WITHDRAW_BANK);
	}
	if (hasLandedOn(ptr2)) {
		ptr2->addCoins(getWorld()->getBalance());
		getWorld()->setBalance(0);
		getWorld()->playSound(SOUND_WITHDRAW_BANK);
	}

}

EventSquare::EventSquare(int ID, int startX, int startY, StudentWorld* ptr) : Square(ID, startX, startY, right, 1, ptr) {
	
}
//if player lands on square, randomly choose between teleporting player, swapping player positions, or giving player a vortex
void EventSquare::doSomething() {

	Player* ptr1 = getWorld()->getPeach();
	Player* ptr2 = getWorld()->getYoshi();
	if (!hasLandedOn(ptr1) && !hasLandedOn(ptr2)) {
		return;
	}

	if (hasLandedOn(ptr1)) {
		int num = randInt(1, 3);
		switch (num) {
		case 1: {
			//call teleport on player
			int x, y;
			getWorld()->getRandomSquare(x, y);
			ptr1->teleportToRandom(x, y);
			getWorld()->playSound(SOUND_PLAYER_TELEPORT);
			break;
		}
		case 2: {
			ptr1->swapPosition(getWorld()->getYoshi());
			getWorld()->playSound(SOUND_PLAYER_TELEPORT);
			break;
		}
		case 3: {
			ptr1->giveVortex();
			getWorld()->playSound(SOUND_GIVE_VORTEX);
			break;
		}

		}
	}
	if (hasLandedOn(ptr2)) {
		int num = randInt(1, 3);
		switch (num) {
		case 1: {
			int x, y;
			getWorld()->getRandomSquare(x, y);
			ptr2->teleportToRandom(x, y);
			getWorld()->playSound(SOUND_PLAYER_TELEPORT);
			break;
		}
		case 2: {
			ptr2->swapPosition(getWorld()->getPeach());
			getWorld()->playSound(SOUND_PLAYER_TELEPORT);
			break;
		}
		case 3: {
			ptr2->giveVortex();
			getWorld()->playSound(SOUND_GIVE_VORTEX);
			break;
		}

		}
	}

}

DroppingSquare::DroppingSquare(int ID, int startX, int startY, StudentWorld* ptr) : Square(ID, startX, startY, right, 1, ptr) {

}

//if landed on, take 10 coins or take 1 star from player
void DroppingSquare::doSomething() {

	Player* ptr1 = getWorld()->getPeach();
	Player* ptr2 = getWorld()->getYoshi();
	if (!hasLandedOn(ptr1) && !hasLandedOn(ptr2)) {
		return;
	}

	if (hasLandedOn(ptr1)) {
		int num = randInt(1, 2);
		switch (num) {
		case 1: {
			ptr1->takeCoins(10);
			break;
		}
		case 2: {
			ptr1->takeStars(1);
			break;
		}
		}
		getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
	}
	if (hasLandedOn(ptr2)) {
		int num = randInt(1, 2);
		switch (num) {
		case 1: {
			ptr2->takeCoins(10);
			break;
		}
		case 2: {
			ptr2->takeStars(1);
			break;
		}
		}
		getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);

	}

}

Vortex::Vortex(int ID, int startX, int startY, int firingDirection, StudentWorld* ptr) : Actor(ID, startX, startY, firingDirection, 0, ptr) {
	//active is set true by actor constructor
	fireDirection = firingDirection;
}


void Vortex::doSomething() {
	if (!isActive())
		return;

	//move forward 2 pixels
	moveAtAngle(fireDirection, 2);

	//if reaches end of screen, set vortex to inactive
	if (getX() < 0 || getX() >= VIEW_WIDTH || getY() < 0 || getY() >= VIEW_WIDTH) {
		setActive(false);
	}

	
	//if vortex hits a baddie, call impact on baddie and set vortex to inactive
	Actor* ptr = getWorld()->isOverlapping(getX(), getY());
	if (ptr != nullptr) {
		ptr->Impact();
		setActive(false);
		getWorld()->playSound(SOUND_HIT_BY_VORTEX);
	}

}