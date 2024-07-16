#define CATCH_CONFIG_RUNNER

#include "catch.hpp"
#include <Java.h>

int main(int argc, char* argv[])
{
	Catch::Session session;

	session.configData().shouldDebugBreak = true;
	session.configData().showSuccessfulTests = true;
	session.configData().showDurations = Catch::ShowDurations::OrNot::Always;

	int cmdErrorCode = session.applyCommandLine(argc, argv);
	if(cmdErrorCode) return cmdErrorCode;

	Java::Initialize();

	int result = session.run(argc, argv);

	Java::Destroy();

	return result;
}
