/*

	Definitions for features and whatnot

*/

#include "Game.h"
#include "Offsets.h"
#include <iostream>
#include <iostream>
#include <Psapi.h>
#include <string>
#include <thread>
#include <TlHelp32.h>
#include "Vector/SVector.h"
using namespace std;



namespace RainbowSix
{

    uint64_t GetLightManager()
    {
        uint64_t rax = 0ull, rbx = 0ull, rcx = 0ull, rdx = 0ull, rdi = 0ull, rsi = 0ull, r8 = 0ull, r9 = 0ull, r10 = 0ull, r11 = 0ull, r12 = 0ull, r13 = 0ull, r14 = 0ull, r15 = 0ull, rbp = 0ull;
        rax = RPM<uint64_t>(base_address + 0x6C78D70);
        rsi = 0xfb6c0bb890cf1772;
        r8 = RPM<uint64_t>(rax + 0x10);
        r8 = _rotl64(r8, 0x1);
        r8 += rsi;
        return RPM<uint64_t>(r8 + 0x3F8);
    }


    void NoSky()
    {
        WPM<float>(GetLightManager() + 0x2D4, 0.0f);
    }

    void AmbientColor()
    {
        WPM<float>(GetLightManager() + 0x238, 0);
        WPM<float>(GetLightManager() + 0x248, 133);
        WPM<float>(GetLightManager() + 0x258, 190);
    }
    

    struct CPlayerController
    {
        std::uintptr_t Address;

        struct CPawnComponent
        {
            std::uintptr_t Address;

            struct CActor
            {
                std::uintptr_t Address;
            };

        };
    };

    void OutlineActor(CPlayerController::CPawnComponent::CActor Actor)
    {

        const auto VisualSettings = _rotl64((RPM<std::uintptr_t>(Actor.Address + 0x1C8) + 0x7B7F518441D25D3F) ^ 0x8914A2C32D5E8B50, 0x21);
        if (!VisualSettings)
            return;

        std::uintptr_t Properties = RPM<UINT64>(VisualSettings + 0xB0);
        if (!Properties)
            return;

        if ((Properties & 0xFFFFFFFF) != 0x20748000)
            WPM<std::uint32_t>(VisualSettings + 0xB0, 0x20748000);
    }

    std::uintptr_t ControllerList(std::uintptr_t GameManager)
    {
        return _rotl64(RPM<std::uintptr_t>(GameManager + 0x138) - 0x19, 0x12);
    }

    int ControllerCount(std::uintptr_t GameManager)
    {
        return static_cast<int>(_rotl64(RPM<std::uintptr_t>(GameManager + 0x140) - 0x19, 0x12) & 0x3FFFFFFF);
    }

    std::uintptr_t GetGameManager()
    {
        // E8 ? ? ? ? 0F 57 C0 0F 57 C9 85 C0 
        return (_rotl64(RPM<std::uintptr_t>(base_address + 0x82F6288), 52) + 0x7B6DEAF5F516B71Ci64) ^ 0x1745AC3A02EE9E46i64;
    }


    void outlines(UINT64 actor)
    {
        
        UINT64 outline = RPM<UINT64>(actor + 0x1C8);
        if (!outline)
            return;

        outline = _rotl64((outline + 0x7B7F518441D25D3F) ^ 0x8914A2C32D5E8B50, 0x21);
        if (!outline)
            return;

        UINT64 val = RPM<UINT64>(outline + 0xB0);
        if (!val)
            return;

        if ((val & 0xFFFFFFFF) != 0x20748000)
            WPM<UINT32>(outline + 0xB0, 0x20748000);
    }

    CPlayerController::CPawnComponent::CActor LocalActor()
    {
        const auto PlayerController = RainbowSix::CPlayerController{ RPM<std::uintptr_t>(RainbowSix::ControllerList(RainbowSix::GetGameManager()) + (0 * sizeof(std::uintptr_t))) };
        if (!PlayerController.Address)
            return {};

        const auto PawnComponent = RainbowSix::CPlayerController::CPawnComponent{ _rotl64(RPM<std::uintptr_t>(PlayerController.Address + 0x38) - 0x211C0F86ED634100i64, 0x3B) - 0x2F };
        if (!PawnComponent.Address)
            return {};

        const auto Actor = RainbowSix::CPlayerController::CPawnComponent::CActor{ (_rotl64(RPM<std::uintptr_t>(PawnComponent.Address + 0x18), 0x1A) ^ 0xBCC79864C12C2B1Fui64) - 0x5C };
        if (!Actor.Address)
            return {};
        return Actor;
    }

    uintptr_t current_weapon(CPlayerController::CPawnComponent::CActor actor)
    {
        uintptr_t component = RPM<uintptr_t>(actor.Address + 0xD8);
        BYTE index = RPM<BYTE>(actor.Address + 0x228);

        uintptr_t weaponArray = RPM<uintptr_t>(component + (index * sizeof(uintptr_t)));

        uintptr_t entry = _rotl64(_rotl64(RPM<uintptr_t>(weaponArray + 0x608), 4) - 0x6A, 0x2C);
        if (!entry) entry = _rotl64(RPM<uintptr_t>(weaponArray + 0x580) + 0x1FDC8465C49F95B, 0x19) - 0x192C98AA3ECEA41D;

        entry = RPM<uintptr_t>(entry);

        uintptr_t weaponEntry = RPM<uintptr_t>(entry + 0x1B0);
        uintptr_t currentWeapon = ((RPM<uintptr_t>(weaponEntry + 0x218) - 0x754F07C2B92B0D3E) ^ 0xCE9B301A9687670B) - 0x37;

        return currentWeapon;
    }

    uint64_t Profilemanager() {
        auto v10 = _rotl64((RPM<uintptr_t>(base_address + 0x7550348) ^ 0x23) - 0x3D65795183DD42A9i64, 36);
        auto v11 = _rotl64(RPM<uintptr_t>(v10 + 24) - 34i64, 8);
        auto v12 = RPM<uintptr_t>(v11);
        return v12;
    }

    CPlayerController::CPawnComponent::CActor Localplayer()
    {
        auto v13 = _rotl64(RPM<uintptr_t>(Profilemanager() + 24), 37) ^ 0x77i64;
        v13 -= 0x630A5EF86C409DFCi64;

        const auto PawnComponent = RainbowSix::CPlayerController::CPawnComponent{ _rotl64(RPM<std::uintptr_t>(v13 + 0x38) - 0x211C0F86ED634100i64, 0x3B) - 0x2F };
        if (!PawnComponent.Address)
            return {};

        const auto Actor = RainbowSix::CPlayerController::CPawnComponent::CActor{ (_rotl64(RPM<std::uintptr_t>(PawnComponent.Address + 0x18), 0x1A) ^ 0xBCC79864C12C2B1Fui64) - 0x5C };
        if (!Actor.Address)
            return {};

        return Actor;
    }

    float GetHealth(uintptr_t pawn)
    {
        auto entity_actor = Localplayer();
        uintptr_t entity_info = RPM<uintptr_t>(entity_actor.Address + 0xD8);
        uintptr_t main_component = RPM<uintptr_t>(entity_info + 0x8);
        return RPM<int>(main_component + 0x1BC);
    }

    std::uintptr_t TeamReplicationInformation(std::uintptr_t PlayerController)
    {
        return ((RPM<std::uintptr_t>(PlayerController + 0x80) - 0x31i64) ^ 0x3B) - 0x63;
    }

    void namesjit()
    {

    }


    void ns()
    {
        WPM< std::uint32_t >(current_weapon(Localplayer()) + 0x60, 0x76FE6EE0);

    }
}