/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
SDName: Boss_Ramstein_The_Gorger
SD%Complete: 70
SDComment:
SDCategory: Stratholme
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "stratholme.h"
#include "TemporarySummon.h"

enum Spells
{
    SPELL_TRAMPLE           = 5568,
    SPELL_KNOCKOUT          = 17307
};

enum CreatureId
{
    NPC_MINDLESS_UNDEAD     = 11030
};

class boss_ramstein_the_gorger : public CreatureScript
{
public:
    boss_ramstein_the_gorger() : CreatureScript("boss_ramstein_the_gorger") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetStratholmeAI<boss_ramstein_the_gorgerAI>(creature);
    }

    struct boss_ramstein_the_gorgerAI : public BossAI
    {
        boss_ramstein_the_gorgerAI(Creature* creature) : BossAI(creature, BOSS_RAMSTEIN_THE_GORGER)
        {
            Initialize();
        }

        void Initialize()
        {
            Trample_Timer = 3000;
            Knockout_Timer = 12000;
        }

        uint32 Trample_Timer;
        uint32 Knockout_Timer;

        void Reset() override
        {
            BossAI::Reset();

            Initialize();
        }

        void JustDied(Unit* killer) override
        {
            BossAI::JustDied(killer);

            for (uint8 i = 0; i < 30; ++i)
            {
                if (Creature* mob = me->SummonCreature(NPC_MINDLESS_UNDEAD, 3969.35f+irand(-10, 10), -3391.87f+irand(-10, 10), 119.11f, 5.91f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 30min))
                    mob->AI()->AttackStart(me->SelectNearestTarget(100.0f));
            }
        }

        void UpdateAI(uint32 diff) override
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            //Trample
            if (Trample_Timer <= diff)
            {
                DoCast(me, SPELL_TRAMPLE);
                Trample_Timer = 7000;
            } else Trample_Timer -= diff;

            //Knockout
            if (Knockout_Timer <= diff)
            {
                DoCastVictim(SPELL_KNOCKOUT);
                Knockout_Timer = 10000;
            } else Knockout_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_ramstein_the_gorger()
{
    new boss_ramstein_the_gorger();
}
