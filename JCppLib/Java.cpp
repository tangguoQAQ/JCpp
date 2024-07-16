#include "Java.h"
#include <stdexcept>
#include <string.h>
#include <algorithm>
#include <cassert>
#include "Util.h"

namespace Java {
	JavaVM* jvm = nullptr;
	JNIEnv* env = nullptr;

	/**
	 * @brief �������߳��е��ô˺�������Ҫ������������ Java::Destroy()��
	 * @throws Java::Exception::JniException
	 */
	void Initialize(ConstString costumClasspath) noexcept(false)
	{
		JavaVMInitArgs vm_args;
		constexpr int nOption = 2;
		JavaVMOption* options = new JavaVMOption[nOption];
		std::string classpathOption = "-Djava.class.path=";
		classpathOption += costumClasspath;
		options[0].optionString = const_cast<char*>(classpathOption.c_str());
		options[1].optionString = const_cast<char*>("-Djava.compiler=NONE");
		vm_args.version = JNI_VERSION_1_8;
		vm_args.nOptions = nOption;
		vm_args.options = options;
		vm_args.ignoreUnrecognized = false;

		const jint rc = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
		delete options;
		Util::ThrowIf(rc != JNI_OK, Exception::JniException("Failed to create Java VM"));
	}

	void Destroy() noexcept
	{
		if(jvm != nullptr) jvm->DestroyJavaVM();
	}

	namespace Exception
	{
		/**
		 * @brief ÿ�ε����� JNI �ӿں󣬶�Ӧ�õ��ô˺���������Ƿ������쳣��
		 * @brief e.g. ThrowIfChecked(Exception::CallMethodFailed);
		 */
		void ThrowIfChecked(JniException e) noexcept(false)
		{
			auto exception = env->ExceptionOccurred();
			if(exception == nullptr) return;

			auto jsExceptionMsg = static_cast<::jstring>(env->CallObjectMethod(exception,
				env->GetMethodID(env->GetObjectClass(exception), "toString", "()Ljava/lang/String;")));
			env->DeleteLocalRef(exception);
			env->ExceptionClear();
			e.SetMsg(jsExceptionMsg);
			env->DeleteLocalRef(jsExceptionMsg);
			throw e;
		}

		/**
		 * @brief �� b Ϊ�棬�������׳� JNI �Ƿ����쳣�������׳� e��
		 * @brief e.g. ThrowIf(pLocalClass == nullptr, JniException(ClassNotFound, className));
		 */
		void ThrowIf(bool b, JniException e) noexcept(false)
		{
			if(!b) return;

			ThrowIfChecked(e);
			// �����е�����˵�� ThrowIfChecked û���׳��쳣��
			throw e;
		}

		JniException::JniException(EType t, ConstString msg) : m_t(t), std::exception(msg)
		{
			szSelfMsg[0] = 0;
		}
		JniException::JniException(ConstString msg) : JniException(Jni, msg) {}
		JniException::JniException(EType t) : m_t(t) {}

		void JniException::SetMsg(::jstring msg)
		{
			const auto len = env->GetStringLength(msg);
			env->GetStringUTFRegion(msg, 0, len > MAX_MSG_LEN ? MAX_MSG_LEN : len, szSelfMsg);
		}

		const char* JniException::what() const noexcept
		{
			static char szWhat[MAX_MSG_LEN];
			::strncpy_s(szWhat, MAX_MSG_LEN, ENUM_NAMES_OF_EType.GetStr(static_cast<int>(m_t)).c_str(), MAX_MSG_LEN);
			::strncat_s(szWhat, ": ", MAX_MSG_LEN);
			::strncat_s(szWhat, szSelfMsg[0] == 0 ? std::exception::what() : szSelfMsg, MAX_MSG_LEN);

			return szWhat;
		}
	}

	/**
	 * @brief ��ȡ jclass �����档��Ҫ�ڶ��̻߳����е��ô˺�������Ϊ����ʹ�� unordered_map��
	 * @throws Java::Exception::JniException
	 */
	const std::shared_ptr<::jclass>* GetClassAndCache(ConstString className) noexcept(false)
	{
		// jclass ������Ϊ�˶�� JClass ������ͬһ�� jclass ʵ����
		static auto classCache = std::unordered_map<std::string, std::shared_ptr<::jclass>>{};

		// ����Ƿ�ִ������
		if(className[::strnlen_s(className, MAX_CLASSPATH_LEN) - 1] == '?')
		{
			char sz[MAX_CLASSPATH_LEN];
			::strncpy_s(sz, MAX_CLASSPATH_LEN, className, MAX_CLASSPATH_LEN);
			sz[::strnlen_s(className, MAX_CLASSPATH_LEN) - 1] = 0;

			auto& source = classCache[sz];
			if(source.use_count() != 1)	return nullptr;

			env->DeleteGlobalRef(*source);
			for(auto it = classCache.begin(); it != classCache.end();)
			{
				if(it->second == source)
				{
					source.reset();
					classCache.erase(it++);
					break;
				}
				else it++;
			}

			return nullptr;
		}

		if(classCache.find(className) != classCache.end())
			return &(classCache[className]);

		using namespace Exception;

		char classpath[MAX_CLASSPATH_LEN];
		::strncpy_s(classpath, MAX_CLASSPATH_LEN, className, MAX_CLASSPATH_LEN);
		std::replace(classpath, classpath + strlen(classpath), '.', '/');
		const auto pLocalClass = env->FindClass(classpath);
		ThrowIf(pLocalClass == nullptr, JniException(ClassNotFound, className));

		const auto pGlobalClass = static_cast<::jclass>(env->NewGlobalRef(pLocalClass));
		env->DeleteLocalRef(pLocalClass);
		ThrowIf(pGlobalClass == nullptr, "Failed to create global reference to class");

		classCache.insert({ className, std::shared_ptr<::jclass>(new jclass(pGlobalClass)) });
		return &classCache[className];
	}
}
