#ifndef GUARD_MYSTERY_DUNGEON_H
#define GUARD_MYSTERY_DUNGEON_H

// Script API:
// - `special StartMysteryDungeon` then `waitstate`.
// - VAR_RESULT afterwards: 1 if the player reached the stairs, 0 if they fainted or quit.
void StartMysteryDungeon(void);

#endif // GUARD_MYSTERY_DUNGEON_H
