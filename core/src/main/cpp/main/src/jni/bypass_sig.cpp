//
// Created by VIP on 2021/4/25.
//

#include "bypass_sig.h"
#include "native_api.h"
#include "native_util.h"
#include "jni_helper.h"
#include "dl_util.h"


namespace lspd {

    std::string hookedPackageName;
    std::string redirectPath;
    std::string apkPathPre;

    CREATE_HOOK_STUB_ENTRIES(
            "__openat",
            int, __openat,
            (int fd, const char* pathname, int flag), {
        if (strstr(pathname, apkPathPre.c_str()) && strstr(pathname, "/base.apk")) {
            return backup(fd, redirectPath.c_str(), flag);
        }
        return backup(fd, pathname, flag);
    });

    LSP_DEF_NATIVE_METHOD(void, SigBypass, enableOpenatHook, jstring packageName) {
        ScopedDlHandle scopedDlHandle(kLibcPath.c_str());
        lspd::HookSym(scopedDlHandle.Get(), __openat);
        JUTFString str(env, packageName);
        hookedPackageName = str.get();
        redirectPath = "/data/data/" + hookedPackageName + "/cache/lspatchapk.so";
        apkPathPre = "/data/app/" + hookedPackageName;
    }

    static JNINativeMethod gMethods[] = {
            LSP_NATIVE_METHOD(SigBypass, enableOpenatHook, "(Ljava/lang/String;)V")
    };

    void RegisterBypass(JNIEnv* env) {
        REGISTER_LSP_NATIVE_METHODS(SigBypass);
    }
}