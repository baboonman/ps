# include <string>
# include "OpenCLSimulation.hpp"

static void					usage()
{
	std::cout << "===== INPUT =====" << std::endl
			<< "[W] [S]\t\tforward / backward" << std::endl
			<< "[A] [D]\t\tleft / right" << std::endl
			<< "[E] <S[E]>\tup / down" << std::endl
			<< "SPACE\t\tplay / pause" << std::endl
			<< "[1] -> [4]\tchange init shape" << std::endl
			<< "[R] [X]\t\treset sim / change gravity eq + reset" << std::endl
			<< "[G] [I]\t\tgravity on/off / invert gravity" << std::endl
			<< "[F]\t\ttoggle gravity fixed mode" << std::endl
			<< "[V]\t\ttoggle camera mode" << std::endl
			<< "=== Other Info ===" << std::endl
			<< "When fixed mode is off -> mouse change the position of the gravity center" << std::endl
			<< "When camera mode is on -> deactivate input except those used by the camera." << std::endl;
}

static int						getNbParticles(int ac, char **av)
{
	int					nbParticles = 10000;

	if (ac == 2) {
		try {
			nbParticles = std::stoi(av[1]);
		} catch (const std::exception & e) {
			std::cout << "Bad conversion: " << e.what() << std::endl;
		}
		if (nbParticles < 100 || nbParticles > 10000000) {
			std::cout << "Number of particles not valid, range is [100 ; 10,000,000]" << std::endl;
			nbParticles = 10000;
		}
	}
	std::cout.precision(2);
	std::cout << std::scientific
			  << "Running simulation with " << static_cast<float>(nbParticles)
			  << " particles" << std::endl
			  << std::fixed;
	return nbParticles;
}

int						main(int ac, char **av)
{
	int					nbParticles = getNbParticles(ac, av);
	OpenCLSimulation	*sim = new OpenCLSimulation(nbParticles);
	
	usage();	
	sim->runSimulation();
	delete sim;
	return (0);
}
