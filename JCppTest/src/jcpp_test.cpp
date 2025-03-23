#include "../catch2/catch_amalgamated.hpp"

int main(int argc, char* argv[])
{
	Catch::Session session;
	session.configData().shouldDebugBreak = true;
	session.configData().showSuccessfulTests = true;
	session.configData().showDurations = Catch::ShowDurations::Always;

    int const cmdErrorCode = session.applyCommandLine(argc, argv);
    if(cmdErrorCode != 0)
	{
		return cmdErrorCode;
	}

    int const result = session.run(argc, argv);

	return result;
}
