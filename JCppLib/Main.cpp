#include <iostream>
#include "Java.h"

int main()
{
    //std::string currentPathStr = std::filesystem::current_path().string();
    const char* customClasspath = ".\\javalib\\javassist-3.30.2-GA.jar;"
        ".\\javalib\\fabric-loader-0.14.21.jar;"
        ".\\javalib\\JarPacker-1.0-SNAPSHOT.jar";
    Java::Initialize(customClasspath);
    
    {
        using namespace Java;

        //// ClassPool pool = ClassPool.getDefault();
        //jclass jcClassPool = env->FindClass("javassist/ClassPool");
        //jmethodID jmClassPool_getDefault = env->GetStaticMethodID(jcClassPool, "getDefault", "()Ljavassist/ClassPool;");
        //jobject jo_pool = env->CallStaticObjectMethod(jcClassPool, jmClassPool_getDefault);

        JClass jcClassPool("javassist.ClassPool");
        auto joPool = jcClassPool.Do<JObject>("getDefault", "()Ljavassist/ClassPool;");

        //// CtClass cModInitializer = pool.get("net.fabricmc.api.ModInitializer");
        //jmethodID jmClassPool_get = env->GetMethodID(jcClassPool, "get", "(Ljava/lang/String;)Ljavassist/CtClass;");
        //jobject jo_cModInitializer = env->CallObjectMethod(jo_pool, jmClassPool_get, env->NewStringUTF("net.fabricmc.api.ModInitializer"));

        auto joCModInitializer = joPool.Do<JObject>("get", "(Ljava/lang/String;)Ljavassist/CtClass;",
            JString("net.fabricmc.api.ModInitializer"));

        //// CtClass cMain = pool.makeClass(topPackage + ".Main");
        //jmethodID jmClassPool_makeClass = env->GetMethodID(jcClassPool, "makeClass", "(Ljava/lang/String;)Ljavassist/CtClass;");
        //jobject jo_cMain = env->CallObjectMethod(jo_pool, jmClassPool_makeClass, env->NewStringUTF("tgmoss.cpptest.Main"));

        //// cMain.addInterface(cModInitializer);
        //jclass jcCtClass = env->FindClass("javassist/CtClass");
        //jmethodID jmCtClass_addInterface = env->GetMethodID(jcCtClass, "addInterface", "(Ljavassist/CtClass;)V");
        //env->CallVoidMethod(jo_cMain, jmCtClass_addInterface, jo_cModInitializer);

        //// CtMethod mOnInitialize = CtNewMethod.copy(cModInitializer.getDeclaredMethod("onInitialize"), cMain, null);
        //jmethodID jmCtClass_getDeclaredMethod = env->GetMethodID(jcCtClass, "getDeclaredMethod", "(Ljava/lang/String;)Ljavassist/CtMethod;");
        //jobject jo_mOriginalOnInitialize = env->CallObjectMethod(jo_cModInitializer, jmCtClass_getDeclaredMethod, env->NewStringUTF("onInitialize"));
        //jclass jcCtNewMethod = env->FindClass("javassist/CtNewMethod");
        //jmethodID jmCtNewMethod_copy = env->GetStaticMethodID(jcCtNewMethod, "copy", "(Ljavassist/CtMethod;Ljavassist/CtClass;Ljavassist/ClassMap;)Ljavassist/CtMethod;");
        //jobject jo_mOnInitialize = env->CallStaticObjectMethod(jcCtNewMethod, jmCtNewMethod_copy, jo_mOriginalOnInitialize, jo_cMain, NULL);

        //// mOnInitialize.setBody("{ System.out.println(\"Hello Lotus!\"); }");
        //jclass jcCtMethod = env->FindClass("javassist/CtMethod");
        //jmethodID jmCtMethod_setBody = env->GetMethodID(jcCtMethod, "setBody", "(Ljava/lang/String;)V");
        //env->CallVoidMethod(jo_mOnInitialize, jmCtMethod_setBody, env->NewStringUTF("{ System.out.println(\"Hello Lotus!\"); }"));

        //// cMain.addMethod(mOnInitialize);
        //jmethodID jmCtClass_addMethod = env->GetMethodID(jcCtClass, "addMethod", "(Ljavassist/CtMethod;)V");
        //env->CallVoidMethod(jo_cMain, jmCtClass_addMethod, jo_mOnInitialize);

        //// cMain.writeFile(".\\out");
        //jmethodID jmCtClass_writeFile = env->GetMethodID(jcCtClass, "writeFile", "(Ljava/lang/String;)V");
        //jstring jstr_outputDir = env->NewStringUTF(".\\out");
        //env->CallVoidMethod(jo_cMain, jmCtClass_writeFile, jstr_outputDir);

        //env->DeleteLocalRef(jcClassPool);
        //env->DeleteLocalRef(jo_pool);
        //env->DeleteLocalRef(jo_cModInitializer);
        //env->DeleteLocalRef(jo_cMain);
        //env->DeleteLocalRef(jcCtClass);
        //env->DeleteLocalRef(jo_mOriginalOnInitialize);
        //env->DeleteLocalRef(jcCtNewMethod);
        //env->DeleteLocalRef(jo_mOnInitialize);
        //env->DeleteLocalRef(jcCtMethod);
        //env->DeleteLocalRef(jstr_outputDir);

    }

    Java::Destroy();
    return 0;
}