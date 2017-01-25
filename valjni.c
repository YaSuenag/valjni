#include <jvmti.h>
#include <jni.h>

#include <stdio.h>
#include <valgrind/valgrind.h>


static jniNativeInterface *origJNIFuncTable, *hookJNIFuncTable;

static jobject NewGlobalRefHook(JNIEnv *env, jobject obj){
  jobject ret = origJNIFuncTable->NewGlobalRef(env, obj);

  if(ret != NULL){
    VALGRIND_MALLOCLIKE_BLOCK(ret, sizeof(jobject), 0, 1);
  }

  return ret;
}

static void DeleteGlobalRefHook(JNIEnv *env, jobject globalRef){
  origJNIFuncTable->DeleteGlobalRef(env, globalRef);

  if(globalRef != NULL){
    VALGRIND_FREELIKE_BLOCK(globalRef, 0);
  }

}


void JNICALL OnVMStart(jvmtiEnv *jvmti, JNIEnv *env){
  jvmtiError err;

  err = (*jvmti)->GetJNIFunctionTable(jvmti, &origJNIFuncTable);
  if(err != JVMTI_ERROR_NONE){
    char *errstr;
    (*jvmti)->GetErrorName(jvmti, err, &errstr);
    fprintf(stderr, "valjni GetJNIFunctinTable: %s.\n", errstr);
    (*jvmti)->Deallocate(jvmti, (unsigned char *)errstr);
  }

  err = (*jvmti)->GetJNIFunctionTable(jvmti, &hookJNIFuncTable);
  if(err != JVMTI_ERROR_NONE){
    char *errstr;
    (*jvmti)->GetErrorName(jvmti, err, &errstr);
    fprintf(stderr, "valjni GetJNIFunctinTable(2): %s.\n", errstr);
    (*jvmti)->Deallocate(jvmti, (unsigned char *)errstr);
  }

  hookJNIFuncTable->NewGlobalRef = NewGlobalRefHook;
  hookJNIFuncTable->DeleteGlobalRef = DeleteGlobalRefHook;

  err = (*jvmti)->SetJNIFunctionTable(jvmti, hookJNIFuncTable);
  if(err != JVMTI_ERROR_NONE){
    char *errstr;
    (*jvmti)->GetErrorName(jvmti, err, &errstr);
    fprintf(stderr, "valjni SetJNIFunctinTable: %s.\n", errstr);
    (*jvmti)->Deallocate(jvmti, (unsigned char *)errstr);
  }

}

JNIEXPORT jint JNICALL Agent_OnLoad(JavaVM *vm, char *options, void *reserved){
  jvmtiEnv *jvmti;

  if(RUNNING_ON_VALGRIND == 0){
    fprintf(stderr, "valjni WARNING: valgrind is not run.\n");
  }

  (*vm)->GetEnv(vm, (void **)&jvmti, JVMTI_VERSION_1);

  jvmtiEventCallbacks callbacks = {0};
  callbacks.VMStart = &OnVMStart;

  (*jvmti)->SetEventCallbacks(jvmti, &callbacks, sizeof(jvmtiEventCallbacks));
  (*jvmti)->SetEventNotificationMode(jvmti, JVMTI_ENABLE, JVMTI_EVENT_VM_START, NULL);

  return JNI_OK;
}

JNIEXPORT void JNICALL Agent_OnUnload(JavaVM *vm){
 // Do nothing
}
