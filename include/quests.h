#ifndef GUARD_QUESTS_H
#define GUARD_QUESTS_H

#include "constants/quests.h"
#include "main.h"

#define NO_ACTIVE_QUEST ((s8)-1)

struct SideQuest
{
    const u8 *name;
    const u8 *desc;
    const u8 *poc;
    const u8 *map;
    const u8 *reward;
};

enum QuestDifficulty
{
    QUEST_DIFFICULTY_EASY,
    QUEST_DIFFICULTY_MEDIUM,
    QUEST_DIFFICULTY_HARD,
    QUEST_DIFFICULTY_EXTREME,
};

enum QuestFlagCases
{
    QUEST_FLAG_GET_UNLOCKED,
    QUEST_FLAG_SET_UNLOCKED,
    QUEST_FLAG_GET_COMPLETED,
    QUEST_FLAG_SET_COMPLETED,
};

extern const struct SideQuest gSideQuests[SIDE_QUEST_COUNT];

void QuestMenu_Init(MainCallback callback);
void Task_OpenQuestMenuFromStartMenu(u8 taskId);
bool8 StartMenuQuestMenuCallback(void);
void SetQuestMenuActive(void);

s8 GetSetQuestFlag(u8 quest, u8 caseId);
s8 GetActiveQuestIndex(void);
void SetActiveQuest(u8 questId);

void GetSetQuestFlagSpecial(void);
void SetActiveQuestSpecial(void);
void OpenQuestMenuSpecial(void);

#endif // GUARD_QUESTS_H
