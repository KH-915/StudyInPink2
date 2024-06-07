#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////
//3.1
MapElement::MapElement(ElementType in_type):type(in_type){}

ElementType MapElement::getType() const{return type;}

Path::Path():MapElement(PATH){}
Wall::Wall():MapElement(WALL){}
FakeWall::FakeWall(int in_req_exp):MapElement(FAKE_WALL),req_exp(in_req_exp){}

int FakeWall::getReqExp() const{return req_exp;}
//3.2
Map::Map(int num_rows, int num_cols, int num_walls, Position * array_walls, int num_fake_walls, Position * array_fake_walls):num_rows(num_rows),num_cols(num_cols){
map = new MapElement **[num_rows];
	for (int row = 0; row < num_rows; row++)
		map[row] = new MapElement *[num_cols];

	for (int i = 0; i < num_walls; i++)
		map[array_walls[i].getRow()][array_walls[i].getCol()] = new Wall();

	for (int i = 0; i < num_fake_walls; i++)
		map[array_fake_walls[i].getRow()][array_fake_walls[i].getCol()] =
			new FakeWall((array_fake_walls[i].getRow() * 257 + array_fake_walls[i].getCol() * 139 + 89) % 900 + 1);

	for (int row = 0; row < num_rows; row++)
		for (int col = 0; col < num_cols; col++)
			if (map[row][col] == nullptr)
				map[row][col] = new Path();
}
Map::~Map(){
  for(int r=0;r<num_rows;r++){
    delete[] map[r];
  }
  delete[] map;
}
//3.3
Position::Position(int r,int c):r(r),c(c){}

Position::Position(const string &str_pos){
  string r_str=str_pos.substr(1, str_pos.find(",") - 1);
  string c_str=str_pos.substr(str_pos.find(",") + 1);
  this->r=stoi(r_str);
  this->c=stoi(c_str);
}

int Position::getRow()const{return r;}
int Position::getCol()const{return c;}
void Position::setRow(int r){this->r=r;}
void Position::setCol(int c){this->c=c;}

string Position::str() const{return "("+to_string(r)+","+to_string(c)+")";}

bool Position::isEqual(int in_r, int in_c) const{return r==in_r&&c==in_c;}
bool Position::isEqual(const Position &pos) const{return r==pos.getRow()&&c==pos.getCol();}
//3.4
MovingObject::MovingObject(int index, const Position pos, Map * map, const string & name):
index(index), pos(pos), map(map), name(name){}

const Position Position::npos = Position(-1, -1);

Position MovingObject::getCurrentPosition() const{return pos;}

void MovingObject::move(){
  Position move=this->getNextPosition();
  if(!move.isEqual(Position::npos))
  pos=move;
}

string MovingObject::getName() const{return name;} 

bool Map::isValid(const Position & pos, MovingObject * mv_obj) const{
  if(pos.getCol()<0||pos.getCol()>=num_rows||pos.getRow()<0||pos.getRow()>=num_rows)
    return false;
  ElementType type=map[pos.getRow()][pos.getCol()]->getType();
  switch (type)
  {
  case ElementType::WALL:
    return false;
    break;
  case ElementType::PATH:
    return true;
    break;
  case ElementType::FAKE_WALL:
    if(mv_obj->getName()=="Watson"&&
    static_cast<FakeWall*>(map[pos.getRow()][pos.getCol()])->getType()<
    static_cast<Watson*>(mv_obj)->getExp())
      return false;
    else
      return true;
    break;
  default:
    return false;
    break;
  }
}
//3.5
Sherlock::Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp):Character(index,init_pos,map,"Sherlock"),step(moving_rule),moving_rule(moving_rule){
  hp=init_hp>500?500:init_hp<0?0:init_hp;
  exp=init_exp>500?500:init_exp<0?0:init_exp;
}

Position Sherlock::getNextPosition(){
  Position next_pos=pos;
  
  switch(step[0]){
	  case 'U':
		  next_pos.setRow(this->pos.getRow() - 1);
      break;
	  case  'D':
		  next_pos.setRow(this->pos.getRow() + 1);
      break;
	  case  'L':
		  next_pos.setCol(this->pos.getCol() - 1);
      break;
	  case  'R':
		  next_pos.setCol(this->pos.getCol() + 1);
      break;
  }

  step.erase(0,1).push_back(step[0]);

  if (this->map->isValid(next_pos, this))
		return next_pos;
  
  return Position::npos;
}

void Sherlock::move(){
  Position next_pos=this->getNextPosition();
  if(!next_pos.isEqual(Position::npos))
    this->pos=next_pos;
}

string Sherlock::str()const{return"Sherlock[index="+to_string(this->index)+";pos="+this->pos.str()+";moving_rule="+this->moving_rule+"]";}
//3.6
Watson::Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp):Character(index,init_pos,map,"Watson"),step(moving_rule),moving_rule(moving_rule){
  hp=init_hp>500?500:init_hp<0?0:init_hp;
  exp=init_exp>500?500:init_exp<0?0:init_exp;
}

Position Watson::getNextPosition(){
  Position next_pos=pos;
  
  switch(this->step[0]){
	  case 'U':
		  next_pos.setRow(this->pos.getRow() - 1);
      break;
	  case 'D':
		  next_pos.setRow(this->pos.getRow() + 1);
      break;
	  case 'L':
		  next_pos.setCol(this->pos.getCol() - 1);
      break;
	  case 'R':
		  next_pos.setCol(this->pos.getCol() + 1);
      break;
  }

  step.push_back(step[0]);

  step.erase(0,1);

  if (this->map->isValid(next_pos, this))
		return next_pos;
  
  return Position::npos;
}

void Watson::move(){
  if(!this->getNextPosition().isEqual(Position::npos))
    this->pos=this->getNextPosition();
}

string Watson::str()const{return"Watson[index="+to_string(this->index)+";pos="+this->pos.str()+";moving_rule="+this->moving_rule+"]";}
//3.7
Criminal::Criminal(int index, const Position & init_pos, Map * map, Sherlock * sherlock, Watson * watson):Character(index,init_pos,map,"Criminal"),sherlock(sherlock),watson(watson),prev_pos(init_pos){}

Position Criminal::getNextPosition(){
  Position next_pos=pos;
  Position sherlock_pos=sherlock->getCurrentPosition();
  Position watson_pos=watson->getCurrentPosition();
  if (sherlock_pos.getRow() < pos.getRow() && watson_pos.getRow() < pos.getRow())
	{
		next_pos.setRow(pos.getRow() + 1);
		if (map->isValid(next_pos, this))
			return next_pos;
	}

	if (sherlock_pos.getRow() < pos.getRow() && watson_pos.getRow() < pos.getRow())
	{
		next_pos.setRow(pos.getRow() - 1);
		if (map->isValid(next_pos, this))
			return next_pos;
	}

	if (sherlock_pos.getCol() < pos.getCol() && watson_pos.getCol() < pos.getCol())
	{
		next_pos.setCol(pos.getCol() + 1);
		if (map->isValid(next_pos, this))
			return next_pos;
	}

	if (sherlock_pos.getCol() < pos.getCol() && watson_pos.getCol() < pos.getCol())
	{
		next_pos.setCol(pos.getCol() - 1);
		if (map->isValid(next_pos, this))
			return next_pos;
	}

  next_pos.setRow(pos.getRow() - 1);
	if (this->map->isValid(next_pos, this))
		return next_pos;

	next_pos.setCol(pos.getCol() - 1);
	if (this->map->isValid(next_pos, this))
		return next_pos;

	next_pos.setRow(pos.getRow() + 1);
	if (this->map->isValid(next_pos, this))
		return next_pos;

	next_pos.setCol(pos.getCol() + 1);
	if (map->isValid(next_pos, this))
		return next_pos;

	return Position::npos;
}

void Criminal::move(){
  if(!this->getNextPosition().isEqual(Position::npos))
    prev_pos=this->pos;
    this->pos=this->getNextPosition();
}

string Criminal::str() const{return"Criminal[index="+to_string(this->index)+";pos="+this->pos.str()+"]";}
//3.8
ArrayMovingObject::ArrayMovingObject(int capacity):capacity(capacity),count(0){arr_mv_objs=new MovingObject *[capacity];}

ArrayMovingObject::~ArrayMovingObject(){delete[] arr_mv_objs;}

bool ArrayMovingObject::isFull() const{return count==capacity;}

bool ArrayMovingObject::add(MovingObject * mv_obj){
  if (!this->isFull()){
    this->arr_mv_objs[this->count++]=mv_obj;
    return true;
  }
  else
    return false;
}

string ArrayMovingObject::str() const{
  string arr_mv_objs_str="ArrayMovingObject[count="+to_string(count)+";capacity="+to_string(capacity);
  for(int i=0;i<count;i++)
    arr_mv_objs_str+=";"+arr_mv_objs[i]->str();
  arr_mv_objs_str+="]";
  return arr_mv_objs_str;
}
//3.9
Configuration::Configuration(const string &filepath){
  ifstream file(filepath);
  num_walls=0,num_fake_walls=0;
  string line;
  while(getline(file,line)){
    string key=line.substr(0,line.find('='));
    string value=line.substr(line.find('=')+1);
    if(key==""||value=="")
      continue;
    else if (key=="MAP_NUM_ROWS")
			map_num_rows = stoi(value);
		else if (key=="MAP_NUM_COLS")
			map_num_cols = stoi(value);
		else if (key=="ARRAY_WALLS")
			arr_walls_str = value;
		else if (key=="NUM_FAKE_WALLS")
			num_fake_walls = stoi(value);
		else if (key=="ARRAY_FAKE_WALLS")
			arr_fake_walls_str = value;
		else if (key=="MAX_NUM_MOVING_OBJECTS")
			max_num_moving_objects = stoi(value);
		else if (key=="SHERLOCK_MOVING_RULE")
			sherlock_moving_rule = value;
		else if (key=="SHERLOCK_INIT_POS")
			sherlock_init_pos = Position(value);
		else if (key=="SHERLOCK_INIT_HP")
			sherlock_init_hp = stoi(value);
		else if (key=="SHERLOCK_INIT_EXP")
			sherlock_init_exp = stoi(value);
		else if (key=="WATSON_MOVING_RULE")
			watson_moving_rule = value;
		else if (key=="WATSON_INIT_POS")
			watson_init_pos = Position(value);
		else if (key=="WATSON_INIT_HP")
			watson_init_hp = stoi(value);
		else if (key=="WATSON_INIT_EXP")
			watson_init_exp = stoi(value);
		else if (key=="CRIMINAL_INIT_POS")
			criminal_init_pos = Position(value);
		else if (key=="NUM_STEPS")
			num_steps = stoi(value);
    if(arr_walls_str!=""){
      string wall_str;
      Position *tmp=new Position[map_num_cols*map_num_rows];
      stringstream arr_walls_ss;
      arr_walls_ss<<arr_walls_str.substr(1,arr_walls_str.size()-2);
      for(num_walls=0;getline(arr_walls_ss,wall_str,';');++num_walls)
        tmp[num_walls]=Position(wall_str);
      arr_walls=new Position[num_walls];
      for(int i=0;i<num_walls;i++)
        arr_walls[i]=tmp[i];
      delete[] tmp;
    }
    if(arr_fake_walls_str!=""){
      string fake_wall_str;
      Position *tmp=new Position[map_num_cols*map_num_rows];
      stringstream arr_fake_walls_ss;
      arr_fake_walls_ss<<arr_fake_walls_str.substr(1,arr_fake_walls_str.size()-2);
      for(num_fake_walls=0;getline(arr_fake_walls_ss,fake_wall_str,';');++num_fake_walls)
        tmp[num_fake_walls]=Position(fake_wall_str);
      arr_fake_walls=new Position[num_fake_walls];
      for(int i=0;i<num_fake_walls;i++)
        arr_fake_walls[i]=tmp[i];
      delete[] tmp;
    }
  }
}
Configuration::~Configuration(){
  delete[] this->arr_walls;
  delete[] this->arr_fake_walls;
}
string Configuration::str() const{
  string config_str;
  config_str += "Configuration[\n";

	config_str += "MAP_NUM_ROWS=" + to_string(map_num_rows) +"\n";
	config_str += "MAP_NUM_COLS=" + to_string(map_num_cols) +"\n";

	config_str += "MAX_NUM_MOVING_OBJECTS=" + to_string(max_num_moving_objects) +"\n";

	config_str += "NUM_WALLS=" + to_string(num_walls) +"\n";

	config_str += "ARRAY_WALLS=[";

	for (int i = 0; i < num_walls; i++){
		config_str += arr_walls[i].str();
    if(i!=num_walls-1)
    config_str+=";";
  }
	config_str += "]\n";

	config_str += "NUM_FAKE_WALLS=" + to_string(num_fake_walls) +"\n";

	config_str += "ARRAY_FAKE_WALLS=[";

	for (int i = 0; i < num_fake_walls; i++){
		config_str += arr_fake_walls[i].str();
    if(i!=num_fake_walls-1)
    config_str+=";";
  }
	config_str += "]\n";

	config_str += "SHERLOCK_MOVING_RULE=" + sherlock_moving_rule  +"\n";
	config_str += "SHERLOCK_INIT_POS=" + sherlock_init_pos.str()  +"\n";
	config_str += "SHERLOCK_INIT_HP=" + to_string(sherlock_init_hp)  +"\n";
	config_str += "SHERLOCK_INIT_EXP=" + to_string(sherlock_init_exp)  +"\n";

	config_str += "WATSON_MOVING_RULE=" + watson_moving_rule  +"\n";
	config_str += "WATSON_INIT_POS=" + watson_init_pos.str()  +"\n";
	config_str += "WATSON_INIT_HP=" + to_string(watson_init_hp)  +"\n";
	config_str += "WATSON_INIT_EXP=" + to_string(watson_init_exp)  +"\n";

	config_str += "CRIMINAL_INIT_POS=" + criminal_init_pos.str()  +"\n";

	config_str += "NUM_STEPS=" + to_string(num_steps)  +"\n";

	config_str += "]";
  return config_str;
}
//3.10
RobotC::RobotC(int index, const Position &init_pos, Map *map, Criminal *criminal):Robot(index,init_pos,map,RobotType::C,"RobotC"),criminal(criminal){}
RobotS::RobotS(int index, const Position & init_pos, Map * map,Criminal *criminal, Sherlock *sherlock):Robot(index,init_pos,map,RobotType::C,"RobotC"),criminal(criminal){}
RobotW::RobotW(int index, const Position & init_pos, Map * map,Criminal *criminal, Watson *watson):Robot(index,init_pos,map,RobotType::C,"RobotC"),criminal(criminal){}
RobotSW::RobotSW(int index, const Position & init_pos, Map * map, Criminal *criminal,Sherlock *sherlock, Watson *watson):Robot(index,init_pos,map,RobotType::C,"RobotC"),criminal(criminal){}


Position Criminal::getPrevPosition(){return prev_pos;}
Position RobotC::getNextPosition(){return criminal->getPrevPosition();}
Position RobotS::getNextPosition(){
  Position next_pos=pos;
  Position sherlock_pos=sherlock->getCurrentPosition();
  if(pos.getRow()>sherlock_pos.getRow()){
    next_pos.setRow(pos.getRow()-1);
    if(map->isValid(next_pos,this)) 
    return next_pos;
  }
  else{
    if(pos.getCol()<sherlock_pos.getCol())
    next_pos.setCol(pos.getCol()+1);
    else
    next_pos.setRow(pos.getRow()+1);
    if(map->isValid(next_pos,this)) 
    return next_pos;
  }
  return Position::npos;
}
Position RobotW::getNextPosition(){
  Position next_pos=pos;
  Position watson_pos=watson->getCurrentPosition();
  if(pos.getRow()>watson_pos.getRow()){
    next_pos.setRow(pos.getRow()-1);
  }
  else if(pos.getCol()<watson_pos.getCol())
    next_pos.setCol(pos.getCol()+1);
  else if(pos.getRow()<watson_pos.getRow())
    next_pos.setRow(pos.getRow()+1);
  else
    next_pos.setCol(pos.getCol()+1);
  if(map->isValid(next_pos,this)) 
    return next_pos;
  return Position::npos;
}
Position RobotSW::getNextPosition(){
  Position next_pos=pos;
  Position sherlock_pos=sherlock->getCurrentPosition();
  Position watson_pos=watson->getCurrentPosition();
  for(int i=0;i<2;i++){
    if(pos.getRow()>sherlock_pos.getRow()&&pos.getRow()>watson_pos.getRow()){
      next_pos.setRow(pos.getRow()-1);
    }
    else if(pos.getCol()<sherlock_pos.getCol()&&pos.getCol()<watson_pos.getCol()){
      next_pos.setCol(pos.getCol()-1);
    }
    else if(pos.getRow()<sherlock_pos.getRow()&&pos.getRow()<watson_pos.getRow()){
      next_pos.setRow(pos.getRow()+11);
    }
    else if(pos.getCol()>sherlock_pos.getCol()&&pos.getCol()>watson_pos.getCol()){
      next_pos.setCol(pos.getCol()-1);
    }
    else{
      next_pos.setRow(pos.getRow()-1);
    }
  }
  if(map->isValid(next_pos,this))
    return next_pos;
    else
    return Position::npos;
}

void RobotC::move(){
  Position next_pos=getNextPosition();
  if(!next_pos.isEqual(Position::npos))
  this->pos=next_pos;
}
void RobotS::move(){
  Position next_pos=getNextPosition();
  if(!next_pos.isEqual(Position::npos))
  this->pos=next_pos;
}
void RobotW::move(){
  Position next_pos=getNextPosition();
  if(!next_pos.isEqual(Position::npos))
  this->pos=next_pos;
}
void RobotSW::move(){
  Position next_pos=getNextPosition();
  if(!next_pos.isEqual(Position::npos))
  this->pos=next_pos;
}

int RobotC::getDistance(Character*obj) const{
  Position obj_pos=obj->getCurrentPosition();
  return abs(pos.getRow()-obj_pos.getRow())+abs(pos.getCol()-obj_pos.getCol());
}
int RobotS::getDistance() const{
  Position sherlock_pos=sherlock->getCurrentPosition();
  return abs(pos.getRow()-sherlock_pos.getRow())+abs(pos.getCol()-sherlock_pos.getCol());
}
int RobotW::getDistance() const{
  Position watson_pos=watson->getCurrentPosition();
  return abs(pos.getRow()-watson_pos.getRow())+abs(pos.getCol()-watson_pos.getCol());
}
int RobotSW::getDistance() const{
  Position sherlock_pos=sherlock->getCurrentPosition();
  Position watson_pos=watson->getCurrentPosition();
  return abs(pos.getRow()-sherlock_pos.getRow())+abs(pos.getCol()-sherlock_pos.getCol())+abs(pos.getRow()-watson_pos.getRow())+abs(pos.getCol()-watson_pos.getCol());
}

string RobotC::str() const{return"Robot[pos="+pos.str()+";type=C;dist=]";}
string RobotS::str() const{return"Robot[pos="+pos.str()+";type=S;dist="+to_string(this->getDistance())+"]";}
string RobotW::str() const{return"Robot[pos="+pos.str()+";type=W;dist="+to_string(this->getDistance())+"]";}
string RobotSW::str() const{return"Robot[pos="+pos.str()+";type=SW;dist="+to_string(this->getDistance())+"]";}

//3.11
Character::Character(int index, const Position &init_pos, Map *map,const string &name) : MovingObject(index, init_pos, map, name){}

Robot::Robot(int index, const Position &init_pos, Map *map, RobotType robot_type, const string &name): MovingObject(index, init_pos, map, name), robot_type(robot_type){
  int n=init_pos.getCol()*init_pos.getRow();
  while(n>10)
  n=n/10+n%10;
  switch(n){
    case 0 ... 1:
    this->item= new MagicBook();
    break;
    case 2 ... 3:
    this->item= new EnergyDrink();
    break;
    case 4 ... 5:
    this->item= new FirstAid();
    break;
    case 6 ... 7:
    this->item= new ExcemptionCard();
    break;
    case 8 ... 9:
    const int t=(this->pos.getCol()*11+this->pos.getRow())%4;
    switch (t)
    {
    case 0:
    this->item=new PassingCard("RobotS");
      break;
    case 1:
    this->item=new PassingCard("RobotC");
    break;
    case 2:
    this->item=new PassingCard("RobotSW");
    break;
    case 3:
    this->item=new PassingCard("all");
  
    default:
    break;
    }
  }
}
Robot::~Robot(){delete item;}

BaseItem::BaseItem(ItemType item_type):item_type(item_type){}

int Sherlock::getExp()const{return exp;}
int Watson::getExp()const{return exp;}
int Sherlock::getHp()const{return hp;}
int Watson::getHp()const{return hp;}
void Sherlock::setExp(int exp){this->exp=exp;}
void Sherlock::setHp(int hp){this->hp=hp;}
void Watson::setExp(int exp){this->exp=exp;}
void Watson::setHp(int hp){this->hp=hp;}

MagicBook::MagicBook():BaseItem(ItemType::MAGIC_BOOK){}
EnergyDrink::EnergyDrink():BaseItem(ItemType::ENERGY_DRINK){}
FirstAid::FirstAid():BaseItem(ItemType::FIRST_AID){}
ExcemptionCard::ExcemptionCard():BaseItem(ItemType::EXCEMPTION_CARD){}
PassingCard::PassingCard(const string &challenge):BaseItem(ItemType::PASSING_CARD),challenge(challenge){}

bool MagicBook::canUse(Character *obj, Robot *robot){
  if(obj->getName()=="Sherlock")
    return static_cast<Sherlock *>(obj)->getExp()<=350?true:false;
  else if(obj->getName()=="Watson")
    return static_cast<Watson *>(obj)->getExp()<=350?true:false;
  return false;
}
bool EnergyDrink::canUse(Character *obj, Robot *robot){
  if(obj->getName()=="Sherlock")
    return static_cast<Sherlock *>(obj)->getHp()<=100?true:false;
  else if(obj->getName()=="Watson")
    return static_cast<Watson *>(obj)->getHp()<=100?true:false;
  return false;
}
bool FirstAid::canUse(Character *obj, Robot *robot){
  if(obj->getName()=="Sherlock")
    return static_cast<Sherlock *>(obj)->getHp()<=100||static_cast<Sherlock *>(obj)->getExp()<=350?true:false;
  else if(obj->getName()=="Watson")
    return static_cast<Watson *>(obj)->getHp()<=100|| static_cast<Watson *>(obj)->getExp()<=350?true:false;
  return false;
}
bool ExcemptionCard::canUse(Character *obj, Robot *robot){
  if(obj->getName()=="Sherlock"&&static_cast<Sherlock*>(obj)->getHp()%2!=0)
    return true;
  return false;
}
bool PassingCard::canUse(Character *obj, Robot *robot){
  if(obj->getName()=="Watson"&&static_cast<Watson*>(obj)->getHp()%2==0)
    return true;
  return false;
}

void MagicBook::use(Character *obj, Robot *robot){
  if(!canUse(obj,robot))
  return;
  if(obj->getName()=="Sherlock"){
    Sherlock *sherlock=static_cast<Sherlock*>(obj);
    sherlock->setExp(static_cast<Sherlock*>(obj)->getExp()*1.25);
  }
  else if(obj->getName()=="Watson"){
    Watson *watson=static_cast<Watson *>(obj);
    watson->setExp(static_cast<Watson*>(obj)->getExp()*1.25);
  }
}
void EnergyDrink::use(Character *obj, Robot *robot){
  if(!canUse(obj,robot))
  return;
  if(obj->getName()=="Sherlock"){
    Sherlock *sherlock=static_cast<Sherlock*>(obj);
    sherlock->setHp(static_cast<Sherlock*>(obj)->getHp()*1.2);
  }
  else if(obj->getName()=="Watson"){
    Watson *watson=static_cast<Watson*>(obj);
    watson->setHp(static_cast<Watson*>(obj)->getHp()*1.2);
  }
}
void FirstAid::use(Character *obj, Robot *robot){
  if(!canUse(obj,robot))
  return;
  if(obj->getName()=="Sherlock"){
    Sherlock*sherlock=static_cast<Sherlock*>(obj);
    sherlock->setHp(static_cast<Sherlock*>(obj)->getHp()*1.5);
  }
  else if(obj->getName()=="Watson"){
    Watson *watson=static_cast<Watson*>(obj);
    watson->setHp(static_cast<Watson*>(obj)->getHp()*1.5);
  }
}
void ExcemptionCard::use(Character *obj, Robot *robot){//Undone
  if(!canUse(obj,robot))
  return;
}

void PassingCard::use(Character *obj, Robot *robot){
  if(!canUse(obj,robot))
    return;
  else if(challenge!="all"&&challenge!=robot->getName()){
    Watson *watson=static_cast<Watson*>(obj);
    watson->setHp(static_cast<Watson*>(obj)->getHp()-50);
  }
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////