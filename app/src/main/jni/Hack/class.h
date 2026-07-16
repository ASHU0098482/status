#ifndef ANDROID_MOD_MENU_HOOK_H
#define ANDROID_MOD_MENU_HOOK_H


class Vvector3 {
public:
    float X;
    float Y;
    float Z;
    Vvector3() : X(0), Y(0), Z(0) {}
    Vvector3(float x1, float y1, float z1) : X(x1), Y(y1), Z(z1) {}
    Vvector3(const Vvector3 &v);
    ~Vvector3();
};
Vvector3::Vvector3(const Vvector3 &v) : X(v.X), Y(v.Y), Z(v.Z) {}
Vvector3::~Vvector3() {}


static void *Player_GetActiveWeapon(void *LocalPlayer)
{
    void *(*_Player_GetActiveWeapon)(void *LocalPlayer) = (void *(*)(void*)) getRealOffset(0x152DC0C);
    return _Player_GetActiveWeapon(LocalPlayer);
}

void *GameFacade_CurrentLocalPlayer()
{
    void *(*_CurrentLocalPlayer)() = (void*(*)())getRealOffset(0x1A391FC);
    return _CurrentLocalPlayer();
}

// Namespace : COW
// classe    : internal class GameFacade
// Address   : CurrentMatch() { } // RVA: 0x1515568
void *GameFacade_CurrentMatch()
{
    void *(*_CurrentMatch)() = (void*(*)())getRealOffset(0x1A38BFC);
    return _CurrentMatch();
}

static Vector3 Transform_INTERNAL_GetPosition(void *player)
{
    Vector3 out = Vector3::Zero();
    void (*_Transform_INTERNAL_GetPosition)(void *transform, Vector3 * out) = (void (*)(void *, Vector3 *))getRealOffset(pAddress.Transform_INTERNAL_GetPosition);
    _Transform_INTERNAL_GetPosition(player, &out);
    return out;
}

static void Transform_INTERNAL_SetPosition(void *player, Vvector3 inn) {
    void (*_Transform_INTERNAL_SetPosition)(void *transform, Vvector3 in) = (void (*)(void *, Vvector3))getRealOffset(pAddress.Transform_SetPosition);
    _Transform_INTERNAL_SetPosition(player, inn);
}
static void *Component_get_transform(void *player)
{
    void *(*_Component_GetTransform)(void *nullo) = (void *(*)(void *))getRealOffset(pAddress.Component_GetTransform);
    return _Component_GetTransform(player);
}
static void *Component_get_transform2(void *player)
{
    void *(*_Component_GetTransform2)(void *component) = (void *(*)(void *))getRealOffset(pAddress.Component_GetTransform);
    return _Component_GetTransform2(player);
}
static Vector3 Transform_INTERNAL_GetPosition2(void *player) {
    Vector3 out = Vector3::Zero();
    void (*_Transform_INTERNAL_GetPosition2)(void *transform, Vector3 * out) = (void (*)(void *, Vector3 *))getRealOffset(pAddress.Transform_INTERNAL_GetPosition);
    _Transform_INTERNAL_GetPosition2(player, &out);
    return out;
}

static void *get_main() {
    void *(*nget_main)(void *Instance) = (void *(*)(void *))getRealOffset(pAddress.Camera_main);
    return nget_main(nullptr);
}


Vector3 WorldToScreenPoint(Vector3 pos) {
    auto main = get_main();
    if (main) {
        auto Camera_WorldToScreenPoint = (Vector3 (*)(void *, Vector3))getRealOffset(pAddress.WorldToScreenPoint);
        return Camera_WorldToScreenPoint(main, pos);
    }
    return {0, 0, 0};
}


static Vector3 GetForward(void *player) {
    Vector3 (*NGetForward)(void *players) = (Vector3 (*)(void *))getRealOffset(pAddress.get_forward);
    return NGetForward(player);
}
static Vector3 GetForward2(void *player) {
    Vector3 (*_GetForward2)(void *players) = (Vector3 (*)(void *))getRealOffset(pAddress.get_forward);
    return _GetForward2(player);
}
static int GetHp(void *instance) {
    return ((int (*)(void *)) getRealOffset(pAddress.GetHp))(instance);
}

static bool get_IsSighting(void *player) {
    bool (*_get_IsSighting)(void *players) = (bool (*)(void *))getRealOffset(pAddress.get_IsSighting);
    return _get_IsSighting(player);
}
static bool IsFiringPlayer(void *player) {
    bool (*_IsFiringPlayer)(void *players) = (bool (*)(void *))getRealOffset(pAddress.get_IsFiringFromPRI);
    return _IsFiringPlayer(player);
}
static void SwapWeapon2(void *player, int POFFNNMOOBM, bool GDKLMFLNNGM) {
    void (*_SwapWeapon)(void *player, int POFFNNMOOBM, bool GDKLMFLNNGM) = (void (*)(void *, int, bool))getRealOffset(0x1A236E0);
    _SwapWeapon(player, POFFNNMOOBM, GDKLMFLNNGM);
}

static void Transform_set_position(void *player, Vector3 inn)
{
    void (*_Transform_SetPosition)(void *transform, Vector3 in) = (void (*)(void *, Vector3))getRealOffset(pAddress.Transform_SetPosition);
    _Transform_SetPosition(player, inn);
}
static void Transform_set_position2(void *player, Vvector3 inn)
{
    void (*_Transform_SetPosition2)(void *transform, Vvector3 in) = (void (*)(void *, Vvector3))getRealOffset(pAddress.Transform_SetPosition);
    _Transform_SetPosition2(player, inn);
}
static Vector3 Transform_get_forward(void *player)
{
    Vector3 (*_GetForward)(void *players) = (Vector3(*)(void *))getRealOffset(pAddress.get_forward);
    return _GetForward(player);
}

static void SetResolution(int32_t width, int32_t height, int32_t fullscreenMode, int32_t preferredRefreshRate)
{
    void (*_SetResolution)(int32_t width, int32_t height, int32_t fullscreenMode, int32_t preferredRefreshRate) = (void (*)(int32_t,int32_t,int32_t,int32_t))getRealOffset(pAddress.SetResolution);
    return _SetResolution(width,height,fullscreenMode,preferredRefreshRate);
}




static bool get_isVisible(void *player) {
    bool (*Nget_isVisible)(void *players) = (bool (*)(void *))getRealOffset(pAddress.get_isVisible);
    return Nget_isVisible(player);
}

static bool IsLocalTeammate(void *instance) {
    return ((bool (*)(void *)) getRealOffset(pAddress.IsLocalTeammate))(instance);
}


static Vector3 CameraPosition(void *player)
{
    return Transform_INTERNAL_GetPosition(Component_get_transform(get_main()));
}
void* TransformNode(void *_this)
{
    return ((void* (*)(void *))getRealOffset(pAddress.TransformNode))(_this);
}

static void *Current_Local_Player() {
    void *(*_Local_Player)(void *players) = (void *(*)(void *))getRealOffset(pAddress.Current_Local_Player);
    return _Local_Player(NULL);
}

static bool IsStreamerVisible(void *player) {
    bool (*_IsStreamerVisible)(void *players) = (bool (*)(void *))getRealOffset(pAddress.get_isVisibleMoita);
    return _IsStreamerVisible(player);
}
static void *IsDieing(void *player) {
    void *(*_IsDieing)(void *players) = (void *(*)(void *))getRealOffset(pAddress.get_IsDieing);
    return _IsDieing(player);
}

void *get_HeadCollider(void *pthis)
{
    return ((void* (*)(void *))getRealOffset(string2Offset(OBFUSCATE("0x1516130"))))(pthis);
}

void *get_gameObject(void *Pthis)
{
    return ((void* (*)(void *))getRealOffset(string2Offset(OBFUSCATE("0x7BACFDC"))))(Pthis);
}
static bool Physics_Raycast(Vector3 camLocation, Vector3 headLocation, unsigned int LayerID, void* collider) {
    bool (*_Physics_Raycast)(Vector3 camLocation, Vector3 headLocation, unsigned int LayerID, void* collider) = (bool(*)(Vector3, Vector3, unsigned int, void*))getRealOffset(0x14CF4E0);
    return _Physics_Raycast(camLocation, headLocation, LayerID, collider);
}

Vector3 GetHeadPosition(void* player) {
    return Transform_INTERNAL_GetPosition(*(void**) ((uint64_t) player + 0x3B8));
}

Vector3 GetHipPosition(void* player) {
    return Transform_INTERNAL_GetPosition(*(void**) ((uint64_t) player + 0x3CC));
}
bool isVisible_Aimbot(void * player)
{
    void *hitObj = NULL;
    void *HeadTF = TransformNode(*(void **) ((uint64_t) player + pAddress.HeadTF));
    Vector3 EnemyHeadPosition = Transform_INTERNAL_GetPosition(HeadTF);
    Vector3 LocalPlayerPos = CameraPosition(Current_Local_Player());
    return !Physics_Raycast(LocalPlayerPos, EnemyHeadPosition,12, &hitObj);

}
bool isVisible_Aimbot_Pos(const Vector3 &targetPos, void *LocalPlayer)
{
    if (LocalPlayer == nullptr) return false;

    void *hitObj = NULL;
    Vector3 LocalPlayerPos = CameraPosition(LocalPlayer);

    // same raycast style as your isVisible_Aimbot
    return !Physics_Raycast(LocalPlayerPos, targetPos, 12, &hitObj);
}

static monoString* get_NickName(void *player)
{
    monoString* (*_get_NickName)(void *players) = (monoString * (*)(void *)) getRealOffset(0x14A2074);
    return _get_NickName(player);
}
char get_Chars(monoString *str, int index) {
    char (*_get_Chars)(monoString *str, int index) = (char (*)(monoString *, int))getRealOffset(0x7AA5690); //public char get_Chars(int index) { }
    return _get_Chars(str, index);
}
static void *CurrentUIScene() {
    void *(*_CurrentUIScene)(void *nuls) = (void *(*)(void *))getRealOffset(0x1A628E8);
    return _CurrentUIScene(NULL);
}
static void ShowAssistantText(void *player, monoString *nick, monoString *grup) {
    if (player == nullptr || nick == nullptr || grup == nullptr) return;

    typedef void (*ShowTextFn)(void *, monoString *, monoString *);
    ShowTextFn _ShowAssistantText = (ShowTextFn)getRealOffset(0x15CBC78);

    if (_ShowAssistantText) {
        _ShowAssistantText(player, nick, grup);
    }
}

static monoString *U3DStr(const char *str) {
    monoString *(*String_CreateString)(void *_this, const char *str) = (monoString * (*)(void *, const char *))getRealOffset(pAddress.CreateString);
    return String_CreateString(NULL, str);
}
static monoString *U3DStrFormat(float distance) {
    char buffer[128] = {0};
    sprintf(buffer, "Test", distance);
    return U3DStr(buffer);
}

static void *get_MyPhsXData(void *player)
{
    void *(*_get_MyPhsXData)(void *component) = (void *(*)(void *))getRealOffset(0x139D528);
    return _get_MyPhsXData(player);
}



#endif