#include <string>
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <stack>
#include <vector>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <map>
#include <initializer_list>
#include <functional>
#include <list>
//#include "classes.h"

using namespace std;
//глобалка хранящяя последний нанесенный урон для сообщения вывода информации об атаке
int lastDamage;

class Warrior;
class Archer;
class Physician;
class Rogue;
class Defender;
class Zombie;
class Bandit;
class Enemy;
class Character;
class Level;

//функция для рандома урона
int getRandomDamage(int min,int max){
    srand(time(NULL)+rand());
    int num = min + rand() % (max - min + 1);
    return num;
}
int getRandomToHeroStack(int n){
    // Установить генератор случайных чисел
  srand(time(NULL)+rand());
  n = n-1;
  // Получить случайное число - формула
  if (n == 0){
    return 0;
  }
  int num = rand() % n ;
  return num;
}
//Получить случ число до 100
int getRandom()
{
  // Установить генератор случайных чисел
  srand(time(NULL)+rand());
  // Получить случайное число - формула
  int num = rand() % 100 ;
  return num;
}
// Получить случ число надбавки скорости с кубика
int getRandomSpeed()
{
  // Установить генератор случайных чисел
  srand(time(NULL)+rand());
  // Получить случайное число - формула
  int num = rand() % 6 + 1 ;
  return num;
}
// Объявление классов
//map<string,void(*)()> Abils = {{"attack1",getRandom()},{"attack2",getRandom()}};
/*
map<string,function<void(*)()>> Abils = {};
void Mapper(){
    map<string,function<void(*)()>> Abils = {};
    Abils["attack1"] = {getRandom()};
    Abils["attack2"] = {getRandom()};
}
*/

class Character
{
public:
    string name;
    int isEnemy = 0;
    double health;
    double maxHealth;
    double damage;
    int armor = 0;
    int dodge = 15;
    int speed;
    int critChance = 20;
    int tmpSpeed;
    int numOfAbils = 1;
    int isHealer = 0;
    vector<string> attackName = {};
    Character(string n, double hp, double maxhp, double dmg,int spd)
    {
        name = n;
        health = hp;
        maxHealth = maxhp;
        damage = dmg;
        speed = spd;
    }
    ~Character(){}
    Character() {}
    void message()
    {
        cout << name << " " <<this->health << endl;
    }
    bool getDamage(int baseDamage){

        if (getRandom()<dodge){
            cout<<this->name<<" dodged attack"<<endl;
            return false;
        }else{
            this->health -= (baseDamage-(baseDamage/100*this->armor));
            lastDamage = (baseDamage-(baseDamage/100*this->armor));
            return true;
        }
    }
    bool getHeal(int baseHeal){
        this->health += baseHeal;
        if (this->health>this->maxHealth){
            this->health = this->maxHealth;
        }
        lastDamage = baseHeal;
        return true;
    }
    virtual void attack1(Character *target)=0;
    virtual void attack2(Character *target)=0;
    virtual void attack3(Character *target)=0;
    virtual void attack4(Character *target)=0;

};
Character* getWarrior();
Character* getArcher();
Character* getDefender();
Character* getRogue();
Character* getZombie();
Character* getBandit();
Character* getBigZombie();
Character* getPhysician();
// Проверка погибла ли цель атаки
bool checkDead(Character *target)
{
    if (target->health <= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int calcDamage(Character* character){
    int resultDamage = 0, critUp = 0, minDamage, maxDamage;
    minDamage = character->damage*0.75;
    maxDamage = character->damage*1.25;
    resultDamage = getRandomDamage(minDamage,maxDamage);
    if (character->critChance>getRandom()){
        resultDamage *= 1.5;
        cout<<"It's a CRIT!"<<endl;
    }
    return resultDamage;
}
vector<Character *> createHeroStack()
{
    vector<Character *> arr;
    /*Character *hero = new Character("Hero", 100, 100, 12);
    Archer *arch = new Archer("Arch", 80, 80, 9);
    arr.push_back(hero);
    arr.push_back(arch);*/
    return arr;
}
// здесь были объявления для хедера
vector<Character *> createEnemyStack()
{
    vector<Character *> arr;
    //arr.push_back(enemy);
    //arr.push_back(enemy);
    /*
    Enemy *en = new Enemy("Enemy", 100, 100, 12);
    Zombie *zb = new Zombie("Zombie", 80, 80, 9);
    arr.push_back(en);
    arr.push_back(zb);*/
    return arr;
}
//расчет урона

// Класс уровня
class Level
{
public:
    vector<Character*> heroStack = createHeroStack();
    vector<Character*> enemyStack = createEnemyStack();
    vector<Character*> turnStack;
    vector<Character*>::iterator enemyStackIt;
    vector<Character*>::iterator heroStackIt;
    int tmpEnemyPos,tmpAbilPos;
    Level(){};
    void addEnemy(Character* e){
        enemyStack.push_back(e);
    }
    void addHero(Character* h){
        heroStack.push_back(h);
    }
    static bool comp(Character* l, Character* r){
        return ((l->tmpSpeed)>(r->tmpSpeed));
    }
    //Функция для накидывания случайной скорости персам в стэке и их сортировки 
    vector<Character*> randomizeSpeedOnStack(vector<Character*> mainStack){
        vector<Character*> tmp = mainStack;
        for(auto& character: tmp){
            //cout<<hero->name<<" ;0 "<<hero->speed<<endl;
            character->tmpSpeed = character->speed+getRandomSpeed();
        }
        sort(tmp.begin(),tmp.end(),comp);
        return tmp;
    }
    //Функция для определения враг или нет
    string enemyOrHero(Character* character){
        switch (character->isEnemy){
            case 0:
                return "(Hero)";
                break;
            case 1:
                return "(Enemy)";
                break;
        }
    }
    //Функция для определения стека с персами в порядке их ходов
    void speedCheck(){
        turnStack.clear();
        vector<Character*> tmpStack;
        tmpStack = randomizeSpeedOnStack(heroStack);
        for(auto& character: tmpStack){
            turnStack.push_back(character);
        }
        tmpStack.clear();
        tmpStack = randomizeSpeedOnStack(enemyStack);
        for(auto& character: tmpStack){
            turnStack.push_back(character);
        }
        tmpStack.clear();
        sort(turnStack.begin(),turnStack.end(),comp);
        /*cout<<"Next turn: "<<endl;
        for(auto& character: turnStack){
            cout.width(3);cout<<"|"<<internal<<character->name<<enemyOrHero(character)<<"|";
        }
        cout<<endl;
        for(auto& character: turnStack){
            cout.width(4);cout<<internal<<"("<<character->health<<"/"<<character->maxHealth<<") hp ";
        }
        cout<<endl;*/
        cout<<"Next turn: "<<endl;
        int i = 1;
        for(auto& character: turnStack){
            cout.width(3);cout<<i<<" - |"<<internal<<character->name<<enemyOrHero(character)<<"|"<<
                "("<<character->health<<"/"<<character->maxHealth<<") hp "<<endl;
            i++;
            Sleep(300);
        }
        cout<<endl;
    } 
    void test1(Character* character, Character* t){
        character->attack1(t);
    }
    int chooseEnemy(int isPlayer){
        int chosenEnemy = 0;
        switch(isPlayer){
            case 0:
                chosenEnemy = getRandomToHeroStack(heroStack.size());
                tmpEnemyPos = chosenEnemy;
                break;
            case 1:
                cout<<"Choose target: "<<endl;
                for (int i = 0;i<enemyStack.size();i++){
                    cout<<i+1<<") "<<enemyStack[i]->name<<"("<<enemyStack[i]->health<<"/"<<enemyStack[i]->maxHealth<<")"<<endl;
                }
                cin>>chosenEnemy;
                if (chosenEnemy>enemyStack.size()){
                    cout<<"Incorrect position"<<endl;
                    chooseEnemy(1);
                }else{
                    chosenEnemy = chosenEnemy-1;
                    tmpEnemyPos = chosenEnemy;
                }
        }
        return tmpEnemyPos;
    }
    int chooseFriend(int isPlayer){
        int chosenFriend = 0;
        switch(isPlayer){
            case 0:
                chosenFriend = getRandomToHeroStack(enemyStack.size());
                tmpEnemyPos = chosenFriend;
                break;
            case 1:
                cout<<"Choose target: "<<endl;
                for (int i = 0;i<heroStack.size();i++){
                    cout<<i+1<<") "<<heroStack[i]->name<<"("<<heroStack[i]->health<<"/"<<heroStack[i]->maxHealth<<")"<<endl;
                }
                cin>>chosenFriend;
                if (chosenFriend>heroStack.size()){
                    cout<<"Incorrect friend's position"<<endl;
                    chooseFriend(1);
                }else{
                    chosenFriend = chosenFriend-1;
                    tmpEnemyPos = chosenFriend;
                }
        }
        return tmpEnemyPos;
    }
    int chooseAbil(int isPlayer, Character* character){
        int chosenAbil = 0;
        switch(isPlayer){
            case 0:
                chosenAbil = getRandomToHeroStack(character->numOfAbils);
                tmpAbilPos = chosenAbil;
                break;
            case 1:
                cout<<"Choose your ability: "<<endl;
                for (int i = 0; i<character->numOfAbils;i++){
                    cout<<i+1<<") "<<character->attackName[i]<<endl;
                }
                cin>>chosenAbil;
                if(chosenAbil>character->numOfAbils){
                    cout<<"Incorrect ability's position"<<endl;
                    chooseAbil(1,character);
                }else{
                    chosenAbil = chosenAbil-1;
                    tmpAbilPos = chosenAbil;
                }
        }
        return tmpAbilPos;
    }
    //Обработка хода игрока
    void playerTurn(Character* character){
        //void (*abil)();
        Sleep(800);
        cout<<"Your "<<character->name<<"\'s ("<<character->health<<"/"<<character->maxHealth<<") hp move!"<<endl;
        Sleep(500);
        int chosenAbil = 0, chosenEnemy = 0;
        chosenAbil = chooseAbil(1,character);
        if (character->isHealer==1 && chosenAbil == 0){// определение того хил или атака, условно все хилы 0ые в списке абилок
            chosenEnemy = chooseFriend(1);
            character->attack1(heroStack[chosenEnemy]);
        }else{
            chosenEnemy = chooseEnemy(1);
            switch(chosenAbil){
                case 0:
                    character->attack1(enemyStack[chosenEnemy]);
                    break;
                case 1:
                    character->attack2(enemyStack[chosenEnemy]);
                    break;
                case 2:
                    character->attack3(enemyStack[chosenEnemy]);
                    break;
                case 3:
                    character->attack4(enemyStack[chosenEnemy]);
                    break;
            }
        }
    }
    //Обработка хода противника
    void enemyTurn(Character* character){
        Sleep(600);
        cout<<"Enemy "<<character->name<<"\'s ("<<character->health<<"/"<<character->maxHealth<<") hp move!"<<endl;
        Sleep(1200);
        int chosenAbil = 0, chosenEnemy = 0;
        chosenAbil = chooseAbil(0,character);
        if (character->isHealer==1 && chosenAbil == 0){// определение того хил или атака, условно все хилы 0ые в списке абилок
            chosenEnemy = chooseFriend(0);
            character->attack1(enemyStack[chosenEnemy]);
        }else{
            chosenEnemy = chooseEnemy(0);
            switch(chosenAbil){
                case 0:
                    character->attack1(heroStack[chosenEnemy]);
                    break;
                case 1:
                    character->attack2(heroStack[chosenEnemy]);
                    break;
                case 2:
                    character->attack3(heroStack[chosenEnemy]);
                    break;
                case 3:
                    character->attack4(heroStack[chosenEnemy]);
                    break;
            }
        }
    }
    //Обработка самого хода
    void turn(){
        
        vector<Character*>::iterator character;
        character = turnStack.begin();
       // distance(turnStack.begin(), find(turnStack.begin(), turnStack.end(), character)
        while (character != turnStack.end() && !heroStack.empty() && !enemyStack.empty() && character <= turnStack.end()){
            Sleep(1000);
            if (heroStack.empty()){
                cout<<"THE END"<<endl;
                exit(0);
            }
            else if ((*character)->isEnemy == 0){
                playerTurn((*character));
                ++character;
            }
            else if((*character)->isEnemy == 1){
                enemyTurn((*character));
                ++character;
            }
        }
    }
    void getRandomPartyHero(){
        //cout<<"AAA"<<endl;
        int tmp = 0;
        Character* hero1;
        Character* hero2;
        Character* hero3;
        Character* hero4;
        vector<Character*> stackTmp = {hero1,hero2,hero3,hero4 };
        for (int i = 0; i<4;i++){
            tmp = getRandomDamage(0,4);//допилить!!!
            switch(tmp){
                case 0:
                    stackTmp[i] = getWarrior();
                    break;
                case 1:
                    stackTmp[i] = getArcher();
                    break;
                case 2:
                    stackTmp[i] = getDefender();
                    break;
                case 3:
                    stackTmp[i] = getRogue();
                    break;
                case 4:
                    stackTmp[i] = getPhysician();
                    break;

            }
        }
        heroStack = stackTmp;
    }
    void getRandomPartyEnemy(){

        int tmp = 0;
        Character* enemy1;
        Character* enemy2;
        Character* enemy3;
        Character* enemy4;
        vector<Character*> stackTmp = {enemy1,enemy2,enemy3,enemy4 };

        for (int i = 0; i<4;i++){
            tmp = getRandomDamage(0,4);//допилить
            switch(tmp){
                case 0:
                    stackTmp[i] = getBandit();
                    break;
                case 1:
                    stackTmp[i] = getZombie();
                    break;
                case 2:
                    stackTmp[i] = getBigZombie();
                    break;
                case 3:
                    stackTmp[i] = getRogue();
                    stackTmp[i]->isEnemy = 1;
                    stackTmp[i]->name = "Highwayman";
                    break;
                case 4:
                    stackTmp[i] = getPhysician();
                    stackTmp[i]->isEnemy = 1;
                    stackTmp[i]->name = "Evil Surgeon";
                    break;
            }
        }
        enemyStack = stackTmp;
    }
};
Level *lvl = new Level();
void gameStart(){
        cout<<"Game is starting";Sleep(400);cout<<".";Sleep(400);cout<<".";Sleep(400);cout<<"."<<endl;
        lvl->getRandomPartyHero();
        lvl->getRandomPartyEnemy();
        while(!lvl->heroStack.empty()){
            if (!lvl->enemyStack.empty()){
                lvl->randomizeSpeedOnStack(lvl->enemyStack);
                lvl->randomizeSpeedOnStack(lvl->heroStack);
                lvl->speedCheck();
                lvl->turn();
            }else{
                lvl->getRandomPartyEnemy();
                lvl->randomizeSpeedOnStack(lvl->enemyStack);
                lvl->randomizeSpeedOnStack(lvl->heroStack);
                lvl->speedCheck();
                lvl->turn();
            }
        }
    }
//Очистка трупа из стэка врагов
bool CorpseClean(Character *target){
    //cout<<target->name<<target->isEnemy<<" flag"<<endl;
    if (checkDead(target) && (target->isEnemy==1)){
        delete lvl->turnStack[distance(lvl->turnStack.begin(), find(lvl->turnStack.begin(), lvl->turnStack.end(), target))];
        lvl->turnStack.erase(find(lvl->turnStack.begin(), lvl->turnStack.end(), target));
        lvl->enemyStack.erase(find(lvl->enemyStack.begin(), lvl->enemyStack.end(), target));
        //delete target;
        //cout<<" KILL "<<lvl->enemyStack.size()<<endl;
        return true;
    }else if(checkDead(target) && (target->isEnemy==0)){
        delete lvl->turnStack[distance(lvl->turnStack.begin(), find(lvl->turnStack.begin(), lvl->turnStack.end(), target))];
        lvl->turnStack.erase(find(lvl->turnStack.begin(), lvl->turnStack.end(), target));
        lvl->heroStack.erase(find(lvl->heroStack.begin(), lvl->heroStack.end(), target));
        //delete target;
        //cout<<" KOLL "<<target->isEnemy<<endl;
        return true;
    }
    return false;
}
//Базовый класс врагов, отличие только в флаге isEnemy
class Enemy : public Character
{
public:
    void message()
    {
        cout << " Enemy " <<health<< endl;
    }
    Enemy(string n, double hp, double maxhp, double dmg, int spd) : Character(n, hp, maxhp, dmg, spd){
        isEnemy = 1;
    };
    
};
class Zombie : public Enemy
{
public:
    Zombie(string n, double hp, double maxhp, double dmg, int spd) : Enemy(n, hp, maxhp, dmg, spd){
        attackName = {"Grab"};
    }; 
    void attack1(Character *target) override
    {
        string killMessage = target->name+" was grabbed by "+this->name;
        if (target->getDamage(calcDamage(this))){
            cout <<this->name <<" is using " << attackName[0] << " at " << target->name <<" with "<<lastDamage<<" dmg!"<< endl;
        };
        if (CorpseClean(target)){
            cout<<killMessage<<endl;
        }
    }
    void attack2(Character *target) override{}
    void attack3(Character *target) override{}
    void attack4(Character *target) override{}
    
};
class Bandit: public Enemy
{
    public:
    Bandit(string n, double hp, double maxhp, double dmg, int spd) : Enemy(n, hp, maxhp, dmg, spd){
        attackName = {"Chop"};
    };
    void attack1(Character *target) override
    {
        string killMessage = target->name+" was chopped by "+this->name;
        if (target->getDamage(calcDamage(this))){
            cout<< this->name <<" is using " << attackName[0] << " at " << target->name <<" with "<<lastDamage<<" dmg!"<< endl;
        };
        if (CorpseClean(target)){
            cout<<killMessage<<endl;
        }
    }
    void attack2(Character *target) override{}
    void attack3(Character *target) override{}
    void attack4(Character *target) override{}
};
class Warrior : public Character
{
public:
    Warrior(string n, double hp, double maxhp, double dmg, int spd) : Character(n, hp, maxhp, dmg, spd){
        numOfAbils = 2;
        attackName = {"Cut","Massacre"};
    };
    void attack1(Character *target) override
    {
        string killMessage = target->name+" was cut down by "+this->name;
        if (target->getDamage(calcDamage(this))){
            cout <<this->name <<" is using " << attackName[0] << " at " << target->name <<" with "<<lastDamage<<" dmg!"<< endl;
        };
        if (CorpseClean(target)){
            cout<<killMessage<<endl;
        }
    }
    void attack2(Character *target) override{
        vector<Character*>::iterator enemy2;
        bool doubleAttack = 0;
        string killMessage = target->name+" was knock down by "+this->name;
        //Character* enemy1;
        enemy2 = find(lvl->enemyStack.begin(),lvl->enemyStack.end(),target);
        if (enemy2+1!= lvl->enemyStack.end()){
            ++enemy2;
            doubleAttack = 1;
        }
        if (target->getDamage(calcDamage(this))){
            cout <<this->name <<" is using " << attackName[1] << " at " << target->name <<" with "<<lastDamage<<" dmg!"<< endl;
        };
        if (CorpseClean(target)){
            cout<<killMessage<<endl;
        }
        if ((*enemy2)->getDamage(calcDamage(this)) && doubleAttack){
            cout <<this->name <<" is using " << attackName[1] << " at " << (*enemy2)->name <<" with "<<lastDamage<<" dmg!"<< endl;
            killMessage = (*enemy2)->name+" was knock down by "+this->name;
        }
        if (CorpseClean((*enemy2))){
            cout<<killMessage<<endl;
        }
    }
    void attack3(Character *target) override{}
    void attack4(Character *target) override{}
};
class Archer : public Character
{
public:
    void attack1(Character *target) override
    {
        string killMessage = target->name+" was shot down by "+this->name;
        if (target->getDamage(calcDamage(this))){
            cout <<this->name <<" is using " << attackName[0] << " at " << target->name <<" with "<<lastDamage<<" dmg!"<< endl;
        };
        if (CorpseClean(target)){
            cout<<killMessage<<endl;
        }
    }
    void attack2(Character *target) override{}
    void attack3(Character *target) override{}
    void attack4(Character *target) override{}
    Archer(string n, double hp, double maxhp, double dmg,int spd) : Character(n, hp, maxhp, dmg, spd){

        attackName = {"Shot"};
    };
    
};
class Physician: public Character{
public:
    void attack1(Character *target) override{
        if (target->getHeal(calcDamage(this)+5)){
            cout <<this->name <<" is using " << attackName[0] << " at " << target->name <<" with "<<lastDamage<<" hp!"<< endl;
        };
    }
    void attack2(Character *target) override
    {
        string killMessage = target->name+" was bleed out by "+this->name;
        if (target->getDamage(calcDamage(this))){
            cout <<this->name <<" is using " << attackName[1] << " at " << target->name <<" with "<<lastDamage<<" dmg!"<< endl;
        };
        if (CorpseClean(target)){
            cout<<killMessage<<endl;
        }
    }
    void attack3(Character *target) override{}
    void attack4(Character *target) override{}
    Physician(string n, double hp, double maxhp, double dmg,int spd) : Character(n, hp, maxhp, dmg, spd){
        attackName = {"Bandage", "Scalpel hit"};
        isHealer = 1;
        numOfAbils = 2;
    };
};
class Rogue : public Character
{
public:
    void attack1(Character *target) override
    {
        string killMessage = target->name+" was stabbed by "+this->name;
        if (target->getDamage(calcDamage(this))){
            cout <<this->name <<" is using " << attackName[0]<< " at " << target->name <<" with "<<lastDamage<<" dmg!"<< endl;
        };
        if (CorpseClean(target)){
            cout<<killMessage<<endl;
        }
    }
    void attack2(Character *target) override{}
    void attack3(Character *target) override{}
    void attack4(Character *target) override{}
    Rogue(string n, double hp, double maxhp, double dmg,int spd) : Character(n, hp, maxhp, dmg,spd){
        attackName = {"Dagger stab"};
    };
};
class Defender : public Character
{
public:
    void attack1(Character *target) override
    {
        string killMessage = target->name+" was killed by "+this->name;
        if (target->getDamage(calcDamage(this))){
            cout <<this->name <<" is using " << attackName[0] << " at " << target->name <<" with "<<lastDamage<<" dmg!"<< endl;
        };
        if (CorpseClean(target)){
            cout<<killMessage<<endl;
        }
    }
    void attack2(Character *target) override{}
    void attack3(Character *target) override{}
    void attack4(Character *target) override{}
    Defender(string n, double hp, double maxhp, double dmg,int spd) : Character(n, hp, maxhp, dmg,spd){
        attackName = {"Shield punch"};
    };
};
Character* getPhysician(){
    Character* p = new Physician("Healer",110,110,22,5);
    return p;
}
Character* getWarrior(){
    Character* w = new Warrior("Warrior",120,120,27,3);
    return w;
}
Character* getArcher(){
    Character* a =new Archer("Arch",100,100,24,4);
    return a;
}
Character* getDefender(){
    Character * d = new Defender("Defer", 200,200,21,2);
    return d;
}
Character * getRogue(){
    Character* r = new Rogue("Roger",90,90,42,6);
    return r;
}
Character* getZombie(){
    Character* z = new Zombie("Corpse",100,100,27,4);
    return z;
}
Character* getBandit(){
    Character* b = new Bandit("Convict",140,140,23,7);
    return b;
}
Character* getBigZombie(){
    Character* z = new Zombie("Cadaver",160,160,26,1);
    return z;
}
int main(){
/*
    Warrior *w = new Warrior("Warrior",120,120,27,300);
    Archer *a = new Archer("Arch",100,100,24,3);
    Defender *d = new Defender("Defer", 200,200,21,200);
    Rogue *r = new Rogue("Roger",90,90,42,250);
    Zombie *test = new Zombie("Corpse",100,100,27,220);
    Zombie *test1 = new Zombie("Weak Corpse",90,90,24,180);*/
    /*lvl->getRandomPartyHero();
    lvl->getRandomPartyEnemy();
    lvl->randomizeSpeedOnStack(lvl->enemyStack);
    lvl->randomizeSpeedOnStack(lvl->heroStack);
    lvl->speedCheck();
    lvl->turn();*/
    gameStart();
}
