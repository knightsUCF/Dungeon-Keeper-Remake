#pragma once
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include "ThempCreatureData.h"
#include "ThempCreatureParty.h"
#include "ThempCreatureTaskManager.h"
#include "ThempTileArrays.h"
#include <micropather.h>
//Number derived from imp traveling 20 tiles (96 base speed), which took ~6.5 seconds, since thats tiles/second and our world values are in subtiles, we have to multiply it by 3
#define BASESPEED_TO_DELTA(x) (((float)(x)) / 10.645161f)
namespace Themp
{
	class D3D;
	class Object3D;
	struct Sprite;
	class Creature
	{
	public:
		enum class CreatureState { JUST_ENTERED,CREATE_LAIR,UNCERTAIN, HUNGRY, ANNOYED, FIGHTING, EXPLORING, SLEEPING,RESEARCHING,TRAINING,DYING };
		struct CreatureConstantBuffer
		{
			float _AnimIndex; //4
			float _NumAnim; //8
			float _SpriteWidth; //12
			uint32_t _isFrozen; //16
			uint32_t _isFlipped; //16
			uint32_t _isHovered; //16
			uint32_t _dummy1; //16
			uint32_t _dummy2; //16
		};//64

		~Creature();

		//spriteIndex = CreatureData::Creature_X
		Creature(CreatureData::CreatureType spriteIndex);
		void SetPosition(int subTileX, int height, int subTileY);
		void SetSprite(int SpriteID);
		void SetToFreshAnimation(CreatureData::AnimationState anim);
		void Update(float delta);
		void AnimationDoneEvent();
		void ImpUpdate(float delta);
		void CreatureUpdate(float delta);

		//Creatures do activities
		void GetActivity();
		//imps do tasks
		void GetTask();
		bool PathTo(float deltaTime, XMINT2 targetSubTile, bool ignoreWalls = false);
		bool TunnelPathTo(float deltaTime, XMINT2 targetSubTile, bool ignoreWalls);
		void StopOrder();
		void StopActivity();
		void DoAnimationDirectionsImp();
		void DoAnimationDirections();
		void CreateLair();
		void Draw(D3D& d3d);
		void SetVisibility(bool val);

		void CheckVisibility();

		Object3D* m_Renderable = nullptr;
		Sprite* m_Sprite = nullptr;

		 
		// creature base data
		CreatureData m_CreatureData;
		//creature current data
		bool m_AreaNeedsDiscovering = true;
		int m_CurrentGoldHold = 0;
		int m_CurrentHungerLevel = 100;
		int m_CurrentHappiness = 100;
		int m_Level = 1;
		float m_CurrentHealth = 10;
		DirectX::XMINT2 m_LairLocation = XMINT2(-1,-1);
		
		CreatureState m_CurrentState = CreatureState::JUST_ENTERED;
		/////////


		CreatureData::CreatureType m_CreatureID = CreatureData::CREATURE_IMP;
		CreatureData::CreatureSpriteIndex m_CreatureSpriteIndex = CreatureData::CreatureSprite_Imp;
		CreatureData::AnimationState m_AnimState = CreatureData::AnimationState::Walking;
		CreatureData::ImpAnimationState m_ImpAnimState = CreatureData::ImpAnimationState::IMP_Walking;
		DirectX::XMFLOAT3 m_Direction;
		uint8_t m_Owner = Owner_PlayerRed;
		bool m_JustSlapped = false;
		float m_Speed = 0;
		float m_AnimationTime = 0;
		float m_PathLerpTime = 0;
		float m_HungerTimer = 0;
		int m_AnimationIndex = 0;
		unsigned int m_CurrentPathIndex = 0;

		float m_PowerCooldownTimer[10];
		float m_ImpSpecialTimer = 0.0f; //Dig, Claim etc..
		float m_TaskSearchTimer = 0.0f;


		float m_Hero_TimeTillStrike = 0;
		Creature* m_HeroLeader = nullptr;
		CreatureParty::HeroObjective m_HeroObjective;

		char* taskString = "No Activity";
		Entity* m_Lair = nullptr;
		CreatureTaskManager::Order m_Order = CreatureTaskManager::Order(false, XMINT2(-1, -1), XMINT2(-1, -1), -1, nullptr);
		CreatureTaskManager::Activity m_Activity = CreatureTaskManager::Activity(false, XMINT2(-1, -1), XMINT2(-1, -1), -1, nullptr);
		micropather::MPVector<void*> m_Path;
		CreatureConstantBuffer m_CreatureCBData;
		ID3D11Buffer* m_CreatureCB = nullptr;
	};
};
