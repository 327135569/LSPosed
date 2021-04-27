//
// Created by VIP on 2021/4/25.
//

#include "bypass_sig.h"
#include "native_api.h"
#include "native_util.h"
#include "jni_helper.h"
#include "dl_util.h"


namespace lspd {
    CREATE_HOOK_STUB_ENTRIES(
            "__openat",
            int, __openat,
            (int fd, const char* pathname, int flag), {

        return backup(fd, pathname, flag);
    });

    LSP_DEF_NATIVE_METHOD(void, SigBypass, enableOpenatHook) {
        ScopedDlHandle scopedDlHandle(kLibcPath.c_str());
        lspd::HookSym(scopedDlHandle.Get(), __openat);
    }

    static JNINativeMethod gMethods[] = {
            LSP_NATIVE_METHOD(SigBypass, enableOpenatHook, "()V")
    };

    void RegisterBypass(JNIEnv* env) {
        REGISTER_LSP_NATIVE_METHODS(SigBypass);
    }
}