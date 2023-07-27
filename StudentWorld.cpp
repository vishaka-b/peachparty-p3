#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <list>
#include <cstdlib>

using namespace std;


GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_balance = 0;
}

int StudentWorld::init()
{
    //use board class to load board file

    string board_file = assetPath() + "board0" + to_string(getBoardNumber()) + ".txt";
    Board::LoadResult result = board.loadBoard(board_file);
    if (result == Board::load_fail_file_not_found)
        return GWSTATUS_BOARD_ERROR;
    else if (result == Board::load_fail_bad_format)
        return GWSTATUS_BOARD_ERROR;
    //create new objects based off of board file contents
    else if (result == Board::load_success) {
        for (int r = 0; r < BOARD_HEIGHT; r++) {
            for (int c = 0; c < BOARD_WIDTH; c++) {
                Board::GridEntry ge = board.getContentsOf(r, c);
                switch (ge) {
                case Board::player: {
                    Peach = new Player(IID_PEACH, r * SPRITE_WIDTH, c * SPRITE_HEIGHT, 1, this);
                    Yoshi = new Player(IID_YOSHI, r * SPRITE_WIDTH, c * SPRITE_HEIGHT, 2, this);
                    Actor* ptr = new CoinSquare(true, IID_BLUE_COIN_SQUARE, r * SPRITE_WIDTH, c * SPRITE_HEIGHT, this);
                    actorList.push_back(ptr);
                    break;
                }
                case Board::blue_coin_square: {
                    Actor* ptr = new CoinSquare(true, IID_BLUE_COIN_SQUARE, r * SPRITE_WIDTH, c * SPRITE_HEIGHT, this);
                    actorList.push_back(ptr);
                    break;
                }
                case Board::red_coin_square: {
                    Actor* ptr = new CoinSquare(false, IID_RED_COIN_SQUARE, r * SPRITE_WIDTH, c * SPRITE_HEIGHT, this);
                    actorList.push_back(ptr);
                    break;
                }
                case Board::up_dir_square: {  
                    Actor* ptr = new DirectionalSquare(GraphObject::up, IID_DIR_SQUARE, r * SPRITE_WIDTH, c * SPRITE_HEIGHT, this);
                    actorList.push_back(ptr);
                    break;
                }
                case Board::down_dir_square: {
                    Actor* ptr = new DirectionalSquare(GraphObject::down, IID_DIR_SQUARE, r * SPRITE_WIDTH, c * SPRITE_HEIGHT, this);
                    actorList.push_back(ptr);
                    break;
                }
                case Board::left_dir_square: {
                    Actor* ptr = new DirectionalSquare(GraphObject::left, IID_DIR_SQUARE, r * SPRITE_WIDTH, c * SPRITE_HEIGHT, this);
                    actorList.push_back(ptr);
                    break;
                }
                case Board::right_dir_square: {
                    Actor* ptr = new DirectionalSquare(GraphObject::right, IID_DIR_SQUARE, r * SPRITE_WIDTH, c * SPRITE_HEIGHT, this);
                    actorList.push_back(ptr);
                    break;
                }
                case Board::event_square: {
                    Actor* ptr = new EventSquare(IID_EVENT_SQUARE, r * SPRITE_WIDTH, c * SPRITE_HEIGHT, this);
                    actorList.push_back(ptr);
                    break;
                }
                case Board::bank_square: {
                    Actor* ptr = new BankSquare(IID_BANK_SQUARE, r * SPRITE_WIDTH, c * SPRITE_HEIGHT, this);
                    actorList.push_back(ptr);
                    break;
                }
                case Board::star_square: {
                    Actor* ptr = new StarSquare(IID_STAR_SQUARE, r * SPRITE_WIDTH, c * SPRITE_HEIGHT, this);
                    actorList.push_back(ptr);
                    break;
                }
                case Board::bowser: {
                    Actor* ptr = new Bowser(IID_BOWSER, r * SPRITE_WIDTH, c * SPRITE_HEIGHT, this);
                    actorList.push_back(ptr);
                    Actor* ptr1 = new CoinSquare(true, IID_BLUE_COIN_SQUARE, r * SPRITE_WIDTH, c * SPRITE_HEIGHT, this);
                    actorList.push_back(ptr1);
                    break;
                }
                case Board::boo: {
                    Actor* ptr = new Boo(IID_BOO, r * SPRITE_WIDTH, c * SPRITE_HEIGHT, this);
                    actorList.push_back(ptr);
                    Actor* ptr1 = new CoinSquare(true, IID_BLUE_COIN_SQUARE, r * SPRITE_WIDTH, c * SPRITE_HEIGHT, this);
                    actorList.push_back(ptr1);
                    break;
                }

                default:
                    break;
                }
            }
        }
    }
    //start 99 seconds timer
	startCountdownTimer(99); 
    return GWSTATUS_CONTINUE_GAME;
}

//if board is empty at parameter coordinates, return true
bool StudentWorld::isBoardEmpty(int x, int y)  {
    if (board.getContentsOf(x, y) == Board::empty)
        return true;
    else
        return false;
}

Player* StudentWorld::getPeach() const {
    return Peach;
}

Player* StudentWorld::getYoshi() const {
    return Yoshi;
}
//gets random number between 0 and number of squares and find square at index of random number and return its coordinates
void StudentWorld::getRandomSquare(int& x, int& y) {
    int num = 0;
    list <Actor*>::iterator it;
    it = actorList.begin();
    while (it != actorList.end()) {
        if ((*it)->isSquare()) {
            num++;
        }
        it++;
    }

    if (num == 0) {
        return;
    }

    int random = randInt(1, num);
    int count = 0;
    list <Actor*>::iterator it2;
    it2 = actorList.begin();
    while (it2 != actorList.end()) {
        if ((*it2)->isSquare()) {
            count++;
            if (count == random) {
                x = (*it2)->getX();
                y = (*it2)->getY();
            }
                
        }
        it2++;
    }
}
//iterate through list and if there is a square at parameter coordinates, return pointer to the square
Actor* StudentWorld::isOnSquare(int x, int y) {
    list <Actor*>::iterator it;
    it = actorList.begin();
    while (it != actorList.end()) {
        if ((*it)->isSquare() && (*it)->getX() == x && (*it)->getY() == y) {
            return (*it);
        }
        it++;
    }
    return nullptr;
}
//iterate through list and if the actor is a directional square and matches parameter coordinates, return pointer to square
Actor* StudentWorld::isADirectional(int x, int y) {
    list <Actor*>::iterator it;
    it = actorList.begin();
    while (it != actorList.end()) {
        if ((*it)->canChangeDirection() && (*it)->getX() == x && (*it)->getY() == y) {
            return (*it);
        }
        it++;
    }
    return nullptr;
}

//iterate through list and if a baddie, if the difference between coordinates is less than 16, return pointer to baddie
Actor* StudentWorld::isOverlapping(int x, int y) {
    list <Actor*>::iterator it;
    it = actorList.begin();
    while (it != actorList.end()) {
        if ((*it)->isImpactable()) {
            int a = (*it)->getX();     //a: bowser's x coordinate      x: vortex's x coordinate
            int b = (*it)->getY();     //b: bowser's y coordinate      y: vortex's y coordinate
            if (abs(a-x) <= 15 && abs(b-y) <= 15) {
                return (*it);
            }

        }
        it++;
    }
    return nullptr;
}
//add dropping square at location
void StudentWorld::addDropping(int x, int y) {
    Actor* ptr = new DroppingSquare(IID_DROPPING_SQUARE, x, y, this);
    actorList.push_back(ptr);
  
}

int StudentWorld::getBalance() const {
    return m_balance;
}

void StudentWorld::setBalance(int amount) {
    m_balance = amount;
}
//add vortex at location
void StudentWorld::addVortex(int x, int y, int dir) {
    Actor* ptr = new Vortex(IID_VORTEX, x, y, dir, this);
    actorList.push_back(ptr);
}


int StudentWorld::move()
{
    
    
    //have players do something

    Yoshi->doSomething();
    Peach->doSomething();
    

    //iterate through Actors list and ask them to doSomething
    list <Actor*>::iterator it;
    it = actorList.begin();
    while (it != actorList.end()) {
        if ((*it)->isActive()) {
            (*it)->doSomething();
        }
        it++;
    }

    //delete inactive actors (iterate through list and if inactive, delete)

    list <Actor*>::iterator it3;
    it3 = actorList.begin();

    while (it3 != actorList.end()) {
        if (!((*it3)->isActive())) {
            delete* it3;
            it3 = actorList.erase(it3);
        }
        else {
            it3++;
        }
    }

    //update status text at top of screen

    string text = "";
    text += "P1 Roll: ";
    text += to_string(Peach->getRoll());
    text += " Stars: ";
    text += to_string(Peach->getStars());
    text += " $$: ";
    text += to_string(Peach->getCoins());
    if (Peach->hasVortex())
        text += " VOR";
    text += " | Time: ";
    text += to_string(timeRemaining());
    text += " | Bank: ";
    text += to_string(getBalance());
    text += " | ";
    text += "P2 Roll: ";
    text += to_string(Yoshi->getRoll());
    text += " Stars: ";
    text += to_string(Yoshi->getStars());
    text += " $$: ";
    text += to_string(Yoshi->getCoins());
    if (Yoshi->hasVortex())
        text += " VOR";

    setGameStatText(text);
    //if game is over, choose winners based off highest number of stars, then coins, then random winner
    if (timeRemaining() <= 0) {
        playSound(SOUND_GAME_FINISHED);
        if (Peach->getStars() > Yoshi->getStars()) {
            setFinalScore(Peach->getStars(), Peach->getCoins());
            return GWSTATUS_PEACH_WON;
        }
        else if (Peach->getStars() < Yoshi->getStars()) {
            setFinalScore(Yoshi->getStars(), Yoshi->getCoins());
            return GWSTATUS_YOSHI_WON;
        }
        else {
            if (Peach->getCoins() > Yoshi->getCoins()) {
                setFinalScore(Peach->getStars(), Peach->getCoins());
                return GWSTATUS_PEACH_WON;
            }
            else if (Peach->getCoins() < Yoshi->getCoins()) {
                setFinalScore(Yoshi->getStars(), Yoshi->getCoins());
                return GWSTATUS_YOSHI_WON;
            }
            else {
                int n = randInt(1, 2);
                setFinalScore(Yoshi->getStars(), Yoshi->getCoins());
                if (n == 1) 
                    return GWSTATUS_PEACH_WON;
                else
                    return GWSTATUS_YOSHI_WON;

            }
        }

    }

	return GWSTATUS_CONTINUE_GAME;
}

//delete both players and every actor in the list
void StudentWorld::cleanUp()
{
    delete Peach;
    delete Yoshi;
    list <Actor*>::iterator it;
    it = actorList.begin();
    while (it != actorList.end()) {
        delete *it;
        it = actorList.erase(it);
    }

}


StudentWorld::~StudentWorld() {
    cleanUp();
}