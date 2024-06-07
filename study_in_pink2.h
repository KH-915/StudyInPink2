/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2023
* Author: Vu Van Tien
* Date: 02.02.2023
*/

//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class MovingObject;
class Position;
class Configuration;
class Map;

class Criminal;
class RobotS;
class RobotW;
class RobotSW;
class RobotC;

class ArrayMovingObject;
class StudyPinkProgram;

class BaseItem;
class BaseBag;
class SherlockBag;
class WatsonBag;

class TestStudyInPink;

enum ItemType { MAGIC_BOOK, ENERGY_DRINK, FIRST_AID, EXCEMPTION_CARD, PASSING_CARD };
enum ElementType { PATH, WALL, FAKE_WALL };
enum RobotType { C=0, S, W, SW };

class MapElement {
    friend class TestStudyInPink;
protected:
    ElementType type;
public:
    MapElement(ElementType in_type);
    virtual ~MapElement(){};
    virtual ElementType getType() const;
};

class Path : public MapElement {
    friend class TestStudyInPink;
    public:
    Path();
};

class Wall : public MapElement {
    friend class TestStudyInPink;
    public:
    Wall();
};

class FakeWall : public MapElement {
    friend class TestStudyInPink;
    private:
    int req_exp;
    public:
    FakeWall(int in_req_exp);
    int getReqExp() const;
};

class Map {
    friend class TestStudyInPink;
private:
    int num_rows, num_cols;
    MapElement ***map;
public:
    Map(int num_rows, int num_cols, int num_walls, Position * array_walls, int num_fake_walls, Position * array_fake_walls);
    ~Map();

    bool isValid ( const Position & pos , MovingObject * mv_obj ) const ;
};

class Position {
    friend class TestStudyInPink;
private:
    int r, c;
public:
    Position (int r =0 , int c =0) ;
    Position(const string & str_pos);

    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);

    string str() const;

    bool isEqual(int in_r, int in_c) const;
    bool isEqual(const Position &pos) const;
    static const Position npos;
};

class MovingObject {
    friend class TestStudyInPink;
protected:
    int index;
    Position pos;
    Map * map;
    string name;

public:
    MovingObject(int index, const Position pos, Map * map, const string & name="");
    virtual ~MovingObject(){};
    virtual Position getNextPosition() = 0;
    Position getCurrentPosition() const;
    virtual void move() = 0;
    virtual string str() const = 0;
    string getName() const;
    bool isValid(const Position & pos, MovingObject * mv_obj) const;
};
class Character : public MovingObject {
    friend class TestStudyInPink;
    public:
    Character(int index, const Position &init_pos, Map *map, const string &name = "");
    virtual ~Character(){};
};

class Sherlock : public Character{
    friend class TestStudyInPink;
private:
	const string moving_rule;
	string step;
	int hp, exp;
public:
    Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp);
    Position getNextPosition();
	void move();
	string str() const;
    int getExp() const;
    int getHp() const;
    void setExp(int exp);
    void setHp(int hp);

    // ...
};

class Watson : public Character
 {friend class TestStudyInPink;
private:
	const string moving_rule;
	string step;
	int hp, exp;
    Position prev_pos;
public:
    Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp);
    Position getNextPosition();
	void move();
	string str() const;
    int getExp() const;
    int getHp() const;
    void setExp(int exp);
    void setHp(int hp);
    // ...
};

class Criminal : public Character
 {friend class TestStudyInPink;
private:
	const string moving_rule;
	string step;
	int hp, exp;
    Position prev_pos;
    Sherlock* sherlock;
    Watson* watson;
public:
    Criminal(int index, const Position & init_pos, Map * map, Sherlock * sherlock, Watson * watson);
    Position getNextPosition();
	void move();
	string str() const;
    Position getPrevPosition();
};

class ArrayMovingObject {
    friend class TestStudyInPink;
private:
    MovingObject **arr_mv_objs;
    int count,capacity;
public:
    ArrayMovingObject(int capacity);

    ~ArrayMovingObject() ;
    bool isFull() const;
    bool add(MovingObject * mv_obj);
    MovingObject * get(int index) const;
    int size() const; // return current number of elements in the array
    string str() const;
};

class Configuration {
    friend class TestStudyInPink;
    friend class StudyPinkProgram;
private:
    int map_num_rows;
	int map_num_cols;
	int max_num_moving_objects;
	int num_walls;
	Position *arr_walls;
    string arr_walls_str;
	int num_fake_walls;
	Position *arr_fake_walls;
    string arr_fake_walls_str;
	string sherlock_moving_rule;
	Position sherlock_init_pos;
	int sherlock_init_hp;
	int sherlock_init_exp;
	string watson_moving_rule;
	Position watson_init_pos;
	int watson_init_hp;
	int watson_init_exp;
	Position criminal_init_pos;
	int num_steps;
public:
    Configuration(const string & filepath);
    ~Configuration();
    string str() const;
};

// Robot, BaseItem, BaseBag,...
class Robot:public MovingObject{
    friend class TestStudyInPink;
    private:
    RobotType robot_type;
    BaseItem *item;
    public:
    int getDistance() const;
    Robot(int index, const Position & init_pos, Map * map, RobotType type, const string &name);
    ~Robot();
};
class RobotC : public Robot {
    friend class TestStudyInPink;
  private:
	Criminal *criminal;
  public:
	RobotC(int index, const Position &init_pos, Map *map, Criminal *criminal);
	Position getNextPosition();
	int getDistance(Character *obj) const;
	string str() const;
    void move() override;
};

class RobotS : public Robot {
    friend class TestStudyInPink;
  private:
	Criminal *criminal;
	Sherlock *sherlock;
  public:
	RobotS(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock);
	Position getNextPosition();
	int getDistance() const;
    void move() override;
    string str() const;
};

class RobotW : public Robot {
    friend class TestStudyInPink;
  private:
	Criminal *criminal;
	Watson *watson;
  public:
	RobotW(int index, const Position &init_pos, Map *map, Criminal *criminal, Watson *watson);
	Position getNextPosition();
	int getDistance() const;
    void move() override;
    string str() const;
};

class RobotSW : public Robot {
    friend class TestStudyInPink;
  private:
	Criminal *criminal;
	Sherlock *sherlock;
	Watson *watson;
  public:
	RobotSW(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson);
	Position getNextPosition();
	int getDistance() const;
    void move() override;
    string str() const;
};

class BaseItem{
    friend class TestStudyInPink;
    private:
    ItemType item_type;
    public:
    BaseItem(ItemType item_type);
    ItemType getType() const;
    virtual bool canUse(Character* obj, Robot * robot) = 0;
    virtual void use(Character* obj, Robot * robot) = 0;
};
class MagicBook:public BaseItem{
    friend class TestStudyInPink;
    public:
    MagicBook();
    virtual bool canUse(Character* obj, Robot * robot) ;
    virtual void use(Character* obj, Robot * robot) ;
};
class EnergyDrink:public BaseItem{
    friend class TestStudyInPink;
    public:
    EnergyDrink();
    virtual bool canUse(Character* obj, Robot * robot) ;
    virtual void use(Character* obj, Robot * robot) ;
};
class FirstAid:public BaseItem{
    friend class TestStudyInPink;
    public:
    FirstAid();
    virtual bool canUse(Character* obj, Robot * robot) ;
    virtual void use(Character* obj, Robot * robot) ;
};
class ExcemptionCard:public BaseItem{
    friend class TestStudyInPink;
    public:
    ExcemptionCard();
    virtual bool canUse(Character* obj, Robot * robot) ;
    virtual void use(Character* obj, Robot * robot) ;
};
class PassingCard:public BaseItem{
    friend class TestStudyInPink;
    private:
    const string &challenge;
    public:
    PassingCard(const string &challenge);
    virtual bool canUse(Character* obj, Robot * robot) ;
    virtual void use(Character* obj, Robot * robot) ;
};
class BaseBag{
    friend class TestStudyInPink;
    private:
    Character *obj;
    int capacity,count;
    BaseItem *items;
    public:
    BaseBag(Character *obj, int capacity);
    ~BaseBag();
    virtual bool insert (BaseItem* item); //return true if insert successfully
    virtual BaseItem* get(Robot *robot); //return the item as described above, if notfound, return NULL
    virtual BaseItem* get(ItemType itemType,Robot *robot); //return the item as described above, if not found, return NULL
    virtual string str() const;
};
class SherlockBag : public BaseBag{
    friend class TestStudyInPink;
    SherlockBag(Sherlock * sherlock);
};
class WatsonBag : public BaseBag{
    friend class TestStudyInPink;
    WatsonBag(Watson * watson);
};
class StudyPinkProgram {
    friend class TestStudyInPink;
private:
    // Sample attributes
    Configuration * config;
    
    Sherlock * sherlock;
    Watson * watson;
    Criminal * criminal;
    
    SherlockBag *sherlock_bag;
    WatsonBag *watson_bag;

    Map * map;
    ArrayMovingObject * arr_mv_objs;


public:
    StudyPinkProgram(const string & config_file_path);

    bool isStop() const;

    void printResult() const {
        if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
            cout << "Sherlock caught the criminal" << endl;
        }
        else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
            cout << "Watson caught the criminal" << endl;
        }
        else {
            cout << "The criminal escaped" << endl;
        }
    }

    void printStep(int si) const {
        cout << "Step: " << setw(4) << setfill('0') << si
            << "--"
            << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
    }

    void run(bool verbose) {
        // Note: This is a sample code. You can change the implementation as you like.
        // TODO
        for (int istep = 0; istep < config->num_steps; ++istep) {
            for (int i = 0; i < arr_mv_objs->size(); ++i) {
                arr_mv_objs->get(i)->move();
                if (isStop()) {
                    printStep(istep);
                    break;
                }
                if (verbose) {
                    printStep(istep);
                }
            }
        }
        printResult();
    }

    ~StudyPinkProgram();
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
