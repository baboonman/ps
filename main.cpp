# include <cstdio>
# include "OpenCLSimulation.hpp"

int						getNbParticles(int ac, char **av)
{
	int					nbParticles = 1000;

	if (ac == 2)
		nbParticles = atoi(av[1]); 
	std::cout.precision(2);
	std::cout << std::scientific;
	std::cout << static_cast<float>(nbParticles) << " particles to render" << std::endl;
	std::cout << std::fixed;
	return nbParticles;
}

int						main(int ac, char **av)
{
	int					nbParticles = getNbParticles(ac, av);
	OpenCLSimulation	*sim = new OpenCLSimulation(nbParticles);
	
	std::cout << "==OpenCL Simulation==" << std::endl;
	sim->runSimulation();
	std::cout << "==END==" << std::endl;
	delete sim;
	return (0);
}
