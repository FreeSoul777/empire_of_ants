#include "main_h.h"


bool Base::attacked(int damage) {
    if(this->p - damage > 0)  this->p -= damage;
    else {
        damage -= this->p;
        this->p = 0;
        this->h -= damage;
        this->isAlive = this->h > 0 ? true : false;
    }
    return this->isAlive;
}

Q Q::newQ(){
    Q new_q = (*this);
    new_q.mather = this->name;
    int pos = new_q.name.find('_');
    if(pos == -1) {
        new_q.name += "_0";
        pos = new_q.name.find('_');
    }
    int n = stoi(new_q.name.substr(pos+1));
    n += 1;
    new_q.name = std::string(new_q.name, 0, pos+1) + std::to_string(n);
    cout << "\nNewQ: name: " << new_q.getName() << " mather: " << new_q.getMName() << endl;
    return (new_q);
}

bool Q::pregnancy() {
    if(period == 0) {
//        cout << " period: " << period << " countChildren: " << countChildren << " countDaughter: " << countDaughter << endl;
        show_2();
        --period;
        return true;
    }
    else if(period == -1) {
        set_period();
        set_daugher();
        set_children();
    }
//    cout << " period: " << period << " countChildren: " << countChildren << " countDaughter: " << countDaughter << endl;
    show_2();
    --period;
    return false;
}

void Empire::generateWar(int cwar, Type T)
{
    std::vector<Type_Warrier> v = q.getTWar();
    bool flag = T.value == 0;
    while(cwar--) {
        if(flag) T = v[0 + rand() % v.size()];
        switch (T.value) {
        case(advanced):
            warriors.emplace_back(std::make_shared<Advanced>(6, 2, 3, 2, 1));
            break;
        case(senior_warrier):
            warriors.emplace_back(std::make_shared<SeniorWarrier>(2, 1, 2, 1, 1));
            break;
        case(elite_hero):
            warriors.emplace_back(std::make_shared<Elite_hero>(8, 4, 5, 2, 2));
            break;
        case(ordinary):
            warriors.emplace_back(std::make_shared<Ordinary>(1, 0, 1, 1, 1));
            break;
        case(senior_berserker):
            warriors.emplace_back(std::make_shared<Senior_berserker>(2, 1, 2, 1, 1));
            break;
        }
        warriors[warriors.size()-1].get()->show();
        warriors[warriors.size()-1].get()->setE(q.getE());
    }
}

void Empire::generateWork(int cwork, Type T)
{
    std::vector<Type_Worker> v = q.getTWork();
    bool flag = T.value == 0;
    while(cwork--) {
        if(flag) T = v[0 + rand() % v.size()];
        switch (T.value) {
        case(senior_worker):
            workers.emplace_back(std::make_shared<SeniorWorker>(2, 1, 1, std::vector<Resources>{leaf, pebble}));
            break;
        case(advanced_sleepy):
            workers.emplace_back(std::make_shared<Advanced_sleepy>(6, 2, 2, std::vector<Resources>{/*twig, dewdrop,*/ pebble, leaf}));
            break;
        case(an_ordinary_pickpocket):
            workers.emplace_back(std::make_shared<An_ordinary_pickpocket>(1, 0, 1, std::vector<Resources>{twig}));
            break;
        }
        workers[workers.size()-1].get()->show();
        workers[workers.size()-1].get()->setE(q.getE());
    }
}

void Empire::generatecInsect(int cinsect, Type T)
{
    std::vector<Type_Insect> v = q.getTI();
    bool flag = T.value == 0;
    while(cinsect--) {
        if(flag) T = v[0 + rand() % v.size()];
        switch (T.value) {
        case(butterfly):
            insects.emplace_back(std::make_shared<Butterfly>(18, 17));
            break;
        case(mole_cricket):
            insects.emplace_back(std::make_shared<Mole_cricket>(23, 9, 0, 2, std::vector<Resources>{dewdrop}));
            break;
        }
        insects[insects.size()-1].get()->show();
        insects[insects.size()-1].get()->setE(q.getE());
    }
}

void Empire::startDay() {
    cout << "\n--------------------startDay--------------------";
    q.show();
    if(q.pregnancy()) {
        for(int i = q.getCountDaughter(); i>0 ; --i) {
            if((bool) 0 + rand() % 2) {
                Q qs = q.newQ();
                newEmpire(qs);
            }
        }
        int count = q.getCountChildren();
        int cwar = 0 + (rand() % ((count-0) + 1)); //min + (rand() % ((max - min) + 1))
        count -= cwar;
        int cwork = (count/2) + (rand() % ((count-(count/2)) + 1));
        count -= cwork;
        generateWar(cwar);
        generateWork(cwork);
        generatecInsect(count);
    }
}

void Empire::randomDistribution()
{
    if(vH.empty()) {
        cout << "\n-----------------------VECTOR FOR HEAP IS EMPTY!!!!-----------------------\n";
        return ;
    }
    for(size_t i = 0; i < warriors.size(); ++i) {
        vH[rand() % vH.size() + 0].addWar(warriors.at(i));
    }
    for(size_t i = 0; i < workers.size(); ++i) {
        vH[rand() % vH.size() + 0].addWork(workers.at(i));
    }
    for(size_t i = 0; i < insects.size(); ++i) {
        vH[rand() % vH.size() + 0].addIns(insects.at(i));
    }
}

void Empire::toTakeResources() {
    for(auto it = workers.begin(); it != workers.end(); ++it) {
        if(!it->get()->getCounTake()) continue;
        for(auto& ii: it->get()->toGetResource()) {
            resources[ii.first] += ii.second;
            ii.second = 0;
        }
    }
    for(auto it = insects.begin(); it != insects.end(); ++it) {
        if(!it->get()->getCounTake()) continue;
        for(auto& ii: it->get()->toGetResource()) {
            resources[ii.first] += ii.second;
            ii.second = 0;
        }
    }
}

void Empire::clear() {
    for(auto it = warriors.begin(); it != warriors.end(); it++) {
        if(!it->get()->getIsAlive()) {
            warriors.erase(it);
            --it;
        }
        else {
            cout << "-------------Recovery-------------\n";
            it->get()->show();
            it->get()->setBT();
            it->get()->show();
            cout << endl;
        }
    }
    for(auto it = workers.begin(); it != workers.end(); it++) {
        if(!it->get()->getIsAlive()) {
            workers.erase(it);
            --it;
        }
    }
    for(auto it = insects.begin(); it != insects.end(); it++) {
        if(!it->get()->getIsAlive()) {
            insects.erase(it);
            --it;
        }
        else {
            cout << "-------------Recovery-------------\n";
            it->get()->show();
            it->get()->setBT();
            it->get()->show();
            cout << endl;
        }
    }
}

void Empire::showArmy() {
    cout << "\n----------------SHOW ARMY----------------\n";
    getQ().show();
    std::cout << "\nPopulation" << " " << workers.size() + warriors.size() + insects.size() << ": "
              << "workers" << "=" << workers.size() << " "
              << "warriors" << "=" << warriors.size() << " "
              << "insects" << "=" << insects.size() << "\n";
    cout << "Resources: ";
    for(pair<Resources, int> i: resources)
        cout << to_string(i.first) << ": " << i.second << " ";
    cout << endl;
}

void newEmpire(Q& q, int cwork, int cwar, int cinsect) {
    shared_ptr<Empire> E = make_shared<Empire>(q);
    E->setQptrOnE(E);
    E->generateWar(cwar);
    E->generateWork(cwork);
    E->generatecInsect(cinsect);
    vE.emplace_back(E);
}

void Heap::addWar(SharedWarriorPtr& w) {
    if(w->in_operation)
        warriors.emplace_back(w);
}

void Heap::addWork(SharedWorkerPtr& w) {
    if(w->in_operation){
        workers.emplace_back(w);
    }
}

void Heap::addIns(SharedInsectPtr& w) {
    if(w->in_operation){
        insects.emplace_back(w);
    }
}

void hit(Warrior* w, Warrior* d) {
    cout << "\n\nBefore: "; w->show(); d->show();
    while(w->getCountBite() || d->getCountBite()) {
        if(!w->getIsAlive()) break;
        if(w->getCountBite() > 0) {
            d->attacked(w->getD());
            --w->getCountBite() ;
        }
        if(!d->getIsAlive()) break;
        if(d->getCountBite() > 0) {
            w->attacked(d->getD());
            --d->getCountBite();
        }
    }
    --w->getCountTargets();
    --d->getCountTargets();
    if(w->getCountTargets() <= 0) w->in_operation = false;
    if(d->getCountTargets() <= 0) d->in_operation = false;
    cout << "\nAfter: "; w->show(); d->show(); cout << endl;
}

void hit(Warrior* w, Worker* d) {
    cout << "\n\nBefore: "; w->show(); d->show();
    while(w->getCountBite()) {
        if(!w->getIsAlive()) break;
        if(w->getCountBite() > 0) {
            d->attacked(w->getD());
            --w->getCountBite() ;
        }
        if(!d->getIsAlive()) break;
    }
    --w->getCountTargets();
    if(w->getCountTargets() <= 0) w->in_operation = false;

    cout << "\nAfter: "; w->show(); d->show(); cout << endl;
}

void Heap::delMySelf() {
    warriors.clear();
    workers.clear();
    insects.clear();
}

bool isEnemy(weak_ptr<Empire> a, weak_ptr<Empire> e){
    if(a.lock() == e.lock()) return false;
    Q& our = a.lock()->q;
    Q& enemy = e.lock()->q;
    if(enemy.getName() == our.getMName()) return false;
    if(enemy.getMName() == our.getName()) return false;
    return true;
}

template <typename  T, typename Q>
void asd(T& t, Q& q) {
    for(auto it = t.begin(); it < t.end(); ++it) {
        for(auto ii = q.begin(); ii < q.end(); ++ii) {
            if(isEnemy(it->get()->getE(), ii->get()->getE())) {
                hit(it->get(), ii->get());
                if(!it->get()->getIsAlive() || !it->get()->in_operation) {
                    it->get()->in_operation = true;
                    t.erase(it);
                    --it; --ii;
                    if(distance(t.begin(), it ) == -1) ++it;
                }
                if(distance(q.begin(), ii ) != -1) {
                    if(!ii->get()->getIsAlive() || !ii->get()->in_operation) {
                        ii->get()->in_operation = true;
                        q.erase(ii);
                        --ii;
                    }
                }
                if(t.empty()) return;
            }
        }
    }
}

void Heap::TAKE_RES() {
    for(auto& it: workers) {
        it.get()->toTakeResource(resources);
    }
    cout << endl << setw(10) << left << "TAKE_RES: ";
    for(pair<Resources, int> i: resources)
        cout << to_string(i.first) << ": " << i.second << " ";
    cout << endl;

    empty = resources[Resources::twig] + resources[Resources::dewdrop] + resources[Resources::leaf] +
            resources[Resources::pebble] <= 0 ;
}

void Heap::WAR() {
    show();
    for(SharedInsectPtr& it: insects) {
        if(it->getCountTargets() > 0)
            warriors.emplace_back(it);
    }
    random_shuffle(warriors.begin(), warriors.end());
//    show();
    asd(warriors, warriors);

    for(SharedInsectPtr& it: insects) {
        if(it->getIsAlive() && it->getCounTake())
            workers.emplace_back(it);
    }
    random_shuffle(workers.begin(), workers.end());
//    show();
    asd(warriors, workers);
    TAKE_RES();
}

void Heap::show() {
    cout << "\n\n------------------------------------------------------------------------\n";
    cout << setw(10) << left << "HEAP: ";
    for(pair<Resources, int> i: resources)
        cout << to_string(i.first) << ": " << i.second << " ";
    cout << endl;
    cout << "\nWAR:";
    for(auto& it: warriors) it.get()->show();
    cout << "\nWORKER:";
    for(auto& it: workers) it.get()->show();
    cout << "\nINSECT:";
    for(auto& it: insects) it.get()->show();
    cout << endl << endl;
}

void Tropic::dopEffect(shared_ptr<Empire>& e) {
    cout <<"\n\n---------------TROPIC DOP_EFFECT---------------\n";
    vector<SharedWorkerPtr>& work = e->getWorkers();
    vector<SharedInsectPtr>& ins = e->getInsects();
    for(auto it = work.begin(); it < work.end(); ++it) {
        e->generatecInsect(1);
        ins[ins.size()-1].get()->setT(it->get()->getT());
        work.erase(it);
        --it;
    }
}
void Tropic::revers(shared_ptr<Empire> &e) {
    cout <<"\n\n---------------TROPIC REVERS---------------\n";
    vector<SharedInsectPtr>& ins = e->getInsects();
    for(auto it = ins.begin(); it < ins.end(); ++it) {
        if(it->get()->getT().get_insect() != defI) continue;
        e->generateWork(1, it->get()->getT());
        ins.erase(it);
        --it;
    }
}


void showWin(shared_ptr<Empire>& ptr) {
    cout << "Встречайте! Наш победитель в Голодных играх:\n";
    ptr->getQ().show();
}

void startGame(size_t day) {
    Tropic t;
    while(day && vH.size() > 0) {
        cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++\n";
        cout << "До полного пиздеца осталось: " << day;
        cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++\n";

        if(t.flag == false || t.ef_day == 0) {
            if(!t.flag){
                t.flag = 0 + rand() % 2;
                if(t.flag == false) goto K;
                else t.setTD();
            }
            if(t.ef_day == 0) {
                t.setED();
            }
            if(t.flag == true) {
                for(auto& it: vE) {
                    t.dopEffect(it);
                }
            }
        }
        K:
        for(size_t i = 0; i < vE.size(); ++i) {
            vE.at(i)->startDay();
            vE.at(i)->randomDistribution();
        }
        for(auto& it: vH) {
            it.WAR();
            it.delMySelf();
        }
        for(auto& it: vE) {
            it->clear();
            it->toTakeResources();
            it->showArmy();
        }
        for(auto it = vH.begin(); it < vH.end(); ++it) {
            if(it->empty){
                vH.erase(it);
                --it;
            }
        }

        --day;
        if(t.flag) {
            --t.day;
            --t.ef_day;
            if(t.ef_day == 0)
                for(auto& it: vE)
                    t.revers(it);
            if(t.day == 0)
                t.flag = false;
        }
    }

    size_t max = -1;
    shared_ptr<Empire> ptr;
    for(auto& it: vE) {
        if(it->sumResource() > max) {
            max = it->sumResource();
            ptr = it;
        }
    }
    showWin(ptr);
}

int main() {
    cout << "\nOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n" << endl;
    {
        setlocale(LC_ALL, "Russia");
        int n;
        while(true) {
            cout << "Input day: ";
            cin >> n;
            if(n < 0) {
                cout << "\nError! Try to again.\n";
            }
            else break;
        }
        Q elizaveta("Elizaveta", 28, 9, 20);
        elizaveta.setFTChildren(1, 15);
        elizaveta.setFTDaught(1, 3);
        elizaveta.setFTPeriod(3, 4);
        elizaveta.setType(std::vector<Type_Worker>{senior_worker, advanced_sleepy},
                          std::vector<Type_Warrier>{senior_warrier, advanced, elite_hero},
                          std::vector<Type_Insect>{butterfly});
        newEmpire(elizaveta, 3, 3, 1);

        Q blanka("Blanka", 24, 6, 21);
        blanka.setFTChildren(1, 15);
        blanka.setFTDaught(1, 3);
        blanka.setFTPeriod(1, 5);
        blanka.setType(std::vector<Type_Worker>{senior_worker, an_ordinary_pickpocket},
                       std::vector<Type_Warrier>{senior_warrier, ordinary, senior_berserker},
                       std::vector<Type_Insect>{mole_cricket});
        newEmpire(blanka, 3, 3, 1);

        vH.push_back(Heap(36, 16, 0, 39));
        vH.push_back(Heap(21, 32, 0, 0));
        vH.push_back(Heap(35, 12, 45, 12));
        vH.push_back(Heap(21, 19, 0, 15));
        vH.push_back(Heap(28, 0, 45, 0));

        startGame(n);
    }

    cout << "\nOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n" << endl;
    return 0;
}


