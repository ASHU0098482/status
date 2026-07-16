#include <list>
#include <vector>
#include <string.h>
#include <pthread.h>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>

#include "Tools/Includes/Logger.h"
#include "Tools/Includes/obfuscate.h"
#include "Tools/Includes/Utils.h"
#include "Tools/SOCKET/Server.h"
#include "Tools/KittyMemory/KittyInclude.hpp"
#include "Hack/Memory.h"
#include "Hack/Offsets.h"
#include "Hack/class.h"
#include "Hack/il2cpp.h"
//Target lib here
#define targetLibName OBFUSCATE("libil2cpp.so")
#include "Tools/Includes/Macros.h"


struct {
    bool enableAimbot = false;
    bool TeleBeta = false;
    bool climbup = false;
    bool autoswitch = false;
    bool speedrun = false;
    bool Aimkilltpv2 = false;
    bool teleport = false;
    bool ghoston = false;
    bool Aimkillrotate = false;
    bool Aimkillrotatev2 = false;
    bool Aimkillrotatev3 = false;
    bool Aimkilltp = false;
    bool downplayer = false;
    bool downaimkill = false;
    bool flyhack = false;
    bool AutoSwitchEnabled = false;
    bool aimbotShoot = false;
    bool aimbotScope = false;
    bool Aimkill = false;
    bool Aimkill360 = false;
    bool AimSilent = false;
    bool AimSilent360 = false;
    bool aimbot = false;
    bool SilentAim = false;
    bool aimbotbody = false;
    bool norecoil = false;
    bool ultraswitch = false;
    float aimbotFOV = 0.0f;
    float aimbotSmoothness = 20.0f;
    float speedValue = 0.0f;
    float FlyUp = 0.0f;
    int FlySpeed = 0;
    bool FOV = true;
    bool enableESP = false;
    bool highjump = false;
    bool doublegun = false;
    bool speedHack = false;
    bool speedhackjoy = false;
    bool undergroundCatapult = false;
    bool catapultDistance = false;
    bool ghostHack = false;
    bool upplayerx = false;
    bool resetguest = false;
    bool wallHack = false;
    bool cameraup = false;
    bool medikitrun = false;
    bool UnlimitedAmmo = false;
    bool teleportcar = false;
    bool telehack = false;
    bool aimbody = false;
    float vehicle_y = 0.0f;
    float vehicle_unY = 0.0f;
} MasterBool;

#include "dobby.h"
#if defined(__aarch64__)
#define DobbyHook A64HookFunction
#endif
int g_screenWidth, g_screenHeight;

ElfScanner g_il2cppELF;




enum Mode {
    InitMode = 1,
    HackMode = 2,
    StopMode = 98,
    EspMode = 99,
};

struct Request {
    int Mode;
    bool boolean;
    int value;
    int ScreenWidth;
    int ScreenHeight;
};

#define maxplayerCount 54

struct PlayerData {
    Vector3 headPosition;
    Vector3 bottomPlayerPosition;

    float health;
    char name[2000];
    bool isDieing;
    float distance;
};

struct Response {
    bool Success;
    int PlayerCount;
    PlayerData Players[maxplayerCount];
};


uintptr_t getLibBase(const char* libName) {
    uintptr_t base = 0;
    FILE* fp = fopen("/proc/self/maps", "rt");
    if (!fp) return 0;

    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, libName)) {
            base = strtoul(line, NULL, 16);
            break;
        }
    }
    fclose(fp);
    return base;
}


MemoryPatch patchFunction;
void unlockMemory(uintptr_t address) {
    long pageSize = sysconf(_SC_PAGESIZE);
    uintptr_t pageStart = address & ~(pageSize - 1);
    mprotect((void*)pageStart, pageSize, PROT_READ | PROT_WRITE | PROT_EXEC);
}

MemoryPatch patchFunction2;MemoryPatch patchFunction3;


void ghoston() {
    uintptr_t libBase = getLibBase("libil2cpp.so");
    if (libBase == 0) {
        return;
    }
    uintptr_t targetOffset = 0x1f4d0ec;//ttt 0x1f4cf40
    uintptr_t targetAddress = libBase + targetOffset;
    unlockMemory(targetAddress);
    patchFunction = MemoryPatch::createWithHex(targetAddress, "00 00 A0 E3 1E FF 2F E1"); // fast switch
    patchFunction.Modify();
}
void ghostoff() {
    uintptr_t libBase = getLibBase("libil2cpp.so");
    if (libBase == 0) {
        return;
    }
    uintptr_t targetOffset = 0x1f4d0ec ;
    uintptr_t targetAddress = libBase + targetOffset;
    unlockMemory(targetAddress);
    patchFunction = MemoryPatch::createWithHex(targetAddress, "D5 C5 75 36 8B DC DD 3E"); // fast switch D5 C5 75 36 8B DC DD 3E //09 0B 1A E8 08 0B 0B 01
    patchFunction.Modify();
}








using namespace std;
std::string utf16le_to_utf82(const std::u16string &u16str) {
    if (u16str.empty()) { return std::string(); }
    const char16_t *p = u16str.data();
    std::u16string::size_type len = u16str.length();
    if (p[0] == 0xFEFF) {
        p += 1;
        len -= 1;
    }

    std::string u8str;
    u8str.reserve(len * 3);

    char16_t u16char;
    for (std::u16string::size_type i = 0; i < len; ++i) {

        u16char = p[i];

        if (u16char < 0x0080) {
            u8str.push_back((char) (u16char & 0x00FF));
            continue;
        }
        if (u16char >= 0x0080 && u16char <= 0x07FF) {
            u8str.push_back((char) (((u16char >> 6) & 0x1F) | 0xC0));
            u8str.push_back((char) ((u16char & 0x3F) | 0x80));
            continue;
        }
        if (u16char >= 0xD800 && u16char <= 0xDBFF) {
            uint32_t highSur = u16char;
            uint32_t lowSur = p[++i];
            uint32_t codePoint = highSur - 0xD800;
            codePoint <<= 10;
            codePoint |= lowSur - 0xDC00;
            codePoint += 0x10000;
            u8str.push_back((char) ((codePoint >> 18) | 0xF0));
            u8str.push_back((char) (((codePoint >> 12) & 0x3F) | 0x80));
            u8str.push_back((char) (((codePoint >> 06) & 0x3F) | 0x80));
            u8str.push_back((char) ((codePoint & 0x3F) | 0x80));
            continue;
        }
        {
            u8str.push_back((char) (((u16char >> 12) & 0x0F) | 0xE0));
            u8str.push_back((char) (((u16char >> 6) & 0x3F) | 0x80));
            u8str.push_back((char) ((u16char & 0x3F) | 0x80));
            continue;
        }
    }

    return u8str;
}
SocketServer server;

int InitServer() {
    if (!server.Create()) {
        return -1;
    }
    if (!server.Bind()) {
        return -1;
    }
    if (!server.Listen()) {
        return -1;
    }
    return 0;
}
typedef struct _monoStringlk {
    void *klass;
    void *monitor;
    int length;
    char chars[1];
    int getLength() {
        return length;
    }
    const char *toChars(){
        u16string ss((char16_t *) getChars(), 0, getLength());
        string str = utf16le_to_utf82(ss);
        return str.c_str();
    }
    char *getChars() {
        return chars;
    }
    std::string get_string() {

        return std::string(toChars());
    }
} monoStringlk;



void CreateExtraSensoryPerceptionhackFixed(Response &response)
{

    if (MasterBool.enableESP) {
        void *GameFacade = *(void **) getRealOffset(0x997EAD0);
        if (GameFacade != nullptr) {
            void *MatchGame = *(void **) ((uint64_t) GameFacade + 0x5C);
            if (MatchGame != nullptr) {
                void *ClassMatchGame = *(void **) ((uint64_t) MatchGame + 0x4);
                if (ClassMatchGame != nullptr) {
                    void *current_match = *(void **) ((uint64_t) ClassMatchGame + 0x50);
                    if (current_match != nullptr) {
                        auto matchStatus = *(uint32_t *) ((uint64_t) current_match + 0x3c);
                        if (matchStatus == 1) {
                            void *CurrentLocalPlayer = Current_Local_Player();
                            if (CurrentLocalPlayer != nullptr) {

                                System_Collections_Generic_Dictionary_IHAAMHPPLMG__Player__o *MonoPlayer = *(System_Collections_Generic_Dictionary_IHAAMHPPLMG__Player__o **)((uint64_t) current_match + pAddress.DictionaryEntities);
                                if (MonoPlayer != nullptr) {
                                    COW_GamePlay_Player_array *players = MonoPlayer->valueSlots;
                                    if (players != nullptr) {
                                        int countagempartida = players->max_length;
                                        if (countagempartida != 1) {
                                            for (int i = 0; i < countagempartida; ++i) {
                                                if (players->m_Items[i] != nullptr &&
                                                    players->m_Items[i] != CurrentLocalPlayer) {
                                                    if (IsStreamerVisible(players->m_Items[i]) &&!IsLocalTeammate(players->m_Items[i]) &&GetHp(players->m_Items[i]) > 0) {

                                                        void *HeadTF = TransformNode(*(void **) ((uintptr_t) players->m_Items[i] +pAddress.HeadTF));
                                                        void *PesTF = TransformNode(*(void **) ((uintptr_t) players->m_Items[i] +pAddress.PesTF));
                                                        void *HeadTFLocal = TransformNode(*(void **) ((uintptr_t) CurrentLocalPlayer +pAddress.HeadTF));
                                                        Vector3 WorldToScreenHead = WorldToScreenPoint(Transform_INTERNAL_GetPosition(HeadTF));

                                                        Vector3 WorldToScreenPes = WorldToScreenPoint(Transform_INTERNAL_GetPosition(PesTF));

                                                        bool Caido2 = IsDieing(players->m_Items[i]);
                                                        float distance = Vector3::Distance(Transform_INTERNAL_GetPosition(HeadTFLocal),Transform_INTERNAL_GetPosition(HeadTF));
                                                        monoStringlk *Nick = *(monoStringlk **) ((uint64_t) players->m_Items[i] + 0x250);
                                                        PlayerData *data = &response.Players[response.PlayerCount];
                                                        if (data != nullptr) {
                                                            data->headPosition = WorldToScreenHead;
                                                            data->bottomPlayerPosition = WorldToScreenPes;
                                                            data->distance = distance;
                                                            data->isDieing = Caido2;
                                                            data->health = GetHp(players->m_Items[i]);
                                                            if (Nick->toChars() != NULL) {
                                                                strcpy(data->name,Nick->toChars());
                                                            }



                                                        }
                                                        ++response.PlayerCount;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        } else {

                            response.PlayerCount = 0;
                        }
                    }
                }
            }
        }

    }
}


void allfunction(){
    if(MasterBool.enableESP){
        if (MasterBool.downaimkill) {
            MasterBool.downplayer = true;
            MasterBool.flyhack = true;

        } else {
            MasterBool.downplayer = false;
            MasterBool.flyhack = false;
        }
    }

    if(MasterBool.enableESP){
        if(MasterBool.Aimkilltpv2){
            ghoston();
        }
        else{
            ghostoff();
        }
    }
}

void *CreateServer(void *) {
    if (InitServer() == 0) {
        if (server.Accept()) {
            Request request{};
            while (server.receive((void*)&request) > 0) {
                Response response{};

                allfunction();

                if (request.Mode == Mode::InitMode) {
                    response.Success = true;
                } else if (request.Mode == Mode::EspMode) {
                    g_screenWidth = request.ScreenWidth;
                    g_screenHeight = request.ScreenHeight;
                    response.Success = true;
                    CreateExtraSensoryPerceptionhackFixed(response);
                } else if (request.Mode == 3) {
                    MasterBool.enableESP = request.boolean;
                    response.Success = true;
                } else if (request.Mode == 109) {
                    MasterBool.speedHack = request.boolean;
                    response.Success = true;
                } else if (request.Mode == 6) {
                    MasterBool.undergroundCatapult = request.boolean;
                    response.Success = true;
                } else if (request.Mode == 101) {
                    MasterBool.enableAimbot = request.boolean;
                    response.Success = true;
                } else if (request.Mode == 102) {
                    MasterBool.aimbotShoot = request.boolean;
                    response.Success = true;
                } else if (request.Mode == 103) {
                    MasterBool.Aimkill = request.boolean;
                    response.Success = true;
                } else if (request.Mode == 104) {
                    MasterBool.aimbotFOV = request.value;
                    // LOGI(" Aim.maxAngle  %f", MasterBool.aimbotFOV);
                    response.Success = true;
                }
                else if (request.Mode == 1055) {
                    MasterBool.Aimkill360 = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 108)
                {
                    MasterBool.aimbotbody = request.boolean;
                    response.Success = true;
                }

                else if(request.Mode == 111)
                {
                    MasterBool.SilentAim = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 1111)
                {
                    MasterBool.cameraup = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 10)
               {
                    MasterBool.ultraswitch = request.boolean;
                   response.Success = true;
               }

                else if(request.Mode == 12)
                {
                    MasterBool.resetguest = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 147)
                {
                    MasterBool.UnlimitedAmmo = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 13)
                {
                    MasterBool.medikitrun = request.boolean;
                    response.Success = true;
                }

                else if(request.Mode == 15)
                {
                    MasterBool.speedhackjoy = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 166)
                {
                    MasterBool.wallHack = request.boolean;
                    response.Success = true;
                }

                else if(request.Mode == 19)
                {
                    MasterBool.telehack = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 20)
                {
                    MasterBool.upplayerx = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 21)
                {
                    MasterBool.aimbody = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 22)
                {
                    MasterBool.AimSilent = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 1056)
                {
                    MasterBool.AimSilent360 = request.boolean;
                    response.Success = true;
                }

                else if(request.Mode == 500)
                {
                    MasterBool.Aimkilltp = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 501)
                {
                    MasterBool.Aimkilltpv2 = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 502)
                {
                    MasterBool.Aimkillrotate = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 503)
                {
                    MasterBool.Aimkillrotatev2 = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 506)
                {
                    MasterBool.Aimkillrotatev3 = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 504)
                {
                    MasterBool.downaimkill = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 505)
                {
                    MasterBool.autoswitch = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 507)
                {
                    MasterBool.speedrun = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 508)
                {
                    MasterBool.TeleBeta = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 149)
                {
                    MasterBool.ghoston = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 509)
                {
                    MasterBool.climbup = request.boolean;
                    response.Success = true;
                } else if(request.Mode == 550)
                {
                    MasterBool.teleport = request.boolean;
                    response.Success = true;
                }
                server.send((void*)& response, sizeof(response));
            }
        }
    }
    return NULL;
}

bool isInsideFOV(int x, int y) {

    float circle_x = g_screenWidth / 2;
    float circle_y = g_screenHeight / 2;
    float rad = MasterBool.aimbotFOV;
    return (x - circle_x) * (x - circle_x) + (y - circle_y) * (y - circle_y) <= rad * rad;
}

Vector3 cSubtract(Vector3 src, Vector3 dst)
{
    Vector3 diff;
    diff.X = src.X - dst.X;
    diff.Y = src.Y - dst.Y;
    diff.Z = src.Z - dst.Z;
    return diff;
}

float cMagnitude(Vector3 vec)
{
    return sqrtf(vec.X*vec.X + vec.Y*vec.Y + vec.Z*vec.Z);
}

float cClamp(float value, float min, float max)
{
    if (value < min)
        value = min;
    else if (value > max)
        value = max;
    return value;
}

float CalculateDistance(Vector3 src, Vector3 dst)
{
    Vector3 diff = cSubtract(src, dst);
    return cMagnitude(diff);
}

float Vector3Distance(const Vector3& a, const Vector3& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    float dz = a.Z - b.Z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
}

void* GetClosestEnemy360()
{
    void* closestEnemy = nullptr;
    float closestDistance = 999999.0f;

    void* GameFacade = *(void**) getRealOffset(0x997EAD0);
    if (!GameFacade) return nullptr;

    void* MatchGame = *(void**)((uint64_t)GameFacade + 0x5C);
    if (!MatchGame) return nullptr;

    void* ClassMatchGame = *(void**)((uint64_t)MatchGame + 0x4);
    if (!ClassMatchGame) return nullptr;

    void* current_match = *(void**)((uint64_t)ClassMatchGame + 0x50);
    if (!current_match) return nullptr;

    uint32_t matchStatus = *(uint32_t*)((uint64_t)current_match + 0x3c);
    if (matchStatus != 1) return nullptr; // Not in a live match

    void* LocalPlayer = Current_Local_Player();
    if (!LocalPlayer) return nullptr;

    Vector3 LocalPosition(0.0f, 0.0f, 0.0f);

    void* LocalHead = TransformNode(*(void**)((uint64_t)LocalPlayer + pAddress.HeadTF));
    if (LocalHead) {
        LocalPosition = Transform_INTERNAL_GetPosition(LocalHead);
    } else {
        return nullptr; // Local player head not found
    }

    auto* MonoPlayer = *(System_Collections_Generic_Dictionary_IHAAMHPPLMG__Player__o**)((uint64_t)current_match + pAddress.DictionaryEntities);
    if (!MonoPlayer) return nullptr;

    COW_GamePlay_Player_array* players = MonoPlayer->valueSlots;
    if (!players) return nullptr;

    int playerCount = players->max_length;
    if (playerCount <= 1) return nullptr; // Only you in match

    for (int i = 0; i < playerCount; ++i)
    {
        void* enemy = players->m_Items[i];
        if (!enemy || enemy == LocalPlayer) continue; // Skip null or self

        // Strong checking if enemy is valid
        if (!IsStreamerVisible(enemy)) continue;
        if (IsDieing(enemy)) continue;
        if (IsLocalTeammate(enemy)) continue;
        if (GetHp(enemy) <= 0) continue;

        void* EnemyHead = TransformNode(*(void**)((uint64_t)enemy + pAddress.HeadTF));
        if (!EnemyHead) continue;

        Vector3 EnemyPosition = Transform_INTERNAL_GetPosition(EnemyHead);

        float Distance = Vector3Distance(LocalPosition, EnemyPosition);

        if (Distance < closestDistance)
        {
            closestDistance = Distance;
            closestEnemy = enemy;
        }
    }

    return closestEnemy;
}






//#define lbil2cpp(address, original, backup)  DobbyHook((void *)getRealOffset(address), (void *)original, (void **)&backup)

void *GetEnemyInsideFOV()
{

    float MaxDist = 9999.0f;
    void *closestEnemy = nullptr;
    void *GameFacade = *(void **) getRealOffset(0x997EAD0);
    if (GameFacade != nullptr) {
        void *MatchGame = *(void **) ((uint64_t) GameFacade + 0x5C);
        if (MatchGame != nullptr) {
            void *ClassMatchGame = *(void **) ((uint64_t) MatchGame + 0x4);
            if (ClassMatchGame != nullptr) {
                void *current_match = *(void **) ((uint64_t) ClassMatchGame + 0x50);
                if (current_match != nullptr) {
                    auto matchStatus = *(uint32_t *) ((uint64_t) current_match + 0x3C);
                    if (matchStatus == 1) {
                        void *CurrentLocalPlayer = Current_Local_Player();
                        if (CurrentLocalPlayer != nullptr) {
                            System_Collections_Generic_Dictionary_IHAAMHPPLMG__Player__o *MonoPlayer = *(System_Collections_Generic_Dictionary_IHAAMHPPLMG__Player__o **)((uint64_t) current_match + pAddress.DictionaryEntities);
                            if (MonoPlayer != nullptr) {
                                COW_GamePlay_Player_array *players = MonoPlayer->valueSlots;
                                if (players != nullptr) {
                                    int countagempartida = players->max_length;
                                    if (countagempartida != 1) {
                                        for (int i = 0; i < countagempartida; ++i) {
                                            if (players->m_Items[i] != nullptr &&players->m_Items[i] != CurrentLocalPlayer) {
                                                if (IsStreamerVisible(players->m_Items[i]) && !IsDieing(players->m_Items[i]) && !IsLocalTeammate(players->m_Items[i]) && GetHp(players->m_Items[i]) > 0) {
                                                    void *HeadTF = TransformNode(*(void **) ((uint64_t) players->m_Items[i] +pAddress.HeadTF));
                                                    if (HeadTF != nullptr) {
                                                        Vector3 currentLocation = Transform_INTERNAL_GetPosition(HeadTF);
                                                        Vector3 WorldToScreenHead = WorldToScreenPoint(currentLocation);

                                                        if (WorldToScreenHead.Z >= 0.01f) {
                                                            Vector3 v2Middle = Vector3((float) (g_screenWidth / 2),(float) (g_screenHeight / 2));
                                                            Vector3 v2Loc = Vector3(WorldToScreenHead.X,WorldToScreenHead.Y);
                                                            float Distance = (float) CalculateDistance(v2Middle, v2Loc);
                                                            if (isInsideFOV((int) WorldToScreenHead.X,(int) WorldToScreenHead.Y)) {
                                                                if (Distance < MaxDist) {
                                                                    closestEnemy = players->m_Items[i];
                                                                    MaxDist = Distance;
                                                                }
                                                            }
                                                        }

                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return closestEnemy;

}



struct DamageInfo2_o {
    void *klass;
    void *monitor;
    int32_t DBLBLKADCNP;
    int32_t KENBMOOEHBG;
    monoString* JANPNJIFOJJ;
    bool NNNADMOFPIE;
    COW_GamePlay_IHAAMHPPLMG_o DHGCIEKPBFA;
    void* GPBDEDFKJNA;
    int32_t PIAMIOFEBKF;
    Vector3 CNEICNJFGLM;
    Vector3 HECJHKEDFEB;
    Vector3 JNLGFLFLBHO;
    uint8_t ACAKHEABPEJ;
    bool MJIHLDJNHLF;
    int32_t LOKIMAEAPCB;
    monoDictionary<uint8_t*, void **> *FHLFLAHCIBN;
};


static int GetDamage(void *pthis)
{
    return ((int (*)(void *))getRealOffset(0x1974AFC))(pthis);
}
COW_GamePlay_IHAAMHPPLMG_o GetplayerID(void *_this)
{
    return ((COW_GamePlay_IHAAMHPPLMG_o (*)(void *))getRealOffset(0x14F0AC0))(_this);
}
static void *GetWeaponOnHand(void *local) {
    void *(*GetWeaponOnHand)(void *local) = (void *(*)(void *))getRealOffset(0x15120A8);
    return GetWeaponOnHand(local);
}
void SwapWeapon(void *Pthis,int32_t FANMJANBFIL,bool GDKLMFLNNGM)
{
    return ((void (*)(void *,int,bool,void*))getRealOffset(0x193F954))(Pthis,FANMJANBFIL,GDKLMFLNNGM,nullptr);
}
bool GameFacade_Send(uint32_t messageID,void *msg,uint8_t sendOption,bool cacheMsgAnyWay)
{
    return ((bool (*)(uint32_t,void *,uint8_t,bool))getRealOffset(0x1A462EC))(messageID,msg,sendOption,cacheMsgAnyWay);
}

uint32_t CFFPIACECIG(COW_GamePlay_IHAAMHPPLMG_o IDNEFEOPGIF)
{
    return ((uint32_t (*)(COW_GamePlay_IHAAMHPPLMG_o))getRealOffset(0x18F6C4C))(IDNEFEOPGIF);
}
void *GKHECDLGAJA(void *pthis, void* a1)
{
    return ((void* (*)(void *,void *))getRealOffset(0x159453C))(pthis,a1);
}
void Syns_SwapWeapon(void *LocalPlayer)
{
    void *WeaponOnHand = GetWeaponOnHand(LocalPlayer);
    if (WeaponOnHand != nullptr)
    {
        void *Class_RUDP_CHANGE_INVENTORY_ON_HAND_c = *(void **)getRealOffset(0x94DEA04); ///
        if (Class_RUDP_CHANGE_INVENTORY_ON_HAND_c == nullptr)
        {
            int max;
            //   max = 2;
            //SwapWeapon(LocalPlayer,rand()%max,1);
        }
        if (Class_RUDP_CHANGE_INVENTORY_ON_HAND_c != nullptr)
        {
            auto RUDP_CHANGE_INVENTORY_ON_HAND = ((void * (*)(void *))getRealOffset(0xF8268C))(Class_RUDP_CHANGE_INVENTORY_ON_HAND_c);/////
            if (RUDP_CHANGE_INVENTORY_ON_HAND)
            {

                *(uint32_t*)((uint64_t)RUDP_CHANGE_INVENTORY_ON_HAND + 0xc) = CFFPIACECIG(GetplayerID(LocalPlayer));
                *(uint32_t*)((uint64_t)RUDP_CHANGE_INVENTORY_ON_HAND + 0x10) = *(uint32_t*)((uint64_t)WeaponOnHand + 0x18);
                GameFacade_Send(108, (GCommon_UDPClientMessageBase_o *)RUDP_CHANGE_INVENTORY_ON_HAND, 2, 0);
            }
        }
    }
    return;
}
monoList<float *> *LCLHHHKFCFP(void *Weapon,void *CAGCICACKCF,void *HFBDJJDICLN,bool LDGHPOPPPNL,DamageInfo2_o *DamageInfo)
{
    return ((monoList<float *> * (*)(void*,void*,void*,bool,DamageInfo2_o*))getRealOffset(0x19A23D0))(Weapon,CAGCICACKCF,HFBDJJDICLN,LDGHPOPPPNL,DamageInfo);
}

int32_t Player_TakeDamage(void *Player, int32_t p_damage, COW_GamePlay_IHAAMHPPLMG_o PlayerID, DamageInfo2_o *DamageInfo, int32_t WeaponDataID, Vector3 FirePos, Vector3 TargetPos, monoList<float *> *CheckParams, void *p_idk1, int32_t p_idk2) {
    return ((int32_t (*)(void *, int32_t, COW_GamePlay_IHAAMHPPLMG_o, DamageInfo2_o *, int32_t, Vector3, Vector3, monoList<float *> *, void *, uint32_t))getRealOffset(0x1908EA4))(Player, p_damage, PlayerID, DamageInfo, WeaponDataID, FirePos, TargetPos, CheckParams, p_idk1, p_idk2);
}

bool (*OriginalFunction)();
bool HookedFunction() {
    return true;
}

void ApplyHook() {
    static bool hookApplied = false;
    if (MasterBool.Aimkill && !hookApplied) {
        void* targetFunction = (void*)getRealOffset(0x1A70220);
        DobbyHook(targetFunction, (void*)&HookedFunction, (void **)&OriginalFunction);
        hookApplied = true;

    }
}



void AimkillStart(void* targetVivo)
{
    if (MasterBool.Aimkill)
    {
        ApplyHook();
        void *LocalPlayer = Current_Local_Player();
        if (LocalPlayer != NULL) {
            void *weaponOnHand = GetWeaponOnHand(LocalPlayer);
            if (weaponOnHand != nullptr)
            {
                //   LOGI("weaponOnHand");
                static bool s_Il2CppMethodIntialized;
                if (!s_Il2CppMethodIntialized)
                {
                    ((void (*)(void *,void*))getRealOffset(0x199E630))(weaponOnHand, *(void**)((uint64_t)LocalPlayer + 0x874));
                    ((void* (*)(void *))getRealOffset(0x4016F28))(LocalPlayer);
                    s_Il2CppMethodIntialized = true;
                }

                void* targetEnemy = nullptr;
                targetEnemy = targetVivo;

                if (targetEnemy != nullptr)
                {

                    void *ObjectPool = *(void **)((uintptr_t)LocalPlayer + 0x874);
                    if(ObjectPool != nullptr)
                    {
                        //   LOGI("ObjectPool");

                        // Syns_SwapWeapon(LocalPlayer);

                        ((void* (*)(void *, void *))getRealOffset(0x193EB50))(LocalPlayer, weaponOnHand);
                        auto weaponDataID = *(uint32_t*)((uint64_t)*(void**)((uint64_t)weaponOnHand + 0x60) + 0x98);
                        auto baseDamage = GetDamage(weaponOnHand);
                        auto playerID = GetplayerID(LocalPlayer);
                        auto playerID2 = GetplayerID(targetEnemy);
                        void *HeadTF = TransformNode(*(void **) ((uint64_t) targetEnemy + pAddress.HeadTF));
                        Vector3 m_Head = Transform_INTERNAL_GetPosition(HeadTF);

                        void *HeadTF2 = TransformNode(*(void **) ((uint64_t) LocalPlayer + pAddress.HeadTF));
                        Vector3 m_HeadLocal = Transform_INTERNAL_GetPosition(HeadTF2);
                        Vector3 LocalPlayerPos = CameraPosition(LocalPlayer);


                        auto DamageS2c = (message_CHDLJFJCPFN_o *)((void* (*)(void *))getRealOffset(0x4016F28))(LocalPlayer);
                        if (DamageS2c)
                        {

                            void *Class_message_DamageInfo_c = *(void **)getRealOffset(0x997EC04);////
                            if (Class_message_DamageInfo_c)
                            {

                                auto DamageInfo = (DamageInfo2_o *)((void * (*)(void *))getRealOffset(0xF8268C))(Class_message_DamageInfo_c);
                                if (DamageInfo)
                                {

                                    *(int*)((char*)DamageInfo + 0x8) = baseDamage;
                                    *(int*)((char*)DamageInfo + 0xC) = 1;
                                    *(void**)((char*)DamageInfo + 0x30) = weaponOnHand;
                                    *(int*)((char*)DamageInfo + 0x34) = weaponDataID;
                                    *(COW_GamePlay_IHAAMHPPLMG_o *)((char*)DamageInfo + 0x18) = playerID;
                                    void *JEEIBOEGGPD = *(void**)((uint64_t)LocalPlayer + 0x874);
                                    *(void**)((uint64_t)JEEIBOEGGPD + string2Offset(OBFUSCATE("0xc"))) = get_gameObject(get_HeadCollider(targetEnemy));
                                    *(void**)((uint64_t)JEEIBOEGGPD + string2Offset(OBFUSCATE("0x10"))) = get_HeadCollider(targetEnemy);
                                    *(int*)((uint64_t)JEEIBOEGGPD + 0x50) = 0.7;
                                    monoList<float *> *CheckParametros = LCLHHHKFCFP(GetWeaponOnHand(LocalPlayer),GKHECDLGAJA(LocalPlayer,*(void**)((uint64_t)LocalPlayer + 0x874)),get_HeadCollider(targetEnemy),false,DamageInfo);

                                    Player_TakeDamage(targetEnemy, baseDamage, playerID, DamageInfo, weaponDataID, m_HeadLocal, m_Head, CheckParametros, CheckParametros, 0);
                                }
                            }
                        }
                    }
                }
            }

        }
    }

    return;
}


void ApplyHookk() {
    static bool hookApplied = false;
    if (MasterBool.Aimkill360 && !hookApplied) {
        void* targetFunction = (void*)getRealOffset(0x1A70220);
        DobbyHook(targetFunction, (void*)&HookedFunction, (void **)&OriginalFunction);
        hookApplied = true;

    }
}



void AimKillStart360(void* targetVivo)
{
    if (MasterBool.Aimkill360)
    {
        ApplyHook();
        void *LocalPlayer = Current_Local_Player();
        if (LocalPlayer != NULL) {
            void *weaponOnHand = GetWeaponOnHand(LocalPlayer);
            if (weaponOnHand != nullptr)
            {
                //   LOGI("weaponOnHand");
                static bool s_Il2CppMethodIntialized;
                if (!s_Il2CppMethodIntialized)
                {
                    ((void (*)(void *,void*))getRealOffset(0x199E630))(weaponOnHand, *(void**)((uint64_t)LocalPlayer + 0x874));
                    ((void* (*)(void *))getRealOffset(0x4016F28))(LocalPlayer);
                    s_Il2CppMethodIntialized = true;
                }

                void* targetEnemy = nullptr;
                targetEnemy = targetVivo;

                if (targetEnemy != nullptr)
                {

                    void *ObjectPool = *(void **)((uintptr_t)LocalPlayer + 0x874);
                    if(ObjectPool != nullptr)
                    {
                        //   LOGI("ObjectPool");

                        // Syns_SwapWeapon(LocalPlayer);

                        ((void* (*)(void *, void *))getRealOffset(0x193EB50))(LocalPlayer, weaponOnHand);
                        auto weaponDataID = *(uint32_t*)((uint64_t)*(void**)((uint64_t)weaponOnHand + 0x60) + 0x98);
                        auto baseDamage = GetDamage(weaponOnHand);
                        auto playerID = GetplayerID(LocalPlayer);
                        auto playerID2 = GetplayerID(targetEnemy);
                        void *HeadTF = TransformNode(*(void **) ((uint64_t) targetEnemy + pAddress.HeadTF));
                        Vector3 m_Head = Transform_INTERNAL_GetPosition(HeadTF);

                        void *HeadTF2 = TransformNode(*(void **) ((uint64_t) LocalPlayer + pAddress.HeadTF));
                        Vector3 m_HeadLocal = Transform_INTERNAL_GetPosition(HeadTF2);
                        Vector3 LocalPlayerPos = CameraPosition(LocalPlayer);


                        auto DamageS2c = (message_CHDLJFJCPFN_o *)((void* (*)(void *))getRealOffset(0x4016F28))(LocalPlayer);
                        if (DamageS2c)
                        {

                            void *Class_message_DamageInfo_c = *(void **)getRealOffset(0x997EC04);////
                            if (Class_message_DamageInfo_c)
                            {

                                auto DamageInfo = (DamageInfo2_o *)((void * (*)(void *))getRealOffset(0xF8268C))(Class_message_DamageInfo_c);
                                if (DamageInfo)
                                {

                                    *(int*)((char*)DamageInfo + 0x8) = baseDamage;
                                    *(int*)((char*)DamageInfo + 0xC) = 1;
                                    *(void**)((char*)DamageInfo + 0x30) = weaponOnHand;
                                    *(int*)((char*)DamageInfo + 0x34) = weaponDataID;
                                    *(COW_GamePlay_IHAAMHPPLMG_o *)((char*)DamageInfo + 0x18) = playerID;
                                    void *JEEIBOEGGPD = *(void**)((uint64_t)LocalPlayer + 0x874);
                                    *(void**)((uint64_t)JEEIBOEGGPD + string2Offset(OBFUSCATE("0xc"))) = get_gameObject(get_HeadCollider(targetEnemy));
                                    *(void**)((uint64_t)JEEIBOEGGPD + string2Offset(OBFUSCATE("0x10"))) = get_HeadCollider(targetEnemy);
                                    *(int*)((uint64_t)JEEIBOEGGPD + 0x50) = 0.7;
                                    monoList<float *> *CheckParametros = LCLHHHKFCFP(GetWeaponOnHand(LocalPlayer),GKHECDLGAJA(LocalPlayer,*(void**)((uint64_t)LocalPlayer + 0x874)),get_HeadCollider(targetEnemy),false,DamageInfo);

                                    Player_TakeDamage(targetEnemy, baseDamage, playerID, DamageInfo, weaponDataID, m_HeadLocal, m_Head, CheckParametros, CheckParametros, 0);
                                }
                            }
                        }
                    }
                }
            }

        }
    }

    return;
}
void Telekillkk(void* targetVivo)
{

    if (MasterBool.telehack)
    {
        void *LocalPlayer = Current_Local_Player();
        if (LocalPlayer != NULL) {
            void *weaponOnHand = GetWeaponOnHand(LocalPlayer);
            if (weaponOnHand != nullptr)
            {
                //LOGI("weaponOnHand");
                static bool s_Il2CppMethodIntialized;
                if (!s_Il2CppMethodIntialized)
                {
                    ((void (*)(void *,void*))getRealOffset(0x1821DCC))(weaponOnHand, *(void**)((uint64_t)LocalPlayer + 0x854));
                    ((void* (*)(void *))getRealOffset(0x13AE504))(LocalPlayer);
                    s_Il2CppMethodIntialized = true;
                }

                void* targetEnemy = targetVivo;

                if (targetEnemy != nullptr)
                {
                    if (MasterBool.telehack && targetEnemy != nullptr && LocalPlayer != nullptr) {
                        // Get transforms
                        void* enemyRoot = *(void**)((uintptr_t)targetEnemy + 0x3B8);  // Root bone
                        if (!enemyRoot) return;

                        void* enemyTransform = *(void**)((uintptr_t)enemyRoot + 0x8);  // TransformNode
                        if (!enemyTransform) return;

                        void* enemyTransformObj = *(void**)((uintptr_t)enemyTransform + 0x8);
                        if (!enemyTransformObj) return;

                        void* enemyMatrix = *(void**)((uintptr_t)enemyTransformObj + 0x20);
                        if (!enemyMatrix) return;

                        void* playerRoot = *(void**)((uintptr_t)LocalPlayer + 0x380);  // Local root bone
                        if (!playerRoot) return;

                        void* playerTransform = *(void**)((uintptr_t)playerRoot + 0x8);
                        if (!playerTransform) return;

                        void* playerTransformObj = *(void**)((uintptr_t)playerTransform + 0x8);
                        if (!playerTransformObj) return;

                        void* playerMatrix = *(void**)((uintptr_t)playerTransformObj + 0x20);
                        if (!playerMatrix) return;

                        Vector3 localPosition = *(Vector3*)((uintptr_t)playerMatrix + 0x80);  // Get real root pos
                        Vector3 enemyPosition = *(Vector3*)((uintptr_t)enemyMatrix + 0x80);

                        float distance = Vector3Distance(enemyPosition, localPosition);
                        if (distance <= 10.0f) {
                            *(Vector3*)((uintptr_t)enemyMatrix + 0x80) = localPosition;  // Teleport
                        }

                        //Speed Hack
                        /*void* timeService = *(void**)((uintptr_t)currentGame + 0x10); // GameTimer
                        if (!timeService) return;

                        // Set higher FixedDeltaTime (faster game)
                        *(float*)((uintptr_t)timeService + 0x24) = 0.065f; // FixedDeltaTime*/
                    }



                }
            }
        }
    }


    return;
}

void ForceAutoSwitchReliable(void* LocalPlayer) {

    if (MasterBool.autoswitch)
    {
        static int lastSlot = 2;
        static int frameCounter = 0;
        static const int switchDelayFrames = 13; // Delay in frames (60 frames ≈ 1 second at 60 FPS)

        if (++frameCounter < switchDelayFrames) return;
        frameCounter = 0;

        void* weaponOnHand = GetWeaponOnHand(LocalPlayer);
        if (!weaponOnHand) return;

        int newSlot = (lastSlot == 1) ? 2 : 1;

        SwapWeapon(LocalPlayer, newSlot, 1);
        lastSlot = newSlot;
    }
}


void dwonplayerloop() {
    static auto last = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();

    // ✅ 1 switch every 20ms (50 times/second) — FAST but not game-breaking
    long long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();
    if (elapsed < 45) return;
    last = now;

    void* localPlayer = Current_Local_Player();
    if (localPlayer == nullptr) return;

    ForceAutoSwitchReliable(localPlayer); // ✅ Fast reliable call

    if (localPlayer != nullptr) {
        void *ClosestEnemy = GetClosestEnemy360();
        if (ClosestEnemy != nullptr) {
/*
            if (MasterBool.flyhack && localPlayer != nullptr) {
                auto tf = Component_get_transform(localPlayer);
                Vector3 currentPos = Transform_INTERNAL_GetPosition(tf);

                currentPos.Y -= 1.35f;  // ⬇️ Go down from current position
                Transform_set_position(tf, currentPos);
            }*/
        }
        if (MasterBool.downplayer && ClosestEnemy != nullptr) {
            auto enemyTransform = Component_get_transform(ClosestEnemy);



            // Center point where enemy will rotate around (its original position)
            Vector3 centerPos = Transform_INTERNAL_GetPosition(enemyTransform);




            Vector3 newPos;
            newPos.X = centerPos.X;
            newPos.Z = centerPos.Z ;
            newPos.Y = centerPos.Y - 1.60f; // Fly slightly up

            // Apply new position (enemy flies and orbits)
            Transform_set_position(enemyTransform, newPos);
        }
    }


}


void ApplyAimKillToAllEnemies(void* LocalPlayer) {
    void* GameFacade = *(void**) getRealOffset(0x94D8E2C);
    if (!GameFacade) return;

    void* MatchGame = *(void**) ((uint64_t) GameFacade + 0x5C);
    if (!MatchGame) return;

    void* ClassMatchGame = *(void**) ((uint64_t) MatchGame + 0x4);
    if (!ClassMatchGame) return;

    void* current_match = *(void**) ((uint64_t) ClassMatchGame + 0x50);
    if (!current_match) return;

    auto matchStatus = *(uint32_t*)((uint64_t) current_match + 0x3C);
    if (matchStatus != 1) return;

    System_Collections_Generic_Dictionary_IHAAMHPPLMG__Player__o* MonoPlayer = *(System_Collections_Generic_Dictionary_IHAAMHPPLMG__Player__o**)((uint64_t) current_match + pAddress.DictionaryEntities);
    if (!MonoPlayer) return;

    COW_GamePlay_Player_array* players = MonoPlayer->valueSlots;
    if (!players) return;

    int count = players->max_length;
    if (count <= 1) return;

    for (int i = 0; i < count; ++i) {
        void* enemy = players->m_Items[i];
        if (!enemy || enemy == LocalPlayer) continue;
        if (!IsStreamerVisible(enemy) || IsDieing(enemy) || IsLocalTeammate(enemy) || GetHp(enemy) <= 0) continue;

        void* enemyTransform = Component_get_transform(enemy);
        if (!enemyTransform) continue;



        // Aimkillrotate - make them rotate
        if (MasterBool.Aimkillrotatev3) {
            static float spinAngle = 0.0f;
            spinAngle += 122.0f;
            if (spinAngle >= 360.0f) spinAngle = 0.0f;

            Vector3 centerPos = Transform_INTERNAL_GetPosition(enemyTransform);
            float radius = 4.0f;

            Vector3 newPos;
            newPos.X = centerPos.X + radius * cosf(spinAngle * 3.14159f / 180.0f);
            newPos.Z = centerPos.Z + radius * sinf(spinAngle * 3.14159f / 180.0f);
            newPos.Y = centerPos.Y + 1.0f + 0.04f;

            Transform_set_position(enemyTransform, newPos);
        }


    }
}

/*void UpPlayer(void* targetVivo)
{
    if (MasterBool.upplayerx)
    {
        void *LocalPlayer = Current_Local_Player();
        if (LocalPlayer != NULL) {
            void *weaponOnHand = GetWeaponOnHand(LocalPlayer);
            if (weaponOnHand != nullptr)
            {
                //LOGI("weaponOnHand");
                static bool s_Il2CppMethodIntialized;
                if (!s_Il2CppMethodIntialized)
                {
                    ((void (*)(void *,void*))getRealOffset(0x16DBEE4))(weaponOnHand, *(void**)((uint64_t)LocalPlayer + 0x7DC));
                    ((void* (*)(void *))getRealOffset(0x12666D4))(LocalPlayer);
                    s_Il2CppMethodIntialized = true;
                }

                void* targetEnemy = targetVivo;

                if (targetEnemy != nullptr)
                {


                    if (MasterBool.upplayerx && targetEnemy != nullptr)
                    {
                        // Get enemy root bone
                        void* enemyRoot = *(void**)((uintptr_t)targetEnemy + 0x380);  // Bones::Root
                        if (!enemyRoot) return;

                        void* enemyTransform = *(void**)((uintptr_t)enemyRoot + 0x8);
                        if (!enemyTransform) return;

                        void* enemyTransformObj = *(void**)((uintptr_t)enemyTransform + 0x8);
                        if (!enemyTransformObj) return;

                        void* enemyMatrix = *(void**)((uintptr_t)enemyTransformObj + 0x20);
                        if (!enemyMatrix) return;

                        // Get current position
                        Vector3 enemyPosition = *(Vector3*)((uintptr_t)enemyMatrix + 0x80);

                        // Raise enemy 1.5f units vertically
                        enemyPosition.Y += 1.5f;

                        // Write updated position
                        *(Vector3*)((uintptr_t)enemyMatrix + 0x80) = enemyPosition;
                    }
                }
            }
        }
    }
    return;
}*/





static float get_Range(void *pthis)
{
    return ((float (*)(void *))getRealOffset(0x196C29C))(pthis);
}
bool isEnemyInRangeWeapon(void *player, void *enemy, void* weapon)
{
    if (player != nullptr && enemy != nullptr && weapon != nullptr)
    {
        void *HeadTF = TransformNode(*(void **) ((uint64_t) enemy + pAddress.HeadTF));
        void *HeadTF2 = TransformNode(*(void **) ((uint64_t) player + pAddress.HeadTF));
        Vector3 EnemyHeadPosition = Transform_INTERNAL_GetPosition(HeadTF);
        Vector3 PlayerHeadPosition = Transform_INTERNAL_GetPosition(HeadTF2);
        float distance = Vector3::Distance(PlayerHeadPosition, EnemyHeadPosition);
        float range = get_Range(weapon);

        if (distance <= range) {
            return true;
        }
    }
    return false;
}

std::chrono::steady_clock::time_point last_update_time = std::chrono::steady_clock::now();

GCommon_AnimationRuntimeHandle_o *(*GetCurrentRunningHandler)(GCommon_AnimationSystemComponent_o *Instance,int32_t layerIndex);
GCommon_AnimationRuntimeHandle_o *_GetCurrentRunningHandler(GCommon_AnimationSystemComponent_o *Instance,int32_t layerIndex)
{

    dwonplayerloop();

    if (Instance != nullptr) {
        if (MasterBool.enableESP) {
            std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
            auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_update_time).count();
            if (elapsed_time > 1000 /30) {

                void *LocalPlayer = Current_Local_Player();
                if (LocalPlayer != nullptr) {
                    ApplyAimKillToAllEnemies(LocalPlayer);
                    ForceAutoSwitchReliable(LocalPlayer); // ✅ Fast reliable call

                    void *ClosestEnemy = GetEnemyInsideFOV();
                    if (ClosestEnemy != nullptr) {
                        void *weaponOnHand = GetWeaponOnHand(LocalPlayer);


                        if (MasterBool.Aimkilltp && ClosestEnemy != nullptr) {
                            Vector3 gotten = Transform_INTERNAL_GetPosition(
                                    Component_get_transform(LocalPlayer));
                            Vector3 enemyPos = Transform_INTERNAL_GetPosition(
                                    Component_get_transform(ClosestEnemy));

                            float realDistance = sqrt(pow(enemyPos.X - gotten.X, 2) +
                                                      pow(enemyPos.Y - gotten.Y, 2) +
                                                      pow(enemyPos.Z - gotten.Z, 2));

                            if (realDistance < 9.0f) {
                                Vector3 coord = gotten;
                                Transform_set_position(
                                        Component_get_transform(ClosestEnemy), coord);

                            }
                        }



                        if(MasterBool.climbup) {
                            Vector3 gotten = Transform_INTERNAL_GetPosition(
                                    Component_get_transform(LocalPlayer));

                            // Initialize a new coordinate vector with the enemy's current position
                            Vector3 coord(0, 0, 0);
                            coord.X = gotten.X;
                            coord.Y = gotten.Y;
                            coord.Z = gotten.Z;

                            // Adjust to Y coordinate by adding 1.0f
                            //coord.Y += 1.7f;//coord.Y += 1.2f; 2 buga muito

                            coord.Y -= 5.0f;//coord.Y += 1.2f;

                            // Defina a nova posição do inimigo
                            Transform_set_position(Component_get_transform(LocalPlayer), coord);
                        }


                        if (MasterBool.Aimkillrotate && ClosestEnemy != nullptr) {
                            auto enemyTransform = Component_get_transform(ClosestEnemy);

                            // Static angle value to keep rotating every frame
                            static float spinAngle = 0.0f;
                            spinAngle += 122.0f;  // Rotation speed (higher = faster) 15.0f
                            if (spinAngle >= 360.0f) spinAngle = 0.0f;

                            // Center point where enemy will rotate around (its original position)
                            Vector3 centerPos = Transform_INTERNAL_GetPosition(enemyTransform);

                            // Radius of circular motion
                            float radius = 4.0f;//5.0f

                            // Create new rotated position using sin/cos
                            Vector3 newPos;
                            newPos.X = centerPos.X + radius * cosf(spinAngle * 3.14159f / 180.0f);
                            newPos.Z = centerPos.Z + radius * sinf(spinAngle * 3.14159f / 180.0f);
                            newPos.Y = centerPos.Y + 0.01f; // Fly slightly up

                            // Apply new position (enemy flies and orbits)
                            Transform_set_position(enemyTransform, newPos);
                        }
                        if(MasterBool.upplayerx) {
                            void *closestEnemy = GetEnemyInsideFOV();
                            if (closestEnemy != NULL) {
                                Vector3 gotten = Transform_INTERNAL_GetPosition(
                                        Component_get_transform(closestEnemy));

                                // Initialize a new coordinate vector with the enemy's current position
                                Vector3 coord(0, 0, 0);
                                coord.X = gotten.X;
                                coord.Y = gotten.Y;
                                coord.Z = gotten.Z;

                                // Adjust to Y coordinate by adding 1.0f
                                //coord.Y += 1.7f;//coord.Y += 1.2f; 2 buga muito

                                coord.Y += 0.1f + 0.03f;//coord.Y += 1.2f;

                                // Defina a nova posição do inimigo
                                Transform_set_position(Component_get_transform(closestEnemy), coord);
                            }
                        }
                        if (MasterBool.teleport && ClosestEnemy != 0) {
                            Vector3 localPos = Transform_INTERNAL_GetPosition(
                                    Component_get_transform(LocalPlayer));

                            Vector3 enemyPos = Transform_INTERNAL_GetPosition(
                                    Component_get_transform(ClosestEnemy));

                            float realDistance = sqrt(pow(enemyPos.X - localPos.X, 2) +
                                                      pow(enemyPos.Y - localPos.Y, 2) +
                                                      pow(enemyPos.Z - localPos.Z, 2));

                            if (realDistance < 200.0f) {
                                // Teleport yourself to the enemy's position
                                Transform_set_position(
                                        Component_get_transform(LocalPlayer), enemyPos);


                            }
                        }
                        if (MasterBool.Aimkillrotatev2 && ClosestEnemy != nullptr) {
                            auto enemyTransform = Component_get_transform(ClosestEnemy);
                            // Static angle value to keep rotating every frame
                            static float spinAngle = 0.0f;
                            spinAngle += 122.0f;  // Rotation speed (higher = faster) 15.0f
                            if (spinAngle >= 360.0f) spinAngle = 0.0f;

                            // Center point where enemy will rotate around (its original position)
                            Vector3 centerPos = Transform_INTERNAL_GetPosition(enemyTransform);

                            // Radius of circular motion
                            float radius = 4.0f;//5.0f

                            // Create new rotated position using sin/cos
                            Vector3 newPos;
                            newPos.X = centerPos.X + radius * cosf(spinAngle * 3.14159f / 180.0f);
                            newPos.Z = centerPos.Z + radius * sinf(spinAngle * 3.14159f / 180.0f);
                            newPos.Y = centerPos.Y + 1.0f + 0.04f; // Fly slightly up

                            // Apply new position (enemy flies and orbits)
                            Transform_set_position(enemyTransform, newPos);
                        }


                        if (weaponOnHand != nullptr) {
                            if (isEnemyInRangeWeapon(LocalPlayer, ClosestEnemy, weaponOnHand)) {

                                if(isVisible_Aimbot(ClosestEnemy)) {

                                    AimkillStart(ClosestEnemy);


                                }
                            }
                        }


                    }
                    void *ClosestEnemys = GetClosestEnemy360();/////////////////////////////////////////


                    if (ClosestEnemys != nullptr) {
                        void *weaponOnHand = GetWeaponOnHand(LocalPlayer);

                        static bool flySet = false;
                        static Vector3 lockPos;

                        if (MasterBool.flyhack && ClosestEnemys != nullptr) {

                            auto tf = Component_get_transform(LocalPlayer);
                            Vector3 pos = Transform_INTERNAL_GetPosition(tf);

                            if (!flySet) {
                                lockPos = pos;
                                lockPos.Y -= 3.4f; // Ground ke andar le jao//    3.5
                                Transform_set_position(tf, lockPos);
                                flySet = true;
                            }

                            // Sirf Y lock, X/Z mat chedho
                            pos.Y = lockPos.Y;
                            Transform_set_position(tf, pos);

                        }
                        else {
                            flySet = false; // Flyhack off

                        }


                        if (weaponOnHand != nullptr) {
                            if (isEnemyInRangeWeapon(LocalPlayer, ClosestEnemy, weaponOnHand)) {
                                if(isVisible_Aimbot(ClosestEnemys)) {
                                    AimKillStart360(ClosestEnemys);
                                }
                            }
                        }
                    }

                }
                last_update_time = std::chrono::steady_clock::now();
            }

            return GetCurrentRunningHandler(Instance, layerIndex);
        }
    }
    return GetCurrentRunningHandler(Instance,layerIndex);
}



struct My_Patches {
    MemoryPatch ashu;
}mrkiller;


bool (*Old_Physics$$Raycast)(Vector3, Vector3, UnityEngine_RaycastHit_o *hitInfo, float, int32_t);
bool New_Physics$$Raycast(Vector3 origin, Vector3 direction,UnityEngine_RaycastHit_o *hitInfo, float maxDistance, int32_t layerMask) {
    if (MasterBool.enableESP) {
        if (MasterBool.AimSilent) {
            void *closestEnemy = GetEnemyInsideFOV();
            if (closestEnemy != NULL) {

                void *HeadTF = TransformNode(
                        *(void **) ((uint64_t) closestEnemy + pAddress.HeadTF));

                Vector3 enemyHeadPosition = Transform_INTERNAL_GetPosition(HeadTF);

                direction = enemyHeadPosition - origin;

                auto position = WorldToScreenPoint(enemyHeadPosition);
                position.Y = g_screenHeight - position.Y;
                if (pow(position.X - g_screenWidth / 2, 2) +
                    pow(position.Y - g_screenHeight / 2, 2) >= pow(MasterBool.aimbotFOV, 2)) {
                    direction = enemyHeadPosition - origin;
                }
            }

        }
        if (MasterBool.AimSilent360) {
            void *closestEnemy = GetClosestEnemy360();
            if (closestEnemy != NULL) {

                void *HeadTF = TransformNode(
                        *(void **) ((uint64_t) closestEnemy + pAddress.HeadTF));

                Vector3 enemyHeadPosition = Transform_INTERNAL_GetPosition(HeadTF);

                direction = enemyHeadPosition - origin;

                auto position = WorldToScreenPoint(enemyHeadPosition);
                position.Y = g_screenHeight - position.Y;
                if (pow(position.X - g_screenWidth / 2, 2) +
                    pow(position.Y - g_screenHeight / 2, 2) >= pow(MasterBool.aimbotFOV, 2)) {
                    direction = enemyHeadPosition - origin;
                }
            }

        }
       /* if(MasterBool.upplayerx) {
            void *closestEnemy = GetEnemyInsideFOV();
            if (closestEnemy != NULL) {
                Vector3 gotten = Transform_INTERNAL_GetPosition(
                        Component_get_transform(closestEnemy));

                // Initialize a new coordinate vector with the enemy's current position
                Vector3 coord(0, 0, 0);
                coord.X = gotten.X;
                coord.Y = gotten.Y;
                coord.Z = gotten.Z;

                // Adjust to Y coordinate by adding 1.0f
                //coord.Y += 1.7f;//coord.Y += 1.2f; 2 buga muito

                coord.Y += 0.1f + 0.03f;//coord.Y += 1.2f;

                // Defina a nova posição do inimigo
                Transform_set_position(Component_get_transform(closestEnemy), coord);
            }
        }*/
    }
    return Old_Physics$$Raycast(origin, direction, hitInfo, maxDistance, layerMask);
}

bool(*FastWeapon)(bool* instance);
bool _FastWeapon(bool* instance) {
    if (MasterBool.ultraswitch && MasterBool.enableESP) {
        return false;
   }
    return FastWeapon(instance);
}

bool(*HighJump)(bool* instance);
bool _HighJump(bool* instance) {
    if (MasterBool.highjump && MasterBool.enableESP) {
        return false;
    }
    return HighJump(instance);
}

bool(*ResetGuest)(bool* instance);
bool _ResetGuest(bool* instance) {
    if (MasterBool.resetguest && MasterBool.enableESP ) {
        return true;
    }
    return ResetGuest(instance);
}

bool(*UnlimitedAmmo)(bool* instance);
bool _UnlimitedAmmo(bool* instance) {
    if (MasterBool.UnlimitedAmmo && MasterBool.enableESP ) {
        static const unsigned char result[] = { 0xBC, 0x00, 0xA0, 0xE3, 0x1E, 0xFF, 0x2F, 0xE1 };
        return result;
    }
    return UnlimitedAmmo(instance);
}
bool(*MedikitRun)(bool* instance);
bool _MedikitRun(bool* instance) {
    if (MasterBool.medikitrun && MasterBool.enableESP ) {
        return false;
    }
    return MedikitRun(instance);
}

bool(*DoubleGun)(bool* instance);
bool _DoubleGun(bool* instance) {
    if (MasterBool.doublegun && MasterBool.enableESP ) {
        return true;
    }
    return DoubleGun(instance);
}
bool (*orig_ghost)(void* _this, int value);
bool _ghost(void* _this, int value) {
    if (_this != NULL) {
        if (MasterBool.teleport && MasterBool.enableESP) {
            return true;
        }
    }
    return orig_ghost(_this, value);
}

bool(*TeleportCar)(bool* instance);
bool _TeleportCar(bool* instance) {
    if (MasterBool.teleportcar && MasterBool.enableESP ) {
        static const unsigned char result[] = { 0x01, 0x00, 0xA0, 0xE3, 0x1E, 0xFF, 0x2F, 0xE1 };
        return result;
    }
    return TeleportCar(instance);
}

bool(*IsSwimSurf)(bool* instance);
bool _IsSwimSurf(bool* instance) {
    if (MasterBool.cameraup && MasterBool.enableESP ) {
        static const unsigned char result[] = { 0x01, 0x00, 0xA0, 0xE3, 0x1E, 0xFF, 0x2F, 0xE1 };
        return result;
    }
    return IsSwimSurf(instance);
}

bool(*SpeedBypass)(bool* instance);
bool _SpeedBypass(bool* instance) {
    if (MasterBool.speedhackjoy && MasterBool.enableESP ) {
      //  static const unsigned char result[] = { 0x01, 0x00, 0xA0, 0xE3, 0x1E, 0xFF, 0x2F, 0xE1 };
        return true;
    }
    return SpeedBypass(instance);
}


bool(*SpeedHack)(bool* instance);
bool _SpeedHack(bool* instance) {
return true;
}
bool (*SpeedFix)();

// Hooked function for get_IsRunningOnWindows
bool _SpeedFix() {
    // Force the function to always return true
    return true;
}
/*
float(*SPEED_BACKUP)(void *thiz);
float SPEED_HOOK(void* thiz) {
    if (thiz != nullptr ) {
        if (MasterBool.speedrun && MasterBool.enableESP){
            return 2.7;//2.7
        } else
        {
            return 1.7;//2.0
        }

    }


    return SPEED_BACKUP(thiz);

}
*/




int (*orig_PlayerNetwork_TakeDamage)(void *_this, int32_t baseDamage, COW_GamePlay_IHAAMHPPLMG_o damager, void *damageInfo, int32_t weaponDataID, Vector3 firePos, Vector3 hitPos, monoList<float> checkParams, void *damagerWeaponDynamicInfo, uint32_t damagerVehicleID);
int hook_PlayerNetwork_TakeDamage(void *_this, int32_t baseDamage, COW_GamePlay_IHAAMHPPLMG_o damager, void *damageInfo, int32_t weaponDataID, Vector3 firePos, Vector3 hitPos, monoList<float> checkParams, void *damagerWeaponDynamicInfo, uint32_t damagerVehicleID) {
    if (_this != NULL && MasterBool.aimbody)
    {
        if (damageInfo != NULL) {
            *(int *)((long) damageInfo + 0xC) = 1;
            hitPos = GetHeadPosition(_this);
        }
    }
    return orig_PlayerNetwork_TakeDamage(_this, baseDamage, damager, damageInfo, weaponDataID, firePos, hitPos, checkParams, damagerWeaponDynamicInfo, damagerVehicleID);
}


#define lbil2cpp(address, original, backup)  DobbyHook((void *)getRealOffset(address), (void *)original, (void **)&backup)
#define lbunity(address, original, backup)  DobbyHook((void *)getRealOffsetUnity(address), (void *)original, (void **)&backup)

void *pthreadcreate(void *arg) {

    while (true) {


        if (getRealOffset(0) != 0) {

            lbil2cpp(string2Offset(OBFUSCATE("0x3696784")), _GetCurrentRunningHandler, GetCurrentRunningHandler);
            lbil2cpp(0x1517CBC, _FastWeapon, FastWeapon);   //	public Boolean get_InSwapWeaponCD() { }
          // // lbil2cpp(0x146db54, _MedikitRun, MedikitRun);//public Boolean IsMoving()
          //  lbil2cpp(0x3e5dc98, _MedikitRun, MedikitRun);//private Void OnPreparationNewCancel(Object[] param) { }
            lbil2cpp(0x6DA94BC, _SpeedHack, SpeedHack);
            lbil2cpp(0x15FF2A8, _SpeedHack, SpeedHack);
       //     lbil2cpp(0x2B1B288, SPEED_HOOK, SPEED_BACKUP);
         //   lbil2cpp(0x14924ec, _IsSwimSurf, IsSwimSurf);//public Boolean get_IsSwimSurf()
           // lbil2cpp(0x1a32764, hook_PlayerNetwork_TakeDamage, orig_PlayerNetwork_TakeDamage);//public override Int32 TakeDamage(Int32 baseDamage, PlayerID damager, DamageInfo damageInfo, Int32 weaponDataID, Vector3 firePos, Vector3 hitPos, List`1 checkParams, WeaponDynamicInfo damagerWeaponDynamicInfo, UInt32 vehicleDataID) { }
        //    DobbyHook((void *)getRealOffset(string2Offset(OBFUSCATE("0x8458A28"))), (void*)New_Physics$$Raycast, (void **)&Old_Physics$$Raycast);
           // lbil2cpp(0x1F4D0EC, _ghost, orig_ghost);
            //lbil2cpp(0x5316084, _ResetGuest, ResetGuest);//public static Void SendTrainingLocalGameLog()
        //    lbil2cpp(0x2928068, _HighJump, HighJump);
            //lbil2cpp(0x16b0164, _DoubleGun, DoubleGun);
            //lbil2cpp(0x21558a0, _TeleportCar, TeleportCar);
            //lbil2cpp(0x1278444, AimSilent_Hook, AimSilent_Backup);
            pthread_exit(0);
        }

    }
    return NULL;
}
bool hookEnabled = false;





__attribute__((constructor))
void initializer() {

    pthread_t ptid12;
    pthread_create(&ptid12, nullptr, pthreadcreate, nullptr);

    pthread_t ptid1;
    pthread_create(&ptid1, nullptr, CreateServer, nullptr);


}

     