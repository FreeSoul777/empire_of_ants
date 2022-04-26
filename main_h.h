#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <bits/shared_ptr.h>
#include <algorithm>
#include <iomanip>
#include <windows.h>
using namespace std;


enum Resources{
    twig = 0,
    dewdrop,
    pebble,
    leaf
};

enum Type_OP {
    Type_Worker_OP = 0x100,
    Type_Warrier_OP = 0x200,
    Type_Insect_OP = 0x300,
    Type_Parent_OP = 0x400,
};
enum Type_Worker{
    defW = 0,
    senior_worker = Type_Worker_OP,
    advanced_sleepy,
    an_ordinary_pickpocket,
};
enum Type_Warrier{
    defWa = 0,
    senior_warrier = Type_Warrier_OP,
    elite_hero,
    advanced,
    senior_berserker,
    ordinary
};
enum Type_Insect{
    defI = 0,
    butterfly = Type_Insect_OP,
    mole_cricket
};
enum Type_Parent{
    defP = 0,
    base = Type_Parent_OP,
    warrior,
    worker,
    insects,
    queen
};


struct Type {
    int value;

    Type(Type_Worker val) : value((int)val) { }
    Type(Type_Warrier val) : value((int)val) { }
    Type(Type_Insect val) : value((int)val) { }
    Type(Type_Parent val) : value((int)val) { }
    Type& operator=(Type_Worker val) {
        value = (int)val;
        return *this;
    }
    Type& operator=(Type_Warrier val) {
        value = (int)val;
        return *this;
    }
    Type& operator=(Type_Insect val) {
        value = (int)val;
        return *this;
    }
    Type& operator=(Type_Parent val) {
        value = (int)val;
        return *this;
    }

    Type_Worker get_worker() const {
        if (value >= senior_worker && value <= an_ordinary_pickpocket)
            return (Type_Worker)value;
        return defW;
    }
    Type_Warrier get_warrier() const {
        if (value >= senior_warrier && value <= ordinary)
            return (Type_Warrier)value;
        return defWa;
    }
    Type_Insect get_insect() const {
        if (value >= butterfly && value <= mole_cricket)
            return (Type_Insect)value;
        return defI;
    }
    Type_Parent get_parent() const {
        if (value >= base && value <= queen)
            return (Type_Parent)value;
        return defP;
    }
};




class Empire;
class Q;
class Worker;
class SpecialInsect;
class Warrior;

class Base {
protected:
    int h = 0;
    int p = 0;
    int d = 0;
    bool isAlive = true;
    Type type = base;

public:
    Base(int h = 0, int p = 0, int d = 0) {
        this->h = h;
        this->p = p;
        this->d = d;
        this->isAlive = true;
//        cout << "\nBase";
    }
    virtual ~Base() {
//        cout << "\n~Base" << endl;
    }
    bool in_operation = true;
    int getH() const { return h;}
    void setH(int newH) { h = newH;}
    int getP() const { return p;}
    void setP(int newP) { p = newP;}
    int getD() const { return d;}
    void setD(int newD) { d = newD;}
    bool getIsAlive() const { return isAlive;}
    void setIsAlive(bool newIsAlive) { isAlive = newIsAlive;}
    void setT(Type newT) { type = newT;}
    Type getT() const { return type;}

    bool attacked(int damage);

    void show() {
        cout << " h: " << h << " p: " << p << " d: " << d /*<< " live: " << isAlive << " TYPE: " << type.value*/;
    }


    template<typename T>
    bool isA() {
        return (dynamic_cast<T*>(this) != NULL);
    }
};

//КЛАСС КОРОЛЕВА---------------------------------------------------------------------
class Q: public Base{
private:
    string name = "";
    string mather = "";
    int period = -1;
    int countChildren = 0;
    int countDaughter = 0;
    int fp = 0, tp = 0, fc = 0, tc = 0, fd = 0, td = 0;
    weak_ptr<Empire> e;

    std::vector<Type_Worker> typeWork = {};
    std::vector<Type_Warrier> typeWar = {};
    std::vector<Type_Insect> typeInsect = {};
public:
    Q():Base(0,0,0) {
        setT(queen);
//        cout << "\nQ: empty Queen";
    }
    Q(string name, int h, int p, int d): Base(h, p, d), name(name) {
        setT(queen);
//        cout << "\nQ: " << name << " Queen";
    }
    ~Q() {
//        cout << "\n~Q: " << name << " " << mather;
    }

    shared_ptr<Empire> getE() { return e.lock();}
    void setE(weak_ptr<Empire> object) { e = object;}
    string getName() { return name;}
    string getMName() {return mather;}
    void setName(std::string n) {this->name = n;}
    void setType(std::vector<Type_Worker> typeWork, std::vector<Type_Warrier> typeWar,
                 std::vector<Type_Insect> typeInsect) {
        this->typeWork = typeWork;
        this->typeWar = typeWar;
        this->typeInsect = typeInsect;
    }
    void setFTPeriod(int f, int t){ this->fp = f; this->tp = t;}
    void setFTDaught(int f, int t){ this->fd = f; this->td = t;}
    void setFTChildren(int f, int t){ this->fc = f; this->tc = t;}

    void set_period() { period = fp + (rand() % ((tp - fp) + 1)); } //min + (rand() % ((max - min) + 1))
    void set_daugher() { countDaughter = fd + (rand() % ((td - fp) + 1)); }
    void set_children() { countChildren = fc + (rand() % ((tc - fc) + 1)); }

    std::vector<Type_Worker> getTWork(){return typeWork;}
    std::vector<Type_Warrier> getTWar(){return typeWar;}
    std::vector<Type_Insect> getTI(){return typeInsect;}

    bool pregnancy();

    void show() {
        cout << "\nQueen:\t" << getName() << " "; Base::show(); show_2();
    }
    void show_2() {
        cout << " period: " << period << " countC: " << countChildren << " countD: " << countDaughter;
    }

    Q newQ();
    int getCountChildren() { return countChildren;}
    int getCountDaughter() { return countDaughter;}

};



//ИМПЕРИЯ---------------------------------------------------------------------------------
using  SharedWorkerPtr = std::shared_ptr<Worker>;
using  SharedWarriorPtr = std::shared_ptr<Warrior>;
using  SharedInsectPtr = std::shared_ptr<SpecialInsect>;

class Empire{
private:
    Q q;
    std::vector<SharedWarriorPtr> warriors = {};
    std::vector<SharedWorkerPtr> workers = {};
    std::vector<SharedInsectPtr> insects = {};
    std::map<Resources, int> resources;
public:
    Empire(Q& q) {
//        cout << "\nEmpire: ";
        this->q = q;
        q.show();
    }

    ~Empire() {
//        cout << "\n~Empire!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
        warriors.clear();
        workers.clear();
        insects.clear();
    }
    void generateWork(int count, Type T = defW);
    void generateWar(int count, Type T = defWa);
    void generatecInsect(int count, Type T = defI);
    void setQptrOnE(weak_ptr<Empire> object) {q.setE(object);}

    friend bool isEnemy(weak_ptr<Empire> a, weak_ptr<Empire> e);

    void clear();

    void toTakeResources();
    void startDay();
    void showArmy();
    Q& getQ() { return q;}

    void randomDistribution();
    std::vector<SharedWorkerPtr>& getWorkers() { return workers;}
    std::vector<SharedInsectPtr>& getInsects() { return insects;}
    std::vector<SharedWarriorPtr>& getWarrior() { return warriors;}

    size_t sumResource() {
        size_t sum = 0;
        for(auto it: resources) {
            sum += it.second;
        }
        return sum;
    }
};


class Heap {
public:
    std::map<Resources, int> resources;
    bool empty = false;

    std::vector<SharedWarriorPtr> warriors = {};
    std::vector<SharedWorkerPtr> workers = {};
    std::vector<SharedInsectPtr> insects = {};

    Heap(int twig = 0, int dewdrop = 0, int pebble = 0, int leaf = 0) {
        resources[Resources::twig] = twig;
        resources[Resources::dewdrop] = dewdrop;
        resources[Resources::pebble] = pebble;
        resources[Resources::leaf] = leaf;
    }

    void addWar(SharedWarriorPtr& w);
    void addWork(SharedWorkerPtr& w);
    void addIns(SharedInsectPtr& w);

    void WAR();
    void TAKE_RES();

    ~Heap() {
//        cout << "\n~Heap";
        delMySelf();
    }

    void delMySelf();
    void totalDel();
    void show();
};

std::vector<shared_ptr<Empire>> vE;
std::vector<Heap> vH;

void newEmpire(Q& q, int cwork = 0, int cwar = 0, int cinsect = 0);

template <class T, class Q>
void hit(T t, Q q);

void startGame(int day);

void showWin(shared_ptr<Empire>& ptr);




//КЛАСС РАБОЧИЙ----------------------------------------------------------------------------
class Worker: virtual public Base {
protected:
    std::map<Resources, int> resources = {};
    int count_canTake = 0;
    std::vector<Resources> v = {};
    weak_ptr<Empire> e;
public:
    Worker();
    Worker(int health = 0, int protection = 0, int canTake = 0, std::vector<Resources> v = {}, Type t = worker):
        Base(health, protection) {
        setT(t);
        this->count_canTake = canTake;
        this->v = v;
        for(auto it: v) {
            resources[it] = 0;
        }
//        cout << "\nWorker";
    }

    virtual ~Worker() {
//        cout << "\n~Worker";
    }
    virtual weak_ptr<Empire> getE() { return e; }
    void setE(weak_ptr<Empire> object) {  e = object; }
//    std::map<Resources, int>& getResources() { return resources;}
    int getCounTake() { return count_canTake;}

    virtual void toTakeResource(std::map<Resources, int>& res) = 0;
    virtual std::map<Resources, int>& toGetResource() { return resources;}

    virtual void show() = 0;
    void show_2() {
        cout << " count_canTake: " << count_canTake << " { ";
        for(auto it: v) cout << to_string(it) << " ";
        cout << " } "; show_3();
    }
    void show_3() {
        cout << " -----Have----- :";
        for(auto& it: resources) {
            cout << " " << it.first << " = " << it.second;
        }
    }
};

class SeniorWorker: public Worker{
public:
    SeniorWorker(int health = 2, int protection = 1, int canTake = 1, std::vector<Resources> v = {leaf, pebble}):
        Base(health, protection),
        Worker::Worker(health, protection, canTake, v, senior_worker) {
//        cout << "\nSeniorWorker";
    }
    ~SeniorWorker() {
//        cout << "\n~SeniorWorker";
    }
    void show() {
        cout << endl << setw(25) << right << "SeniorWorker:\t"; Base::show(); Worker::show_2();
    }
    void toTakeResource(std::map<Resources, int>& res) {
        for(size_t i = 0; i < v.size(); ++i) {
            int a = resources[v.at(i)];
            if(res.find(v.at(i)) == res.end()) continue;
            res[v.at(i)] -= count_canTake;
            resources[v.at(i)] += count_canTake;
            if(res[v.at(i)] < 0) {
                resources[v.at(i)] += res[v.at(i)];
                res[v.at(i)] = 0;
            }
            if(a != resources[v.at(i)]) break;
        }
    }
};

class Advanced_sleepy: public Worker{
    bool sleep = false;
public:
    Advanced_sleepy(int health = 6, int protection = 2, int canTake = 2, std::vector<Resources> v = {/*twig, dewdrop,*/ pebble, leaf}):
        Base(health, protection),
        Worker::Worker(health, protection, canTake, v, advanced_sleepy) {
//        cout << "\nAdvanced_sleepy";
    }
    ~Advanced_sleepy() {
//        cout << "\n~Advanced_sleepy";
    }
    void show() {
        cout << endl << setw(25) << right << "Advanced_sleepy:\t"; Base::show(); Worker::show_2();
    }
    void toTakeResource(std::map<Resources, int>& res) {
        if((rand() % 10) < 3 && !sleep) {
            sleep = true;
            in_operation = false;
        }
        for(size_t i = 0; i < v.size(); ++i) {
            int a = resources[v.at(i)];
            if(res.find(v.at(i)) == res.end()) continue;
            res[v.at(i)] -= count_canTake;
            resources[v.at(i)] += count_canTake;
            if(res[v.at(i)] < 0) {
                resources[v.at(i)] += res[v.at(i)];
                res[v.at(i)] = 0;
            }
            if(a != resources[v.at(i)]) break;
        }
    }
    std::map<Resources, int>& toGetResource() {
        map<Resources, int> res = {};
        cout << "\ntoGetResource, Advanced_sleepy ";
        if(sleep && !in_operation) {
            sleep = false;
//            return {};
        }
        else if(!sleep && !in_operation) {
            in_operation = true;
//            return resources;
        }
        cout << "sleep - " << sleep << endl;
        return sleep == false? res : resources;
    }
};

class An_ordinary_pickpocket: public Worker{
public:
    An_ordinary_pickpocket(int health = 1, int protection = 0, int canTake = 1, std::vector<Resources> v = {twig}):
        Base(health, protection),
        Worker::Worker(health, protection, canTake, v, an_ordinary_pickpocket) {
//        cout << "\nAn_ordinary_pickpocket";
    }
     ~An_ordinary_pickpocket() {
//        cout << "\n~An_ordinary_pickpocket";
    }
    void show() {
        cout << endl << setw(25) << right << "An_ordinary_pickpocket:\t"; Base::show(); Worker::show_2();
    }
    void toTakeResource(std::map<Resources, int>& res) {
        for(size_t i = 0; i < v.size(); ++i) {
            int a = resources[v.at(i)];
            if(res.find(v.at(i)) == res.end()) continue;
            res[v.at(i)] -= count_canTake;
            resources[v.at(i)] += count_canTake;
            if(res[v.at(i)] < 0) {
                resources[v.at(i)] += res[v.at(i)];
                res[v.at(i)] = 0;
            }
            if(a != resources[v.at(i)]) break;
        }
    }
};


//КЛАСС ВОИН-------------------------------------------------------------------------------
class Warrior: virtual public Base {
protected:
    int count_bite = 0;
    int count_targets = 0;
    weak_ptr<Empire> e;
public:
    Warrior(int h = 0, int p = 0, int d = 0, int cb = 0, int ct = 0, Type t = warrior):
        Base(h, p, d), count_bite(cb), count_targets(ct) {
        setT(t);
//        cout << "\nWarrior";
    }
    virtual ~Warrior() {
//        cout << "\n~Warrior";
    }
    virtual weak_ptr<Empire> getE() { return e;}
    void setE(weak_ptr<Empire> object) { e = object;}
    int& getCountBite() {return count_bite;}
    int& getCountTargets() {return count_targets;}
    virtual bool attacked(int damage) { return Base::attacked(damage);}
    virtual void show() = 0;
    void show_2() {
        cout << " count_bite: " << count_bite << " count_targets: " << count_targets;
    }
    virtual void setBT() = 0;
};

class SeniorWarrier: public Warrior {
public:
#define B 1
#define T 1
    SeniorWarrier(int health = 2, int protection = 1, int damage = 2, int cb = 1, int ct = 1):
        Base(health, protection, damage),
        Warrior::Warrior(health, protection, damage, cb, ct, senior_warrier) {
//        cout << "\nSeniorWarrier";
    }
    ~SeniorWarrier() {
//        cout << "\n~SeniorWarrier";
    }
    void show() {
        cout << endl << setw(25) << right << "SeniorWarrier:\t"; Base::show(); Warrior::show_2();
    }
    void setBT() {
        count_bite = B;
        count_targets = T;
    }
#undef B
#undef T
};

class Ordinary: public Warrior {
public:
#define B 1
#define T 1
    Ordinary(int health = 1, int protection = 0, int damage = 1, int cb = 1, int ct = 1):
        Base(health, protection, damage),
        Warrior::Warrior(health, protection, damage, cb, ct, ordinary) {
//        cout << "\nOrdinary";
    }
    ~Ordinary() {
//        cout << "\n~Ordinary";
    }
    void show() {
        cout << endl << setw(25) << right << "Ordinary:\t"; Base::show(); Warrior::show_2();
    }
    void setBT() {
        count_bite = B;
        count_targets = T;
    }
#undef B
#undef T
};

class Advanced: public Warrior {
public:
#define B 2
#define T 1
    Advanced(int health = 6, int protection = 2, int damage = 3, int cb = 2, int ct = 1):
        Base(health, protection, damage),
        Warrior::Warrior(health, protection, damage, cb, ct, advanced) {
//        cout << "\nAdvanced";
    }
    ~Advanced() {
//        cout << "\n~Advanced";
    }
    void show() {
        cout << endl << setw(25) << right << "Advanced:\t"; Base::show(); Warrior::show_2();
    }
    void setBT() {
        count_bite = B;
        count_targets = T;
    }
#undef B
#undef T
};

class Elite_hero: public Warrior {
public:
#define B 2
#define T 2
    Elite_hero(int health = 8, int protection = 4, int damage = 5, int cb = 2, int ct = 2):
        Base(health, protection, damage),
        Warrior::Warrior(health, protection, damage, cb, ct, elite_hero) {
//        cout << "\nElite_hero";
    }
    ~Elite_hero() {
//        cout << "\n~Elite_hero";
    }
    void show() {
        cout << endl << setw(25) << right << "Elite_hero:\t"; Base::show(); Warrior::show_2();
    }
    void setBT() {
        count_bite = B;
        count_targets = T;
    }
#undef B
#undef T
};

class Senior_berserker: public Warrior {
public:
#define B 1
#define T 1
    Senior_berserker(int health = 2, int protection = 1, int damage = 2, int cb = 1, int ct = 1):
        Base(health, protection, damage),
        Warrior::Warrior(health, protection, damage, cb, ct, senior_berserker) {
//        cout << "\nSenior_berserker";
    }
    ~Senior_berserker() {
//        cout << "\n~Senior_berserker";
    }
    void show() {
        cout << endl << setw(25) << right << "Senior_berserker:\t"; Base::show(); Warrior::show_2();
    }
    void setBT() {
        count_bite = B;
        count_targets = T;
    }
#undef B
#undef T
};


//КЛАСС СПЕЦИАЛЬНЫЕ НАСЕКОМЫЕ--------------------------------------------------------------
class SpecialInsect: public Warrior, public Worker{
protected:
    weak_ptr<Empire> e;
public:
    SpecialInsect(int h = 0, int p = 0, int d = 0, int canTake = 0,
                  std::vector<Resources> v = {}, int cb = 0, int ct = 0, Type t = insects):
        Base(h, p, d),
        Warrior::Warrior(h, p, d, cb, ct) , Worker::Worker(h, p, canTake, v) {
        setT(t);
//        cout << "\nSpecialInsect";
    }
    virtual ~SpecialInsect() {
//        cout << "\n~SpecialInsect";
    }
    weak_ptr<Empire> getE() { return e;}
    void setE(weak_ptr<Empire> object) {e = object;}
    bool isWarrier() { return count_bite == 0 ? false : true;}
    virtual bool attacked(int damage) { return Warrior::attacked(damage);}
    virtual void show() = 0;
};

class Butterfly: public SpecialInsect {
public:
#define B 0
#define T 0
    Butterfly(int health = 18, int protection = 17, int damage = 0,
              int canTake = 0, std::vector<Resources> v = {}, int cb = 0, int ct = 0):
        Base(health, protection, damage),
        SpecialInsect::SpecialInsect(health, protection, damage, canTake, v, cb, ct, butterfly) {
//        cout << "\nButterfly";
    }
    ~Butterfly() {
//        cout << "\n~Butterfly";
    }
    bool attacked(int damage) { return Warrior::attacked(damage);}
    void show() {
        cout << endl << setw(25) << right << "Butterfly:\t"; Base::show(); Warrior::show_2(); Worker::show_2();
    }
    void setBT() {
        count_bite = B;
        count_targets = T;
    }
    void toTakeResource(std::map<Resources, int>& res) {
        return;
    }
#undef B
#undef T

};

class Mole_cricket: public SpecialInsect {
public:
#define B 0
#define T 0
    Mole_cricket(int health = 23, int protection = 9, int damage = 0,
                 int canTake = 2, std::vector<Resources> v = {dewdrop}, int cb = 0, int ct = 0):
        Base(health, protection, damage),
        SpecialInsect::SpecialInsect(health, protection, damage, canTake, v, cb, ct, mole_cricket) {
//        cout << "\nMole_cricket";
    }
    ~Mole_cricket() {
//        cout << "\n~Mole_cricket\n";
    }
    bool attacked(int damage) { return Warrior::attacked(damage);}
    void show() {
        cout << endl << setw(25) << right << "Mole_cricket:\t"; Base::show(); Warrior::show_2(); Worker::show_2();
    }
    void setBT() {
        count_bite = B;
        count_targets = T;
    }
    void toTakeResource(std::map<Resources, int>& res) {
        int i = 0;
        if(res.find(v.at(i)) == res.end()) return;
        if(res[v.at(i)] == 0) {
            resources[v.at(i)] += count_canTake;
            return ;
        }
        res[v.at(i)] -= count_canTake;
        resources[v.at(i)] += count_canTake;
        if(res[v.at(i)] < 0) {
            resources[v.at(i)] += res[v.at(i)];
            res[v.at(i)] = 0;
        }
    }
#undef B
#undef T
};




class Tropic{
public:
#define TropicDay 5
#define EffectDay 3
    int day = TropicDay;
    int ef_day = EffectDay;
    bool flag = false;
    void dopEffect(shared_ptr<Empire>& e);
    void revers(shared_ptr<Empire>& e);
    void setTD() { day = TropicDay;}
    void setED() { ef_day = EffectDay;}
#undef TropicDay
#undef EffectDay
};
