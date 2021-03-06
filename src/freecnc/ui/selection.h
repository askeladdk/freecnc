#ifndef _UI_SELECTION_H
#define _UI_SELECTION_H

#include <list>
#include "../freecnc.h"

class Unit;
class Structure;

class Selection
{
public:
    Selection();
    ~Selection();
    bool addUnit(Unit *selunit, bool enemy);
    void removeUnit(Unit *selunit);
    bool addStructure(Structure *selstruct, bool enemy);
    void removeStructure(Structure *selstruct);

    bool saveSelection(unsigned char savepos);
    bool loadSelection(unsigned char loadpos);
    bool mergeSelection(unsigned char loadpos);

    void clearSelection();

    void purge(Unit* sel);
    void purge(Structure* sel);

    bool canAttack() const {return !enemy_selected && (numattacking>0);}
    bool canMove() const {return !enemy_selected && (sel_units.size() > 0);}
    bool isEnemy() const {return enemy_selected;}
    bool empty() const {return sel_units.empty() && sel_structs.empty();}
    // Returns the number of the player who owns all things selected
    unsigned char getOwner() const;
    void moveUnits(unsigned int pos);
    void attackUnit(Unit* target);
    void attackStructure(Structure* target);
    void checkSelection();
    Unit* getRandomUnit();
    bool getWall() const;
    void stop();
private:
    std::list<Unit*> sel_units;
    std::list<Structure*> sel_structs;

    unsigned int numattacking;
    bool enemy_selected;
    std::list<Unit*>saved_unitsel[10];
    std::list<Structure*>saved_structsel[10];
};

#endif
