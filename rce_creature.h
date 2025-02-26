#ifndef RCE_CREATURE_H
#define RCE_CREATURE_H

extern float creatureX;
extern float creatureY;

void initCreature();
void updateCreaturePosition(float playerX, float playerY);
void drawCreature();
void logCreatureStatus(float playerX, float playerY);

#endif