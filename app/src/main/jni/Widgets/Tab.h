class Tab {

public:
    void create(JNIEnv* env, const char* name ) {
        jclass CMenu = env->FindClass("com/ashu/Menu");
        jmethodID MTab = env->GetStaticMethodID(CMenu, "addTab", "(Ljava/lang/String;)V");
        return env->CallStaticVoidMethod(CMenu, MTab, env->NewStringUTF(name));
    }

};