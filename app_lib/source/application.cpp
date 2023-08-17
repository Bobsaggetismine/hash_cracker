#include "stl_includes.hpp"
#include "application.hpp"



int main(int argc, char* argv[])
{
	try {
		return application::m_app->main(argc, argv);
	}
	catch (const char* msg) {
		std::cout << msg << std::endl;
	}
	catch (std::exception const& e) {
		std::cout << e.what() << std::endl;
	}
	catch (...) {
		std::cout << "unknown error" << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}


application* application::m_app = nullptr;


int application::main(int argc, char* argv[])
{
	m_args.assign(argv, argv + argc);
	init();
	return execute();
}


application::application()
{
	if (m_app)
	{
		std::cout << "cannot have duplicate app instances" << std::endl;
		std::terminate();
	}
	m_app = this;
	
}