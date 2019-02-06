#pragma once

#include "graphics/staticobjects.h"
#include "graphics/pose.h"
#include "graphics/animation.h"
#include "game/gametime.h"

#include <cstdint>
#include <string>

#include <daedalus/DaedalusVM.h>

class WorldScript;

class Npc final {
  public:
    enum Anim : uint16_t {
      NoAnim,
      Idle,
      Move,
      MoveBack,
      MoveL,
      MoveR,
      RotL,
      RotR,
      Jump,
      DrawFist,
      DrawWeapon1h
      };

    enum WeaponState : uint8_t {
      NoWeapon,
      Fist,
      W1H,
      W2H
      };

    enum Talent : uint8_t {
      NPC_TALENT_UNKNOWN            = 0,
      NPC_TALENT_1H                 = 1,
      NPC_TALENT_2H                 = 2,
      NPC_TALENT_BOW                = 3,
      NPC_TALENT_CROSSBOW           = 4,
      NPC_TALENT_PICKLOCK           = 5,
      NPC_TALENT_MAGE               = 7,
      NPC_TALENT_SNEAK              = 8,
      NPC_TALENT_REGENERATE         = 9,
      NPC_TALENT_FIREMASTER         = 10,
      NPC_TALENT_ACROBAT            = 11,
      NPC_TALENT_PICKPOCKET         = 12,
      NPC_TALENT_SMITH              = 13,
      NPC_TALENT_RUNES              = 14,
      NPC_TALENT_ALCHEMY            = 15,
      NPC_TALENT_TAKEANIMALTROPHY   = 16,
      NPC_TALENT_FOREIGNLANGUAGE    = 17,
      NPC_TALENT_WISPDETECTOR       = 18,
      NPC_TALENT_C                  = 19,
      NPC_TALENT_D                  = 20,
      NPC_TALENT_E                  = 21,

      NPC_TALENT_MAX                = 22
      };

    enum Attribute : uint8_t {
      ATR_HITPOINTS      = 0,
      ATR_HITPOINTSMAX   = 1,
      ATR_MANA           = 2,
      ATR_MANAMAX        = 3,
      ATR_STRENGTH       = 4,
      ATR_DEXTERITY      = 5,
      ATR_REGENERATEHP   = 6,
      ATR_REGENERATEMANA = 7,
      ATR_MAX
      };

    enum Protection : uint8_t {
      PROT_BARRIER = 0,
      PROT_BLUNT   = 1,
      PROT_EDGE    = 2,
      PROT_FIRE    = 3,
      PROT_FLY     = 4,
      PROT_MAGIC   = 5,
      PROT_POINT   = 6,
      PROT_FALL    = 7,
      PROT_MAX     = 8
      };

    Npc(WorldScript& owner,Daedalus::GameState::NpcHandle hnpc);
    Npc(const Npc&)=delete;
    ~Npc();

    void setView      (StaticObjects::Mesh&& m);
    void setPosition  (float x,float y,float z);
    void setPosition  (const std::array<float,3>& pos);
    void setDirection (float x,float y,float z);
    void setDirection (float rotation);

    std::array<float,3> position() const;
    float               rotation() const;

    void updateAnimation();

    void setName      (const std::string& name);
    void setVisual    (const Skeleton *visual);
    void setVisualBody(StaticObjects::Mesh &&head,StaticObjects::Mesh&& body);
    void setArmour    (StaticObjects::Mesh&& body);
    void setFatness   (float f);
    void setOverlay   (const std::string &name, float time);
    void setScale     (float x,float y,float z);
    void setAnim      (Anim a);
    void setAnim      (Anim a, WeaponState nextSt);
    Anim anim() const { return current; }

    ZMath::float3 animMoveSpeed(Anim a, uint64_t dt) const;
    bool          isFlyAnim() const;

    void    setTalentSkill(Talent t,int32_t lvl);
    int32_t talentSkill(Talent t) const;

    int32_t attribute (Attribute a) const;
    int32_t protection(Protection p) const;

    uint32_t guild() const;
    int32_t  magicCyrcle() const;
    int32_t  level() const;
    int32_t  experience() const;
    int32_t  experienceNext() const;
    int32_t  learningPoints() const;

    void setToFistMode ();
    void setToFightMode(const uint32_t item);
    Daedalus::GameState::ItemHandle
         addItem       (const uint32_t item, size_t count=1);
    void equipItem     (const uint32_t item);

    void closeWeapon();
    void drawWeaponMelee();
    void drawWeapon2H();
    void drawWeaponFist();

    WeaponState weaponState() const { return weaponSt; }

    void addRoutine(gtime s, gtime e, int32_t callback);

  private:
    struct Routine final {
      gtime   start;
      gtime   end;
      int32_t callback=0;
      };

    WorldScript&                   owner;
    Daedalus::GameState::NpcHandle hnpc;
    float                          x=0.f;
    float                          y=0.f;
    float                          z=0.f;
    float                          angle=0.f;
    float                          sz[3]={1.f,1.f,1.f};

    Tempest::Matrix4x4             pos;
    StaticObjects::Mesh            head;
    StaticObjects::Mesh            view;
    StaticObjects::Mesh            armour;

    const Skeleton*                skeleton=nullptr;
    const Animation::Sequence*     animSq  =nullptr;
    uint64_t                       sAnim   =0;
    Anim                           current =NoAnim;
    WeaponState                    weaponSt=WeaponState::NoWeapon;
    Pose                           skInst;

    std::string                    name;
    int32_t                        talents[NPC_TALENT_MAX]={};

    std::vector<Routine>           routines;

    const std::list<Daedalus::GameState::ItemHandle> &getItems();
    size_t getItemCount(const uint32_t id);

    void updatePos();
    void setPos(const Tempest::Matrix4x4& m);
    const Animation::Sequence*     solveAnim(Anim a) const;
    const Animation::Sequence*     solveAnim(Anim a, WeaponState st0, Anim cur, WeaponState st) const;
    const char*                    animName(Anim a, WeaponState st) const;
  };
